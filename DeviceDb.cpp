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

#include "RsMouse.h"
#include "DeviceDb.h"

namespace inputhook {

void DeviceDb::AddDevice(int32_t id, int32_t vid, int32_t pid) {
    deviceMap[id] = [vid, pid]() {
#define ID(vid, pid) ((static_cast<int64_t>(vid) << 32) | static_cast<int64_t>(pid))
        switch (ID(vid, pid)) {
            case ID(0, 0): // Blacklist internal devices
                return DeviceDescriptor{
                    .blacklisted = true
                };
            default:
                return DeviceDescriptor{};
        }
#undef ID
    }();
}

void DeviceDb::RemoveDevice(int32_t id) {
    deviceMap.erase(id);
}

DeviceDescriptor DeviceDb::at(int32_t id) const {
    if (deviceMap.count(id)) {
        return deviceMap.at(id);
    } else {
        return DeviceDescriptor{
            .blacklisted = true
        };
    }
}

} // namespace inputhook
