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
#include <OpenSpeed/Game.MW05/Types/IRBVehicle.h>
#include <OpenSpeed/Game.MW05/Types/RigidBody.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/Gen/collisionreactions.h>

namespace OpenSpeed::MW05 {
  struct RBVehicle : RigidBody, IRBVehicle {
    IVehicle*                                     mVehicle;
    ISuspension*                                  mSuspension;
    BehaviorSpecsPtr<Attrib::Gen::rigidbodyspecs> mSpecs;
    float                                         mDeadOnWheels;
    CollisionGeometry::Collection*                mGeoms;
    std::uint32_t                                 mFrame;
    UMath::Vector3                                mCollisionCenterOfGravity;
    float                                         mCollisionMass;
    bool                                          mObjectCollisionsEnabled;
    eInvulnerability                              mInvulnerableState;
    float                                         mInvulnerableTimer;
    float                                         mLastPenetration;
    Attrib::Gen::collisionreactions               mReactions;

    virtual ~RBVehicle();
  };
}  // namespace OpenSpeed::MW05
