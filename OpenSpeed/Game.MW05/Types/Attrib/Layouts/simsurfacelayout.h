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
#include <OpenSpeed/Game.MW05/Types/Attrib/Private.h>
#include <OpenSpeed/Game.MW05/Types/Attrib/RefSpec.h>

namespace OpenSpeed::MW05::Attrib::Layouts {
  struct simsurfacelayout {
    struct RoadNoiseRecord {
      float Frequency;
      float Amplitude;
      float MinSpeed;
      float MaxSpeed;
    };
    struct TireEffectRecord {
      RefSpec mEmitter;
      float   mMinSpeed;
      float   mMaxSpeed;
    };

    Private          _Array_TireDriveEffects;
    TireEffectRecord TireDriveEffects[3];
    Private          _Array_TireSlideEffects;
    TireEffectRecord TireSlideEffects[3];
    Private          _Array_TireSlipEffects;
    TireEffectRecord TireSlipEffects[3];
    RoadNoiseRecord  RenderNoise;
    const char*      CollectionName;
    float            GROUND_FRICTION;
    float            ROLLING_RESISTANCE;
    float            WORLD_FRICTION;
    float            DRIVE_GRIP;
    float            LATERAL_GRIP;
    float            STICK;
    std::uint16_t    WheelEffectFrequency;
    std::uint8_t     WheelEffectIntensity;
  };
}  // namespace OpenSpeed::MW05::Attrib::Layouts
