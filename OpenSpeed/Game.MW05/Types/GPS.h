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
#include <OpenSpeed/Game.MW05/Types/Sim.h>
#include <OpenSpeed/Game.MW05/Types/WRoadNav.h>

namespace OpenSpeed::MW05 {
  struct GPS : Sim::Activity {
    UMath::Vector3 mTarget;
    HSIMTASK__*    mTask;
    UMath::Vector3 mPosition;
    eModel*        mArrowModel;
    UMath::Vector3 mDirection;
    eGPSState      mState;
    WRoadNav       mRoadNav;
    float          mPathDist;
    bool           mDrawn;
    float          mAngle;
    float          mScale;
    float          mMaxDeviation;
    float          mDeviation;

    static inline GPS** g_mThis = reinterpret_cast<GPS**>(0x90D8E4);
    static GPS*         Get() {
      if (!g_mThis) return nullptr;
      return *g_mThis;
    }
  };
}  // namespace OpenSpeed::MW05
