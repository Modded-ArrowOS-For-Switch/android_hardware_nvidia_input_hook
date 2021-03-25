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

#ifndef VENDOR_NVIDIA_HARDWARE_SHIELDTECH_INPUTFLINGER_V2_0_INPUTHOOK_H
#define VENDOR_NVIDIA_HARDWARE_SHIELDTECH_INPUTFLINGER_V2_0_INPUTHOOK_H

#include <optional>
#include "Common.h"
#include "RsMouse.h"
#include "DeviceDb.h"

namespace vendor {
namespace nvidia {
namespace hardware {
namespace shieldtech {
namespace inputflinger {
namespace V2_0 {
namespace implementation {

using namespace inputhook;


struct InputHook : public IInputHook {
    ::android::sp<IInputHookCallback> mInputHookCallback;
    DeviceDb mDeviceDb;
    RsMouse mRsMouse;

    InputHook();
    status_t registerAsSystemService();

    // Methods from ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::IInputHook follow.
    Return<bool> init(const ::android::sp<IInputHookCallback> &inputHookCallback) override;
    Return<void> filterNewDevice(const hidl_handle &fd, int32_t id, const hidl_string& path, const InputIdentifier &identifier, IInputHook::filterNewDevice_cb _hidl_cb) override;
    Return<void> filterCloseDevice(int32_t id) override;
    Return<void> filterEvent(const HidlInputEvent &iev, int32_t deviceId, IInputHook::filterEvent_cb _hidl_cb) override;
    Return<bool> notifyKeyState(int32_t deviceId, int32_t keyCode, bool handled) override;
    Return<bool> notifyMotionState(int32_t deviceId, const AnalogCoords &pc, bool handled) override;
    Return<void> registerDevices() override;
    Return<bool> treatMouseAsTouch() override;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace inputflinger
}  // namespace shieldtech
}  // namespace hardware
}  // namespace nvidia
}  // namespace vendor

#endif  // VENDOR_NVIDIA_HARDWARE_SHIELDTECH_INPUTFLINGER_V2_0_INPUTHOOK_H
