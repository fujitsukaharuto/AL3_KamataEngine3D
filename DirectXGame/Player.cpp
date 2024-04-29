#include "Player.h"
#include "MathCal.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WinApp.h"
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
	
	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.5f;
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}
	const float kMoveLimitX = 32.5f;
	const float kMoveLimitY = 18.5f;
	worldTransform_.translation_ += move;

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

	ReticleCal(viewProjection);

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

void Player::Move() {}

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
}
