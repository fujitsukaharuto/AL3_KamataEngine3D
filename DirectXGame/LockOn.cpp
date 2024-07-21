#include "LockOn.h"
#include "Enemy.h"
#include "Input.h"
#include "TextureManager.h"
#include "MathCal.h"
#include "WinApp.h"

#include <cmath>

void LockOn::Initialize() {

	lockOnMarkHandle_ = TextureManager::Load("reticle.png");
	lockOnMark_.reset(Sprite::Create(lockOnMarkHandle_, {0.0f, 0.0f}));
	lockOnMark_->SetAnchorPoint({0.5f, 0.5f});
}

void LockOn::Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			prebutton_ = false;
		}
	}

	if (target_) {

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && (prebutton_ == false))
			{
				target_ = nullptr;
				prebutton_ = true;
			} else if (SearchAreaAngle(viewProjection)) {
				target_ = nullptr;
			}
		}

	} else {

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && (prebutton_ == false)) {
				Search(enemies, viewProjection);
				prebutton_ = true;
			}
		}
	}

	if (target_) {

		Vector3 positionWorld = target_->GetCenterWorldPosition();
		Vector3 positionScreen = WorldToScreen(positionWorld, viewProjection);
		Vector2 positionScreenV2(positionScreen.x, positionScreen.y);

		lockOnMark_->SetPosition(positionScreenV2);

	}
}

void LockOn::Draw() {

	if (target_ != nullptr) {
		lockOnMark_->Draw();
	}
}

void LockOn::Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection) {

	// 目標
	std::list<std::pair<float, const Enemy*>> targets;

	// すべての敵にロックオン判定
	for (const std::unique_ptr<Enemy>& enemy : enemies) {

		Vector3 positionWorld = enemy->GetCenterWorldPosition();

		Vector3 positionView = Transform(positionWorld, viewProjection.matView);

		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_)
		{
			// カメラ前方との角度計算
			float arcTangent = std::atan2(
				std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
				positionView.z);

			// 角度条件チェック(コーンに収まっているか)
			if (std::sqrt(arcTangent * arcTangent) <= angleRange_)
			{
				targets.emplace_back(std::make_pair(positionView.z, enemy.get()));
			}

			target_ = nullptr;
			if (targets.size() != 0) {
				targets.sort([](auto& pair1, auto& pair2) { return pair1.first < pair2.first; });
				target_ = targets.front().second;
			}

		}



	}

}

bool LockOn::SearchAreaAngle(const ViewProjection& viewProjection)
{
	
	Vector3 positionWorld = target_->GetCenterWorldPosition();
	Vector3 positionView = Transform(positionWorld, viewProjection.matView);

	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		
		float arcTangent = std::atan2(
			std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y),
			positionView.z);

		// 角度条件チェック(コーンに収まっているか)
		if (std::sqrt(arcTangent * arcTangent) <= angleRange_) {
			return false;
		}
	}

	return true;

}

Vector3 LockOn::WorldToScreen(Vector3 worldPosition, const ViewProjection& viewProjection) {

	Vector3 positionEnemy = worldPosition;
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewport = Multiply(viewProjection.matView, viewProjection.matProjection);
	matViewProjectionViewport = Multiply(matViewProjectionViewport, matViewport);

	positionEnemy = Transform(positionEnemy, matViewProjectionViewport);
	Vector3 screenEnemy = {float(positionEnemy.x), float(positionEnemy.y), 0.0f};

	return screenEnemy;
}
