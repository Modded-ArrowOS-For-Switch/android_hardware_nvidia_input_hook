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

#ifndef INPUTHOOK_COMMON_H
#define INPUTHOOK_COMMON_H

#include <vendor/nvidia/hardware/shieldtech/inputflinger/2.0/IInputHook.h>
#include <vendor/nvidia/hardware/shieldtech/inputflinger/2.0/IInputHookCallback.h>

namespace inputhook {

using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::IInputHook;
using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::IInputHookCallback;
using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::HidlInputEvent;
using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::AnalogCoords;
using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::InputIdentifier;
using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::Response;
using ::vendor::nvidia::hardware::shieldtech::inputflinger::V2_0::NewDevice;
using ::android::hardware::hidl_handle;
using ::android::hardware::hidl_string;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::status_t;

}

#endif // INPUTHOOK_COMMON_H
