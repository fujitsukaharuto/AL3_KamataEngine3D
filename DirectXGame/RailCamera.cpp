#include "RailCamera.h"
#include "imgui.h"
#include "MathCal.h"


RailCamera::RailCamera() {}

RailCamera::~RailCamera() {}

void RailCamera::Initialize(Vector3 world, Vector3 rotate)
{

	/*worldTransform_.translation_ = world;
	worldTransform_.rotation_ = rotate;*/

	viewProjection_.Initialize();

}

void RailCamera::Update()
{
	float kSpeed = 0.2f;
	float kRotateSpedd = 0.002f;
	worldTransform_.translation_.z -= kSpeed;
	worldTransform_.rotation_.y += kRotateSpedd;
#ifdef _DEBUG

	ImGui::Begin("Camera");
	ImGui::SliderFloat3("trans", &worldTransform_.translation_.x, -300.0f, 300.0f);
	ImGui::SliderFloat3("rotate", &worldTransform_.rotation_.x, -1.0f, 1.0f);
	ImGui::End();

#endif // _DEBUG

	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

}
