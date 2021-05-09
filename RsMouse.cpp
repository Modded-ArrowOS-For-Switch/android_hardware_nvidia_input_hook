/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "RsMouse"

// #define LOG_NDEBUG 0

#include <chrono>
#include <cmath>
#include <android/log.h>
#include <cutils/native_handle.h>
#include <linux/input.h>
#include <log/log.h>
#include "InputHook.h"
#include "DeviceDb.h"
#include "RsMouse.h"

namespace inputhook {
namespace device {
    constexpr std::string_view Name{"Right-Stick Mouse"};
    constexpr int16_t Bus{BUS_BLUETOOTH}; //!< Pretend to be bluetooth as not all apps will accept virtual devices
    constexpr int16_t Pid{0}; //!< 0 PID/VID is used to identify internal devices
    constexpr int16_t Vid{0};
    constexpr int16_t Version{1};
}

namespace cursor {
    constexpr auto UpdateRate{std::chrono::milliseconds{1000} / 60}; //!< 60Hz update rate
    constexpr float Deadzone{0.1f};
    constexpr float Power{3.0f}; //!< Power for cursor velocity curve
    constexpr float SpeedCoeffFinal{27.0f}; //! Coefficient for the final output cursor speed
    constexpr auto FadeTime{std::chrono::seconds{15}}; //! Maximum time it takes the cursor to fade (should match frameworks/base/libs/input/PointerController.cpp)
}

RsMouse::RsMouse(const DeviceDb &deviceDb) : mDeviceDb(deviceDb) {}

RsMouse::~RsMouse() {
    mExiting = true;

    if (mMouseThread.joinable())
        mMouseThread.join();
}

static float Deadzone(float value, float deadzone) {
    if (value >= 0.0f && value < deadzone)
        return deadzone;

    if (value < 0.0f && value > -deadzone)
        return -deadzone;

    return value;
}

void RsMouse::MouseMain() {
    float accumulateX{}, accumulateY{};
    auto activeTime{std::chrono::system_clock::now()};

    while (!mExiting) {
        auto coords{mStickCoords.load()};
        float adjustedX{std::abs(Deadzone(coords.rsX, cursor::Deadzone)) - cursor::Deadzone};
        float adjustedY{std::abs(Deadzone(coords.rsY, cursor::Deadzone)) - cursor::Deadzone};

        float combined{std::min(adjustedX + adjustedY, 1.0f - cursor::Deadzone)};
        float combinedPow{std::pow(combined, cursor::Power)};

        int32_t changeX{}, changeY{};
        if (adjustedX != 0.0f) {
            float rsX = combinedPow * adjustedX * ((coords.rsX > 0.0f) ? cursor::SpeedCoeffFinal : -cursor::SpeedCoeffFinal);

            changeX = static_cast<int32_t>(std::round(accumulateX + rsX)) - static_cast<int32_t>(std::round(accumulateX));
            accumulateX += rsX;
            if (changeX)
                mInjector.SendRel(REL_X, changeX);
        }

        if (adjustedY != 0.0f) {
            float rsY = combinedPow * adjustedY * ((coords.rsY > 0.0f) ? cursor::SpeedCoeffFinal : -cursor::SpeedCoeffFinal);

            changeY = static_cast<int32_t>(std::round(accumulateY + rsY)) - static_cast<int32_t>(std::round(accumulateY));
            accumulateY += rsY;
            if (changeY)
                mInjector.SendRel(REL_Y, changeY);
        }

        if (changeX || changeY) {
            mInjector.SendSynReport();
            activeTime = std::chrono::system_clock::now();
            mCanClick = true;
        }

        if (mCanClick) {
            if (std::chrono::system_clock::now() - activeTime > cursor::FadeTime) {
                mCanClick = false;
                accumulateX = accumulateY = 0.0f; // Take this oppertunity to reset the accumulate variable to prevent {over, under}flows, however unlikely they are
            }
        }

        std::this_thread::sleep_for(cursor::UpdateRate);
    }
}

void RsMouse::Register() {
    if (mRegistered)
        LOG_FATAL("Cannot register RsMouse twice!");

    mInjector.ConfigureBegin(std::string{device::Name}.c_str(), device::Bus, device::Vid, device::Pid, device::Version);
    mInjector.ConfigureInputProperty(INPUT_PROP_POINTER);
    mInjector.ConfigureKey(BTN_LEFT);
    mInjector.ConfigureKey(BTN_RIGHT);
    mInjector.ConfigureRel(REL_X);
    mInjector.ConfigureRel(REL_Y);
    mInjector.ConfigureEnd();

    auto ret{mInjector.GetError()};
    if (ret) {
        ALOGE("Failed to register RsMouse: %d", ret);
    } else {
        mMouseThread = std::thread{&RsMouse::MouseMain, this};
        mRegistered = true;
    }
}

Response RsMouse::FilterEvent(HidlInputEvent &iev, int32_t &deviceId) {
    if (!mRegistered)
        return Response::EVENT_DEFAULT;

    // Replace R1/R2 clicks with RsMouse clicks if possible
    if (mCanClick) {
        if (iev.type == EV_ABS && iev.code == ABS_RZ) {
            mInjector.SendKey(BTN_LEFT, iev.value > 0);
            mInjector.SendSynReport();
            return Response::EVENT_SKIP;
        } else if (iev.type == EV_ABS && iev.code == BTN_TR) {
            mInjector.SendKey(BTN_RIGHT, iev.value);
            mInjector.SendSynReport();
            return Response::EVENT_SKIP;
        }
    }

    return Response::EVENT_DEFAULT;
}

bool RsMouse::NotifyMotionState(int32_t deviceId, const AnalogCoords &pc, bool handled) {
    if (!mRegistered)
        return false;

    // If the app handles any motion event then we stop grabbing click inputs
    if (handled)
        mCanClick = false;
    else
        mStickCoords = pc;


    return true;
}

} // namespace inputhook
