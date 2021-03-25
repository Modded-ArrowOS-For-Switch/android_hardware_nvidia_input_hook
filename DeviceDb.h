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

#ifndef INPUTHOOK_DEVICE_DB_H
#define INPUTHOOK_DEVICE_DB_H

#include <string_view>
#include <linux/input.h>
#include <unordered_map>
#include "Common.h"

namespace inputhook {

struct DeviceDescriptor {
    bool blacklisted{}; //!< If this device shouldn't be used for any input hooks
    int32_t rsMouseClickKeyCode{BTN_THUMBR}; //!< Keycode for the key to be used as RsMouse click
};

class DeviceDb {
  private:
    std::unordered_map<int32_t, DeviceDescriptor> deviceMap;

  public:
    void AddDevice(int32_t id, int32_t vid, int32_t pid);

    void RemoveDevice(int32_t id);

    DeviceDescriptor at(int32_t id) const;
};

} // namespace inputhook

#endif // INPUTHOOK_DEVICE_DB_H
