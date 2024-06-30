#include "FollowCamera.h"
#include "Input.h"
#include "MathCal.h"

FollowCamera::FollowCamera() {}

FollowCamera::~FollowCamera() {}

void FollowCamera::Initialize()
{

	viewProject_.Initialize();

}

void FollowCamera::Update()
{
	

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float kRotateSpeed = 0.05f;

		destinationAngleY_ += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			destinationAngleY_ = target_->rotation_.y;
		}
	}

	viewProject_.rotation_.y = LerpShortAngle(viewProject_.rotation_.y, destinationAngleY_, 0.4f);

	if (target_) {
		interTarget_ = Leap(interTarget_, target_->translation_, 0.3f);
	}
	Vector3 offset = OffsetCal();
	
	viewProject_.translation_ = interTarget_ + offset;

	viewProject_.UpdateMatrix();
}

void FollowCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

void FollowCamera::Reset()
{
	if (target_) {
		interTarget_ = target_->translation_;
		viewProject_.rotation_.y = target_->rotation_.y;
	}
	destinationAngleY_ = viewProject_.rotation_.y;

	Vector3 offset = OffsetCal();
	viewProject_.translation_ = interTarget_ + offset;
}

Vector3 FollowCamera::OffsetCal() const {

	Vector3 offset = {0.0f, 2.0f, -10.0f};

	Matrix4x4 rotateCamera = MakeRotateXYZMatrix(viewProject_.rotation_);
	offset = TransformNormal(offset, rotateCamera);

	return offset;
}
