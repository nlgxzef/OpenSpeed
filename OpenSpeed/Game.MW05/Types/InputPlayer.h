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
#include <OpenSpeed/Game.MW05/Types/IInputPlayer.h>
#include <OpenSpeed/Game.MW05/Types/PInput.h>

namespace OpenSpeed::MW05 {
  struct InputPlayer : PInput, IInputPlayer {
    virtual ~InputPlayer();
    virtual bool OnAction(ActionRef action);

#pragma region overides
    virtual void Reset() override;
    virtual void OnTaskSimulate(float deltaTime);
    virtual void OnBehaviorChange(const UCrc32& mechanic);
    virtual bool IsLookBackButtonPressed() override;
    virtual bool IsPullBackButtonPressed() override;
    virtual bool IsAutomaticShift() override;
    virtual void BlockInput(bool block) override;
    virtual bool IsBlocked() override;
    virtual void FetchInput() override;
    virtual void ClearInput() override;
#pragma endregion
  };
}  // namespace OpenSpeed::MW05
