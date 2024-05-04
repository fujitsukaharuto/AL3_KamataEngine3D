#include "Player.h"
#include "MathCal.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WinApp.h"
#include "PrimitiveDrawer.h"
#include <cassert>

Player::Player() {}

Player::~Player()
{

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	input_ = Input::GetInstance();
	SetAttribute(kCollisionAttributePlayer);
	SetMask((0xffffffff ^ (kCollisionAttributePlayer)));

	worldTransfoem3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {0, 0}, {1, 1, 1, 1}, {0.5f, 0.5f});

	controlPoints_ = {
	    {0,  0,  0},
        {10, 10, 0},
        {10, 15, 0},
        {20, 15, 0},
        {20, 0,  0},
        {30, 0,  0},
	};
}

void Player::Update(const ViewProjection& viewProjection) 
{
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});


	Rotate();
	
	Move();
	const float kMoveLimitX = 32.5f;
	const float kMoveLimitY = 18.5f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);
#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -40.5f, 40.5f);
	ImGui::End();

#endif // _DEBUG
	worldTransform_.UpdateMatrix();


	const float kDistancePlayerTo3DReticle = 50.0f;
	Vector3 offset = {0, 0, 1.0f};
	offset = Multiply(offset, worldTransform_.matWorld_);
	offset = offset.Normalize();
	offset = offset * kDistancePlayerTo3DReticle;
	Vector3 PosPlayer = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]};
	worldTransfoem3DReticle_.translation_ = PosPlayer + offset;
	worldTransfoem3DReticle_.UpdateMatrix();

	ReticleMouse(viewProjection);

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	model_->Draw(worldTransfoem3DReticle_, viewProjection);

}

void Player::Move()
{

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.5f;
	XINPUT_STATE joyState;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}
	worldTransform_.translation_ += move;

}

void Player::Rotate()
{
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack()
{
	XINPUT_STATE joyState;

	
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (input_->TriggerKey(DIK_SPACE)) {
			const float kBulletSpeed = 1.0f;
			Vector3 worldPosReticle = {worldTransfoem3DReticle_.matWorld_.m[3][0], worldTransfoem3DReticle_.matWorld_.m[3][1], worldTransfoem3DReticle_.matWorld_.m[3][2]};
			Vector3 worldPosPlayer = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]};
			Vector3 velocity = worldPosReticle - worldPosPlayer;
			velocity = velocity.Normalize();
			velocity = velocity * kBulletSpeed;

			PlayerBullet* newBullet = new PlayerBullet();
			Vector3 worldPos = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]};
			newBullet->Initialize(model_, worldPos, velocity);
			bullets_.push_back(newBullet);
		}
		return;
	}

	if (joyState.Gamepad.wButtons&XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		const float kBulletSpeed = 1.0f;
		Vector3 worldPosReticle = {worldTransfoem3DReticle_.matWorld_.m[3][0], worldTransfoem3DReticle_.matWorld_.m[3][1], worldTransfoem3DReticle_.matWorld_.m[3][2]};
		Vector3 worldPosPlayer = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]};
		Vector3 velocity = worldPosReticle - worldPosPlayer;
		velocity = velocity.Normalize();
		velocity = velocity * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 worldPos = {worldTransform_.matWorld_.m[3][0], worldTransform_.matWorld_.m[3][1], worldTransform_.matWorld_.m[3][2]};
		newBullet->Initialize(model_, worldPos, velocity);
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::SetParent(const WorldTransform* parent)
{

	worldTransform_.parent_ = parent;

}

void Player::DrawUI()
{

	sprite2DReticle_->Draw();

}

void Player::ReticleCal(const ViewProjection& viewProjection)
{

	Vector3 positionReticle = {worldTransfoem3DReticle_.matWorld_.m[3][0], worldTransfoem3DReticle_.matWorld_.m[3][1], worldTransfoem3DReticle_.matWorld_.m[3][2]};
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProJectionViewport = Multiply(viewProjection.matView, viewProjection.matProjection);
	matViewProJectionViewport = Multiply(matViewProJectionViewport, matViewport);

	positionReticle = Transform(positionReticle, matViewProJectionViewport);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

#ifdef _DEBUG

	ImGui::Begin("Text");
	ImGui::Text("Player");
	ImGui::Text("3Dreticle:(%+.2f,%+.2f,%+.2f)", worldTransfoem3DReticle_.translation_.x, worldTransfoem3DReticle_.translation_.y, worldTransfoem3DReticle_.translation_.z);
	ImGui::End();

#endif // _DEBUG
}

void Player::ReticleMouse(const ViewProjection& viewProjection) {
	
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	Vector2 spritePos = sprite2DReticle_->GetPosition();
	XINPUT_STATE joyState;

	sprite2DReticle_->SetPosition(Vector2(float(mousePosition.x), float(mousePosition.y)));
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePos.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 5.0f;
		spritePos.y += (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(spritePos);
	}

	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = Multiply(viewProjection.matView, viewProjection.matProjection);
	matVPV = Multiply(matVPV, matViewport);

	Matrix4x4 matInverseVPV = Inverse(matVPV);

	Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);

	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = mouseDirection.Normalize();

	const float kDistanceTestObject = 80.0f;
	worldTransfoem3DReticle_.translation_ = posNear + (mouseDirection * kDistanceTestObject);
	worldTransfoem3DReticle_.UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("Text");
	ImGui::Text("Player");
	ImGui::Text("2DReticle:(%f,%f)", mousePosition.x, mousePosition.y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3Dreticle:(%+.2f,%+.2f,%+.2f)", worldTransfoem3DReticle_.translation_.x, worldTransfoem3DReticle_.translation_.y, worldTransfoem3DReticle_.translation_.z);
	ImGui::End();

#endif // _DEBUG

}

void Player::LineDraw(const ViewProjection& viewProjection)
{
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection);
	std::vector<Vector3> pointsDrawing;
	const size_t segumentCount = 100;
	for (size_t i = 0; i < segumentCount+1; i++) {
		float t = 1.0f / segumentCount * i;
		Vector3 pos = CatmullRom(controlPoints_, t);
		pointsDrawing.push_back(pos);
	}

	for (int i = 0; i < pointsDrawing.size() - 2;) {
		int index1 = i;
		i++;
		int index2 = i;
		PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing[index1], pointsDrawing[index2], Vector4{1.0f, 0.0f, 0.0f, 1.0f});
	}

}
