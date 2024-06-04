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
	viewProject_.UpdateMatrix();

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float kRotateSpeed = 0.05f;

		viewProject_.rotation_.y += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * kRotateSpeed;
	}

	if (target_) {
		Vector3 offset = {0.0f, 2.0f, -10.0f};
		
		Matrix4x4 rotateCamera = MakeRotateXYZMatrix(viewProject_.rotation_);
		offset = TransformNormal(offset, rotateCamera);

		viewProject_.translation_ = target_->translation_ + offset;
	}

	
	viewProject_.TransferMatrix();
}
