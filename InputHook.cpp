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

#define LOG_TAG "vendor.nvidia.hardware.shieldtech.inputflinger@2.0-service"

// #define LOG_NDEBUG 0

#include <android/log.h>
#include <log/log.h>
#include "InputHook.h"

namespace vendor {
namespace nvidia {
namespace hardware {
namespace shieldtech {
namespace inputflinger {
namespace V2_0 {
namespace implementation {

InputHook::InputHook() : mRsMouse(mDeviceDb) {}

status_t InputHook::registerAsSystemService() {
    status_t ret{IInputHook::registerAsService()};
    if (ret != 0) {
        ALOGE("Failed to register InputHook (%d)", ret);
        goto fail;
    } else {
        ALOGI("Successfully registered InputHook");
    }

fail:
    return ret;
}

// Methods from ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::IInputHook follow.
Return<bool> InputHook::init(const ::android::sp<IInputHookCallback> &inputHookCallback) {
    ALOGI("InputHook::init");

    if (!inputHookCallback)
        return false;

    mInputHookCallback = inputHookCallback;

    return true;
}

Return<void> InputHook::filterNewDevice(const hidl_handle& fd, int32_t id, const hidl_string& path, const InputIdentifier& identifier, IInputHook::filterNewDevice_cb _hidl_cb) {
    if (fd->numFds <= 0) {
		_hidl_cb(false, identifier.name);
		return Void();
	}

    ALOGI("InputHook::filterNewDevice: fd: %d, id: %d, path: %s, identifier: { vendor: %x product: %x name: %s uniqueId: %s }", fd->data[0], id, path.c_str(), identifier.vendor, identifier.product, identifier.name.c_str(), identifier.uniqueId.c_str());

    mDeviceDb.AddDevice(id, identifier.vendor, identifier.product);

    _hidl_cb(true, identifier.name);

    return Void();
}

Return<void> InputHook::filterCloseDevice(int32_t id) {
    ALOGI("InputHook::filterCloseDevice: id: %d", id);

    mDeviceDb.RemoveDevice(id);

    return Void();
}

Return<void> InputHook::filterEvent(const HidlInputEvent& iev, int32_t deviceId, IInputHook::filterEvent_cb _hidl_cb) {
    auto filterIev{iev};
    auto response{Response::EVENT_DEFAULT};

    if (!mDeviceDb.at(deviceId).blacklisted) {
        response = mRsMouse.FilterEvent(filterIev, deviceId);
    }

    _hidl_cb(response, deviceId, filterIev);

    return Void();
};

Return<bool> InputHook::notifyKeyState(int32_t deviceId, int32_t keyCode, bool handled) {
    return false;
}


Return<bool> InputHook::notifyMotionState(int32_t deviceId, const AnalogCoords &pc, bool handled) {
    if (mDeviceDb.at(deviceId).blacklisted)
        return false;

    return mRsMouse.NotifyMotionState(deviceId, pc, handled);
}

Return<void> InputHook::registerDevices() {
    ALOGI("InputHook::registerDevices");

    mRsMouse.Register();

    return Void();
}

Return<bool> InputHook::treatMouseAsTouch() {
    return false;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace inputflinger
}  // namespace shieldtech
}  // namespace hardware
}  // namespace nvidia
}  // namespace vendor
