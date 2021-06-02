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

#ifndef INPUTHOOK_RSMOUSE_H
#define INPUTHOOK_RSMOUSE_H

#include <atomic>
#include <thread>
#include "EvdevInjector.h"
#include "Common.h"

namespace inputhook {

class DeviceDb;

class RsMouse {
  private:
    // RsMouse thread stuff
    std::atomic<AnalogCoords> mStickCoords{};
    std::atomic_bool mExiting{}; //!< If the RsMouse thread should exit
    std::atomic_bool mCanClick{}; //!< Controls whether R3 press events will be treated as mouse clicks or passed through
    bool mDisabled{};
    std::thread mMouseThread;

    const DeviceDb &mDeviceDb;
    EvdevInjector mInjector;

    bool mRegistered{}; //!< If the RsMouse input device  has been registered
    int32_t rightStickButtonState{}; //!< Keeps track of whether the right stick button has been pressed

    void MouseMain();

  public:
    explicit RsMouse(const DeviceDb &deviceDb);

    ~RsMouse();

    void Register();

    Response FilterEvent(HidlInputEvent &iev, int32_t &deviceId);

    bool NotifyMotionState(int32_t deviceId, const AnalogCoords &pc, bool handled);
};

} // namespace inputhook

#endif // INPUTHOOK_RSMOUSE_H
