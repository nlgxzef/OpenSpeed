// clang-format off
//
//    Copyright (C) 2022 Berkay Yigit <berkaytgy@gmail.com>
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Affero General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU Affero General Public License for more details.
//
//    You should have received a copy of the GNU Affero General Public License
//    along with this program. If not, see <https://www.gnu.org/licenses/>.
//
// clang-format on

#pragma once
#include <OpenSpeed/Game.MW05/Types.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Class.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Collection.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Database.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Definition.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/HashMap.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Instance.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Private.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/RefSpec.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/RGBA.h>

namespace OpenSpeed::MW05::Attrib {
  static inline StringKey StringToKey(const char* name) {
    return reinterpret_cast<StringKey(__cdecl*)(const char*)>(0x454640)(name);
  }

  static inline Collection* FindCollection(StringKey classKey, StringKey collectionKey) {
    return reinterpret_cast<Collection*(__cdecl*)(StringKey, StringKey)>(0x455FD0)(classKey, collectionKey);
  }
  static inline Collection* FindCollection(const char* className, StringKey collectionKey) {
    return FindCollection(StringToKey(className), collectionKey);
  }
  static inline Collection* FindCollection(const char* className, const char* collectionName) {
    return FindCollection(StringToKey(className), StringToKey(collectionName));
  }
}  // namespace OpenSpeed::MW05::Attrib
