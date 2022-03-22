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
#include <OpenSpeed/Game.MW05/Types/UTL.h>

namespace OpenSpeed::MW05 {
  struct IHumanAI : UTL::COM::IUnknown {
    virtual ~IHumanAI();
    virtual void                  ChangeDragLanes(bool left);
    virtual bool                  IsPlayerSteering();
    virtual bool                  GetAiControl();
    virtual void                  SetAiControl(bool ai_control);
    virtual void                  SetWorldMoment(const UMath::Vector3& position, float radius);
    virtual const UMath::Vector3& GetWorldMomentPosition();
    virtual float                 GetWorldMomentRadius();
    virtual void                  ClearWorldMoment();
    virtual bool                  IsFacingWrongWay();

    static IHandle* GetIHandle() { return reinterpret_cast<IHandle*(__cdecl*)()>(0x403770)(); }
  };
}  // namespace OpenSpeed::MW05