#include "Player.h"
#include "Input.h"
#include "MathCal.h"
#include "ImGuiManager.h"

#include <cmath>
#include <iostream>
#include <cassert>


enum PlayerModelIndex {
	kModelIndexBody = 0,
	kModelIndexHead = 1,
	kModelIndexL_arm = 2,
	kModelIndexR_arm = 3,
};


Player::Player() {}

Player::~Player() {}

void Player::Initialize(const std::vector<Model*>& models) {
	
	BaseCharacter::Initialize(models);

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	worldTransformHead_.translation_ = {0.0f, 1.65f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.59f, 1.65f, 0.0f};
	worldTransformR_arm_.translation_ = {0.59f, 1.65f, 0.0f};
	InitializeFloatingGimmick();
	InitializeArmGimmick();
}

void Player::Update()
{
	Move();
	UpdateFloatingGimmick();
	UpdateArmGimmick();
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection)
{
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);

}

void Player::Move() {

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float threshold = 0.7f;
		bool ismoving = false;

		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		if (move.Lenght() > threshold) {
			ismoving = true;
		}

		float targetRotate = 0;
		if (ismoving) {
			const float kCharacterSpeed = 0.3f;
			move = move.Normalize() * kCharacterSpeed;
			Matrix4x4 rotateCamera = MakeRotateXYZMatrix(viewProjection_->rotation_);
			move = TransformNormal(move, rotateCamera);

			worldTransform_.translation_ += move;
			targetRotate = std::atan2(move.x, move.z);
		}
		worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetRotate, 0.075f);
	}
}

void Player::InitializeFloatingGimmick()
{
	floatingParameter_ = 0.0f;


}

void Player::UpdateFloatingGimmick()
{
	float mpi = 3.14159265f;
	int valueCycle = static_cast<int>(cycle_);
	const float step = 2.0f * mpi / cycle_;
#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Head Translation", &worldTransformHead_.translation_.x, 1.0f, 1.0f);
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, 1.0f, 1.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, 1.0f, 1.0f);
	ImGui::SliderInt("Cycle", reinterpret_cast<int*>(&valueCycle), 10, 120);
	cycle_ = static_cast<uint16_t>(valueCycle);
	ImGui::SliderFloat("FloatingAmplitude", &floatingAmplitude_, 0.01f, 2.0f);
	ImGui::End();

#endif // _DEBUG
	floatingParameter_ += step;
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * mpi);

	
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * floatingAmplitude_;
	
}

void Player::InitializeArmGimmick()
{

	armParameter_ = 0.0f;

}

void Player::UpdateArmGimmick()
{
	float mpi = 3.14159265f;
	const float step = 2.0f * mpi / cycle_;
#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::SliderFloat("ArmAmplitude", &armAmplitude_, 0.01f, 2.0f);
	ImGui::End();

#endif // _DEBUG

	armParameter_ += step;
	armParameter_ = std::fmod(armParameter_, 2.0f * mpi);

	worldTransformL_arm_.rotation_.z = std::sin(armParameter_) * armAmplitude_;
	worldTransformR_arm_.rotation_.z = -(std::sin(armParameter_) * armAmplitude_);
	worldTransformL_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude_;
	worldTransformR_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude_;
}
