#include "Player.h"
#include "MathCal.h"
#include "ImGuiManager.h"
#include <cassert>

Player::Player() {}

Player::~Player()
{

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

}

void Player::Initialize(Model* model, uint32_t textureHandle)
{ 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() 
{

	Rotate();

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;
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
	ImGui::SliderFloat3("Player", &worldTransform_.translation_.x, -32.5f, 32.5f);
	ImGui::End();

#endif // _DEBUG
	worldTransform_.UpdateMatrix();

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

}

void Player::Draw(ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

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
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);
		bullets_.push_back(newBullet);
	}
}
