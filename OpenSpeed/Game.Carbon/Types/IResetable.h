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
#include <OpenSpeed/Game.Carbon/Types.h>
#include <OpenSpeed/Game.Carbon/Types/UTL.h>

namespace OpenSpeed::Carbon {
  struct IResetable : UTL::COM::IUnknown {
    virtual ~IResetable();

    virtual bool HasResetPosition()                                                                = 0;
    virtual bool ResetVehicle(bool findRoad)                                                       = 0;
    virtual void SetResetPosition(const UMath::Vector3& position, const UMath::Vector3& direction) = 0;
    virtual void ClearResetPosition()                                                              = 0;
  };
}  // namespace OpenSpeed::Carbon
