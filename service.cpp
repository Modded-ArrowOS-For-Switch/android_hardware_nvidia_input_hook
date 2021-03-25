/*
 * Copyright (C) 2017 The Android Open Source Project
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
#include <hidl/HidlTransportSupport.h>
#include "InputHook.h"

using android::sp;
using android::status_t;
using android::OK;

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::implementation::InputHook;

int main() {

    status_t status;
    android::sp<InputHook> service = nullptr;

    ALOGI("Input Hook Service 2.0 for Nvidia is starting.");

    service = new InputHook();
    if (service == nullptr) {
        ALOGE("Can not create an instance of Input Hook Iface, exiting.");

        goto shutdown;
    }

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    status = service->registerAsSystemService();
    if (status != OK) {
        ALOGE("Could not register service for Input Hook Iface (%d).", status);
        goto shutdown;
    }

    ALOGI("Input Hook is ready");
    joinRpcThreadpool();
    //Should not pass this line

shutdown:
    // In normal operation, we don't expect the thread pool to exit

    ALOGE("Input Hook is shutting down");
    return 1;
}
