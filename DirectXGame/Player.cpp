#include "Player.h"
#include "Input.h"
#include "MathCal.h"


#include <cmath>
#include <iostream>
#include <cassert>

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position)
{
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;


	worldTransform_.Initialize();
	worldTransform_.translation_ = position;


}

void Player::Update()
{

	Move();
	worldTransform_.UpdateMatrix();
}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

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
