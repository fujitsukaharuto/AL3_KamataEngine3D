#include "RailCamera.h"
#include "imgui.h"
#include "MathCal.h"


RailCamera::RailCamera() {}

RailCamera::~RailCamera() {}

void RailCamera::Initialize(Vector3 world, Vector3 rotate)
{

	worldTransform_.translation_ = world;
	worldTransform_.rotation_ = rotate;

	viewProjection_.Initialize();

	controlPoints_ = {
	    {0,   0,   0  },
        {100, 100, 80 },
        {100, 150, 100},
        {200, 150, 120},
        {200, 0,   140},
        {250, 0,   100},
	};

}

void RailCamera::Update()
{
	//float kSpeed = 0.2f;
	//float kRotateSpedd = 0.002f;
	//worldTransform_.translation_.z -= kSpeed;
	//worldTransform_.rotation_.y += kRotateSpedd;

	const float timelimitt = 2500.0f;
	if (time_<timelimitt) {
		time_++;
	}
	float t = 1.0f / timelimitt * time_;
	Vector3 eye = CatmullRom(controlPoints_, t);
	float t2 = 1.0f / timelimitt * (time_ + 50);
	Vector3 target = CatmullRom(controlPoints_, t2);

	worldTransform_.translation_ = eye;
	Vector3 forward = target - eye;
	forward = forward.Normalize();

	worldTransform_.rotation_.z = std::atan2(-forward.y, forward.x);
	Matrix4x4 zrota = MakeRotateZMatrix(-worldTransform_.rotation_.z);
	Vector3 forwardZ = TransformNormal(forward, zrota);

	worldTransform_.rotation_.y = std::atan2(forwardZ.x, forwardZ.z);
	Matrix4x4 yrota = MakeRotateYMatrix(-worldTransform_.rotation_.y);
	forwardZ = TransformNormal(forwardZ, yrota);

	worldTransform_.rotation_.x = std::atan2(-forwardZ.y, forwardZ.z);


#ifdef _DEBUG

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("trans", &worldTransform_.translation_.x, -300.0f, 300.0f);
	ImGui::SliderFloat3("rotate", &worldTransform_.rotation_.x, -1.0f, 1.0f);
	ImGui::End();

#endif // _DEBUG

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

}
