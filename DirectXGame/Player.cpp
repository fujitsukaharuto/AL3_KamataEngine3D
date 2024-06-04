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

		Vector3 move = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		const float kCharacterSpeed = 0.3f;

		move = move.Normalize() * kCharacterSpeed;
		
		Matrix4x4 rotateCamera = MakeRotateXYZMatrix(viewProjection_->rotation_);
		
		move = TransformNormal(move, rotateCamera);

		worldTransform_.translation_ += move;
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}
}
