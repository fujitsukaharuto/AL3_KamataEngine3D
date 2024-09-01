#include "Player.h"
#include "CollisionTypeIdDef.h"
#include "Effect.h"
#include "GlobalVariables.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "LockOn.h"
#include "MathCal.h"
#include "TextureManager.h"
#include "Audio.h"

#include <cassert>
#include <cmath>
#include <iostream>

enum PlayerModelIndex {
	kModelIndexBody = 0,
	kModelIndexHead = 1,
	kModelIndexL_arm = 2,
	kModelIndexR_arm = 3,
	kModelIndexWeapon = 4,
	kModelIndexBullet = 5,
};

const std::array<Player::ConstAttack, Player::ComboNum> Player::kConstAttacks_ = {
    {
     // 振りかぶり、攻撃前硬直、攻撃振り時間、硬直
        {0, 0, 40, 0, 0.0f, 0.0f, 0.15f},
     {15, 5, 35, 0, 0.2f, 0.0f, 0.0f},
     {0, 10, 35, 30, 0.2f, 0.0f, 0.0f},
     }
};

Player::Player() {}

Player::~Player() {

	for (SnowBall* ball : snowBalls_) {
		delete ball;
	}

	delete AbuttonSprite_;
	delete StickSprite_;
	delete RBButtonSprite_;
	delete YButtonSprite_;
	for (uint32_t i = 0; i < 4; i++) {
		delete hpSprite_[i];
	}

}

void Player::Initialize(const std::vector<Model*>& models) {

	BaseCharacter::Initialize(models);
	/*hammer_ = std::make_unique<Hammer>();
	hammer_->Initialize(models_[kModelIndexWeapon]);
	hammer_->UpdateWorldTransform();*/

	GlobalVariables* globalvariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalvariables->AddItem(groupName, "Test", int32_t(26));

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	/*hammer_->SetParent(worldTransformBody_);*/

	/*worldTransformHead_.translation_ = {0.0f, 1.65f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.59f, 1.65f, 0.0f};
	worldTransformR_arm_.translation_ = {0.59f, 1.65f, 0.0f};*/

	globalvariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalvariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalvariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	globalvariables->AddItem(groupName, "floatingCycle", cycle_);
	globalvariables->AddItem(groupName, "floatingAmplitude", floatingAmplitude_);

	ApplyGlobalVariables();

	InitializeFloatingGimmick();
	InitializeArmGimmick();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	/*hammer_->UpdateWorldTransform();*/

	AbuttonHandle_ = TextureManager::Load("AButton.png");
	StickHandle_ = TextureManager::Load("Stick.png");
	RBButtonHandle_ = TextureManager::Load("Contollor.png");
	YButtonHandle_ = TextureManager::Load("tutolialEnd.png");
	hpHandle_ = TextureManager::Load("hp.png");

	AbuttonSprite_ = Sprite::Create(AbuttonHandle_, {200.0f, 130.0f});
	StickSprite_ = Sprite::Create(StickHandle_, {700.0f, 130.0f});
	RBButtonSprite_ = Sprite::Create(RBButtonHandle_, {950.0f, 550.0f});
	YButtonSprite_ = Sprite::Create(YButtonHandle_, {950.f, 620.0f});
	for (uint32_t i = 0; i < lifeCount_; i++) {
		hpSprite_[i] = Sprite::Create(hpHandle_, {50.0f + static_cast<float>(80 * i), 620});
	}


	hitSound_ = Audio::GetInstance()->LoadWave("damaged1.mp3");
	DethSound_ = Audio::GetInstance()->LoadWave("powerdown.mp3");

}

void Player::SceneReset() {

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	behaviorRequest_ = Behavior::kRoot;

	for (SnowBall* ball : snowBalls_) {
		ball->SetDeath();
	}

	lifeCount_ = 4;
	invincibilityTime_ = 0;

	endPlayer_ = false;
	endTime_ = 0;

}

void Player::Update() {
	snowBalls_.remove_if([](SnowBall* ball) {
		if (ball->IsDead()) {
			delete ball;
			return true;
		}
		return false;
	});

	ApplyGlobalVariables();

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorChargeInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		case Behavior::kJump:
			BehaviorJumpInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}

	if (endTime_ == 0) {
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootUpdate();
			break;
		case Behavior::kAttack:
			BehaviorChargeUpdate();
			break;
		case Behavior::kDash:
			BehaviorDashUpdate();
			break;
		case Behavior::kJump:
			BehaviorJumpUpdate();
			break;
		}
	} else {
		endTime_--;
		if (endTime_ % 10 == 0) {
			Effect::GetInstance()->CreateEnemyDeth(worldTransform_.translation_);
		}
		if (endTime_ == 0) {
			endPlayer_ = true;
		}
	}

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (behavior_ != Behavior::kJump && behavior_ != Behavior::kAttack && invincibilityTime_ == 0) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				behaviorRequest_ = Behavior::kAttack;
				behaviorTimer_ = 60.0f;
			}
		}
	}
	if (behaviorTimer_ >= 0) {
		behaviorTimer_--;
	}

	for (SnowBall* ball : snowBalls_) {
		ball->Update();
	}

	if (invincibilityTime_ > 0) {
		invincibilityTime_--;

		float frequency = 0.3f;
		float alpha = (sin(static_cast<float>(invincibilityTime_) * frequency) + 1.0f) / 2.0f;
		models_[kModelIndexBody]->SetAlpha(alpha);
		models_[kModelIndexHead]->SetAlpha(alpha);
		models_[kModelIndexL_arm]->SetAlpha(alpha);
		models_[kModelIndexR_arm]->SetAlpha(alpha);
	} else {
	
		float alpha = 1.0f;

		models_[kModelIndexBody]->SetAlpha(alpha);
		models_[kModelIndexHead]->SetAlpha(alpha);
		models_[kModelIndexL_arm]->SetAlpha(alpha);
		models_[kModelIndexR_arm]->SetAlpha(alpha);
	}

}

void Player::Draw(const ViewProjection& viewProjection) {
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	for (SnowBall* ball : snowBalls_) {
		ball->Draw(viewProjection);
	}
}

void Player::DrawSprite() {

	RBButtonSprite_->Draw();

	for (uint32_t i = 0; i < lifeCount_; i++) {
		hpSprite_[i]->Draw();
	}
}

void Player::DrawTutolialSprite() {

	AbuttonSprite_->Draw();
	StickSprite_->Draw();
	YButtonSprite_->Draw();
}

void Player::BehaviorRootUpdate() {

	Move();
	UpdateArmGimmick();
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	/*hammer_->UpdateWorldTransform();*/

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			/*behaviorRequest_ = Behavior::kDash;*/
		}
		if (1) {
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				behaviorRequest_ = Behavior::kJump;
			}
		}
	}
}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
}

#pragma region コンボ

void Player::BehaviorAttackUpdate() {
#ifdef _DEBUG

	ImGui::Begin("PlayerAttack");
	ImGui::SliderFloat3("ArmL Translation", &worldTransformL_arm_.translation_.x, 1.0f, 1.0f);
	ImGui::SliderFloat3("ArmL Rotate", &worldTransformL_arm_.rotation_.x, -4.0f, 4.0f);
	ImGui::SliderFloat3("ArmR Translation", &worldTransformR_arm_.translation_.x, 1.0f, 1.0f);
	ImGui::SliderFloat3("ArmR Rotate", &worldTransformR_arm_.rotation_.x, -4.0f, 4.0f);
	ImGui::Text("attackSpeed:%f", attackSpeed_);
	ImGui::End();

#endif // _DEBUG

	if (lockOn_ && lockOn_->ExistTarget()) {
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();
		Vector3 sub = lockOnPosition - worldTransform_.translation_;

		float distance = sub.Lenght();
		const float threshold = 1.0f;

		if (distance > threshold) {
			worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);

			if (attackSpeed_ > distance - threshold) {
				attackSpeed_ = distance - threshold;
			}
		}
	}

	uint32_t totalTime = 0;
	uint32_t onePhaseTime = kConstAttacks_[workAttack_.comboIndex].anticipationTime;
	uint32_t twoPhaseTime = kConstAttacks_[workAttack_.comboIndex].chargeTime;
	uint32_t threePhaseTime = kConstAttacks_[workAttack_.comboIndex].swingTime;
	switch (workAttack_.comboIndex) {
	case 0:
		if (behaviorTimer_ > 45) {
			attackMove_ = {0.0f, 0.0f, attackSpeed_};
			Matrix4x4 rotateBody = MakeRotateXYZMatrix(worldTransform_.rotation_);
			attackMove_ = TransformNormal(attackMove_, rotateBody);

			worldTransform_.translation_ += attackMove_;
		}

		switch (workAttack_.inComboPhase) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			worldTransformBody_.rotation_.y = LerpShortAngle(worldTransformBody_.rotation_.y, 0.0f, 0.3f);
			break;
		case 3:
		default:
			break;
		}

		break;
	case 1:
		if (behaviorTimer_ > 45) {
			attackMove_ = {0.0f, 0.0f, attackSpeed_};
			Matrix4x4 rotateBody = MakeRotateXYZMatrix(worldTransform_.rotation_);
			attackMove_ = TransformNormal(attackMove_, rotateBody);

			worldTransform_.translation_ += attackMove_;
		}

		if ((workAttack_.attackParameter_ <= onePhaseTime)) {
			worldTransformR_arm_.rotation_.x = LerpShortAngle(worldTransformR_arm_.rotation_.x, -3.36f, 0.3f);
			worldTransformR_arm_.rotation_.y = LerpShortAngle(worldTransformR_arm_.rotation_.y, 0.0f, 0.3f);
			worldTransformR_arm_.rotation_.z = LerpShortAngle(worldTransformR_arm_.rotation_.z, 0.416f, 0.3f);

			worldTransformL_arm_.rotation_.x = LerpShortAngle(worldTransformL_arm_.rotation_.x, -3.36f, 0.3f);
			worldTransformL_arm_.rotation_.y = LerpShortAngle(worldTransformL_arm_.rotation_.y, 0.0f, 0.3f);
			worldTransformL_arm_.rotation_.z = LerpShortAngle(worldTransformL_arm_.rotation_.z, -0.416f, 0.3f);

			/*hammer_->SetRotaion({LerpShortAngle(hammer_->GetRotation().x, -0.2f, 0.3f), 0.0f, 0.0f});*/
			if (workAttack_.attackParameter_ == onePhaseTime) {
				workAttack_.inComboPhase += kConstAttacks_[workAttack_.comboIndex].chargeTime;
			}
		}
		if ((workAttack_.attackParameter_ <= (onePhaseTime + twoPhaseTime)) && (workAttack_.attackParameter_ > onePhaseTime)) {

			if (workAttack_.attackParameter_ == (onePhaseTime + twoPhaseTime)) {
				workAttack_.inComboPhase += kConstAttacks_[workAttack_.comboIndex].swingTime;
			}
		}
		if ((workAttack_.attackParameter_ <= (onePhaseTime + twoPhaseTime + threePhaseTime)) && (workAttack_.attackParameter_ > (onePhaseTime + twoPhaseTime))) {

			worldTransformR_arm_.rotation_.x = LerpShortAngle(worldTransformR_arm_.rotation_.x, -1.36f, 0.3f);
			worldTransformR_arm_.rotation_.y = LerpShortAngle(worldTransformR_arm_.rotation_.y, -0.533f, 0.3f);
			worldTransformR_arm_.rotation_.z = LerpShortAngle(worldTransformR_arm_.rotation_.z, 0.0f, 0.3f);

			worldTransformL_arm_.rotation_.x = LerpShortAngle(worldTransformL_arm_.rotation_.x, -1.36f, 0.3f);
			worldTransformL_arm_.rotation_.y = LerpShortAngle(worldTransformL_arm_.rotation_.y, 0.533f, 0.3f);
			worldTransformL_arm_.rotation_.z = LerpShortAngle(worldTransformL_arm_.rotation_.z, 0.0f, 0.3f);

			/*hammer_->SetRotaion({LerpShortAngle(hammer_->GetRotation().x, 1.5f, 0.3f), 0.0f, 0.0f});*/
			if (workAttack_.attackParameter_ == (onePhaseTime + twoPhaseTime + threePhaseTime)) {
				workAttack_.inComboPhase += kConstAttacks_[workAttack_.comboIndex].recoveryTime;
			}
		}
		break;
	case 2:
	default:
		if (behaviorTimer_ > 45) {
			attackMove_ = {0.0f, 0.0f, attackSpeed_};
			Matrix4x4 rotateBody = MakeRotateXYZMatrix(worldTransform_.rotation_);
			attackMove_ = TransformNormal(attackMove_, rotateBody);

			worldTransform_.translation_ += attackMove_;
		}

		if ((workAttack_.attackParameter_ <= onePhaseTime)) {

			if (workAttack_.attackParameter_ == onePhaseTime) {
				workAttack_.inComboPhase += kConstAttacks_[workAttack_.comboIndex].chargeTime;
			}
		}
		if ((workAttack_.attackParameter_ <= (onePhaseTime + twoPhaseTime)) && (workAttack_.attackParameter_ > onePhaseTime)) {

			if (workAttack_.attackParameter_ == (onePhaseTime + twoPhaseTime)) {
				workAttack_.inComboPhase += kConstAttacks_[workAttack_.comboIndex].swingTime;
			}
		}
		if ((workAttack_.attackParameter_ <= (onePhaseTime + twoPhaseTime + threePhaseTime)) && (workAttack_.attackParameter_ > (onePhaseTime + twoPhaseTime))) {

			if (worldTransformBody_.rotation_.y < 6.26573f) {
				worldTransformBody_.rotation_.y += 0.3f;
			}
			if (worldTransformBody_.rotation_.y > 6.26573f) {
				worldTransformBody_.rotation_.y = 6.26573f;
			}

			if (workAttack_.attackParameter_ == (onePhaseTime + twoPhaseTime + threePhaseTime)) {
				workAttack_.inComboPhase += kConstAttacks_[workAttack_.comboIndex].recoveryTime;
			}
		}
		break;
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	/*hammer_->UpdateWorldTransform();*/

	XINPUT_STATE joyStatePre;
	XINPUT_STATE joyState;
	if (workAttack_.comboIndex < (ComboNum - 1)) {

		if (Input::GetInstance()->GetJoystickState(0, joyState) && Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre)) {

			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && !(joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				workAttack_.comboNext = true;
			}
		}
	}

	totalTime = kConstAttacks_[workAttack_.comboIndex].anticipationTime + kConstAttacks_[workAttack_.comboIndex].chargeTime + kConstAttacks_[workAttack_.comboIndex].swingTime +
	            kConstAttacks_[workAttack_.comboIndex].recoveryTime;
	if (++workAttack_.attackParameter_ >= totalTime) {
		if (workAttack_.comboNext) {
			workAttack_.comboNext = false;

			workAttack_.attackParameter_ = 0;
			workAttack_.inComboPhase = 0;

			if (Input::GetInstance()->GetJoystickState(0, joyState)) {
				const float threshold = 0.7f;
				bool ismoving = false;

				velocity_ = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
				if (velocity_.Lenght() > threshold) {
					ismoving = true;
				}

				float targetRotate = 0;
				if (ismoving) {
					const float kCharacterSpeed = 0.3f;
					velocity_ = velocity_.Normalize() * kCharacterSpeed;
					Matrix4x4 rotateCamera = MakeRotateXYZMatrix(viewProjection_->rotation_);
					velocity_ = TransformNormal(velocity_, rotateCamera);

					targetRotate = std::atan2(velocity_.x, velocity_.z);
					destinationAngleY_ = targetRotate;
					worldTransform_.rotation_.y = destinationAngleY_;
				}
			}

			switch (workAttack_.comboIndex) {
			case 0:
				/*worldTransformL_arm_.rotation_ = {-3.36f, 0.0f, -0.416f};
				worldTransformR_arm_.rotation_ = {-3.36f, 0.0f, 0.416f};*/
				/*hammer_->SetRotaion({hammer_->GetRotation().x, 0.0f, 1.5708f});*/
				/*hammer_->DeletionContactHistory();*/
				attackSpeed_ = 0.2f;
				attackMove_ = {0.0f, 0.0f, attackSpeed_};
				workAttack_.comboIndex++;
				break;
			case 1:
			default:
				worldTransformBody_.rotation_.y = 0.0f;
				/*hammer_->DeletionContactHistory();*/
				attackSpeed_ = 0.2f;
				attackMove_ = {0.0f, 0.0f, attackSpeed_};
				workAttack_.comboIndex++;
				break;
			}

		} else {
			behaviorRequest_ = Behavior::kRoot;
			workAttack_.attackParameter_ = 0;
			workAttack_.comboIndex = 0;
			workAttack_.inComboPhase = 0;
			worldTransformBody_.rotation_.y = 0.0f;
		}
	}
}

void Player::BehaviorAttackInitialize() {

	worldTransformBody_.rotation_.y = 1.5708f;
	worldTransformL_arm_.rotation_ = {-1.36f, 0.533f, 0.0f};
	worldTransformR_arm_.rotation_ = {-1.36f, -0.533f, 0.0f};
	/*hammer_->SetRotaion({1.5f, 0.0f, 1.5708f});
	hammer_->DeletionContactHistory();*/
	workAttack_.inComboPhase = 2;
	attackSpeed_ = 0.2f;
	attackMove_ = {0.0f, 0.0f, attackSpeed_};
}
#pragma endregion

void Player::BehaviorDashUpdate() {

	const float kCharacterSpeed = 1.5f;
	Vector3 move = {0.0f, 0.0f, 1.0f};
	move = move.Normalize() * kCharacterSpeed;
	Matrix4x4 rotatePlayer = MakeRotateXYZMatrix(worldTransform_.rotation_);
	move = TransformNormal(move, rotatePlayer);
	worldTransform_.translation_ += move;

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	/*hammer_->UpdateWorldTransform();*/

	const uint32_t behaviorDashTime = 20;

	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorDashInitialize() {

	workDash_.dashParameter_ = 0;
	worldTransform_.rotation_.y = destinationAngleY_;
}

void Player::BehaviorJumpUpdate() {

	worldTransform_.translation_ += velocity_;
	const float kGravityAcceleration = 0.05f;
	Vector3 accelerationVector = {0.0f, -kGravityAcceleration, 0.0f};
	velocity_ += accelerationVector;

	if (worldTransform_.translation_.y <= 0.0f) {
		worldTransform_.translation_.y = 0.0f;
		behaviorRequest_ = Behavior::kRoot;
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	/*hammer_->UpdateWorldTransform();*/
}

void Player::BehaviorJumpInitialize() {
	worldTransform_.rotation_.y = destinationAngleY_;
	worldTransformBody_.translation_.y = 0;
	worldTransformL_arm_.rotation_.x = 0;
	worldTransformL_arm_.rotation_.z = 0;
	worldTransformR_arm_.rotation_.x = 0;
	worldTransformR_arm_.rotation_.z = 0;

	// ジャンプ初速度
	const float kJumpFirstSpeed = 0.75f;
	velocity_.y = kJumpFirstSpeed;
}

void Player::BehaviorChargeUpdate() {

	snowBalls_.remove_if([](SnowBall* ball) {
		if (ball->IsDead()) {
			delete ball;
			return true;
		}
		return false;
	});

	if (snowBalls_.size() == 0) {
		behaviorRequest_ = Behavior::kRoot;
		return;
	}
	if (snowBalls_.back()->IsRemove()) {
		behaviorRequest_ = Behavior::kRoot;
		return;
	}

	XINPUT_STATE joyStatePre;
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState) && Input::GetInstance()->GetJoystickStatePrevious(0, joyStatePre)) {
		if (!(joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (joyStatePre.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			behaviorRequest_ = Behavior::kRoot;
			if (!snowBalls_.back()->IsRemove()) {
				snowBalls_.back()->SetRemove(true);
				const float kBallSpeed = 0.5f;
				Vector3 vel = {0.0f, 0.0f, 1.0f};
				vel = vel.Normalize() * kBallSpeed;
				Matrix4x4 rotatePlayer = MakeRotateXYZMatrix(worldTransform_.rotation_);
				vel = TransformNormal(vel, rotatePlayer);
				snowBalls_.back()->SetVelocity(vel);
				snowBalls_.back()->SetRadius(snowBalls_.back()->GetRadius());
			}
		}
	}

	Move();
	if (!snowBalls_.back()->IsRemove()) {
		if (addSnowSize_) {
			float newRad = snowBalls_.back()->GetRadius();
			newRad += 0.01f;
			snowBalls_.back()->SetSizeRadius(newRad);
		}

		Vector3 oldpos = snowBalls_.back()->GetTrans();
		const float kCharacterDistance = 1.0f;
		Vector3 snowBallPos = {0.0f, 0.0f, 1.0f};
		snowBallPos = snowBallPos.Normalize() * kCharacterDistance;

		snowBallPos.z += snowBalls_.back()->GetRadius();
		Matrix4x4 rotatePlayer = MakeRotateXYZMatrix(worldTransform_.rotation_);
		snowBallPos = TransformNormal(snowBallPos, rotatePlayer);

		snowBallPos += worldTransform_.translation_;
		snowBallPos.y += snowBalls_.back()->GetRadius();
		snowBalls_.back()->SetTransform(snowBallPos);

		Vector3 Differential = snowBallPos - oldpos;
		Differential = Differential.Normalize();

		Vector3 forward = {0.0f, 1.0f, 0.0f};
		Vector3 rotateAxis = forward.Cross(Differential);
		if (rotateAxis.Lenght() > 0.0f) {

			rotateAxis = rotateAxis.Normalize();
			float rotateAngle = (Differential.Lenght() / snowBalls_.back()->GetRadius());

			Matrix4x4 rotateMat = MakeRotateAxisMatrix(rotateAxis, rotateAngle);

			Matrix4x4 currentTransform = snowBalls_.back()->GetWorldMat();

			currentTransform = Multiply(rotateMat, currentTransform);

			Vector3 newRotate = ExtractEulerAngles(currentTransform);

			snowBalls_.back()->SetRotate(newRotate);
		}
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::BehaviorChargeInitialize() {

	worldTransformL_arm_.rotation_ = {-1.36f, 0.533f, 0.0f};
	worldTransformR_arm_.rotation_ = {-1.36f, -0.533f, 0.0f};
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	SnowBall* newSnowBall = new SnowBall();

	const float kCharacterDistance = 1.0f;
	Vector3 snowBallPos = {0.0f, 0.0f, 1.0f};
	snowBallPos.z += newSnowBall->GetRadius();
	snowBallPos = snowBallPos.Normalize() * kCharacterDistance;
	Matrix4x4 rotatePlayer = MakeRotateXYZMatrix(worldTransform_.rotation_);
	snowBallPos = TransformNormal(snowBallPos, rotatePlayer);
	snowBallPos += worldTransform_.translation_;
	snowBallPos.y += newSnowBall->GetRadius();

	newSnowBall->Initialize(models_[kModelIndexBullet], snowBallPos);
	newSnowBall->SetLittleModel(littleModel_);
	snowBalls_.push_back(newSnowBall);
}

void Player::Move() {

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		const float threshold = 0.7f;
		bool ismoving = false;
		addSnowSize_ = false;

		velocity_ = {(float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0, (float)joyState.Gamepad.sThumbLY / SHRT_MAX};
		if (velocity_.Lenght() > threshold) {
			ismoving = true;
			addSnowSize_ = true;
		}

		float targetRotate = 0;
		destinationAngleY_ = targetRotate;
		if (ismoving) {
			const float kCharacterSpeed = 0.3f;
			weight = 1.0f;
			if (snowBalls_.size() != 0 && !(snowBalls_.back()->IsRemove())) {
				if (snowBalls_.back()->GetRadius() < 1.5f) {
					weight = 1.0f;
				}
				if (snowBalls_.back()->GetRadius() >= 1.5f) {
					float subNumber = 0.09f * snowBalls_.back()->GetRadius();
					weight = weight - subNumber;
				}
				if (weight < 0.1f) {
					weight = 0.1f;
				}
			}
			velocity_ = velocity_.Normalize() * kCharacterSpeed * weight;
			Matrix4x4 rotateCamera = MakeRotateXYZMatrix(viewProjection_->rotation_);
			velocity_ = TransformNormal(velocity_, rotateCamera);

			Effect::GetInstance()->CreateWalk(worldTransform_.translation_);

			worldTransform_.translation_ += velocity_;
			targetRotate = std::atan2(velocity_.x, velocity_.z);
			destinationAngleY_ = targetRotate;
			worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetRotate, 0.075f);

		}
		if (lockOn_ && lockOn_->ExistTarget()) {
			Vector3 lockOnPosition = lockOn_->GetTargetPosition();
			Vector3 sub = lockOnPosition - worldTransform_.translation_;

			worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
		}
	}
}

void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

void Player::UpdateFloatingGimmick() {
	float mpi = 3.14159265f;
	const float step = 2.0f * mpi / cycle_;
#ifdef _DEBUG

	int valueCycle = static_cast<int>(cycle_);

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

void Player::InitializeArmGimmick() { armParameter_ = 0.0f; }

void Player::UpdateArmGimmick() {
	float mpi = 3.14159265f;
	const float step = 2.0f * mpi / cycle_;
#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::SliderFloat("ArmAmplitude", &armAmplitude_, 0.01f, 2.0f);
	ImGui::End();

#endif // _DEBUG

	armParameter_ += step;
	armParameter_ = std::fmod(armParameter_, 2.0f * mpi);

	worldTransformL_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude_;
	worldTransformR_arm_.rotation_.x = std::sin(armParameter_) * armAmplitude_;
}

void Player::OnCollision([[maybe_unused]] Collider* other) {

	if (lifeCount_ > 0) {

		uint32_t typeID = other->GetTypeID();
		if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy) || typeID == static_cast<uint32_t>(CollisionTypeIdDef::kLittleEnemy)) {

			if (invincibilityTime_ == 0 && lifeCount_ > 0) {
				lifeCount_--;
				Audio::GetInstance()->PlayWave(hitSound_, false, 0.3f);
				if (lifeCount_ > 4) {
					lifeCount_ = 0;
				}

				invincibilityTime_ = 50;

				if (behavior_ == Behavior::kAttack) {
					if (!snowBalls_.size() == 0) {
						if (!snowBalls_.back()->IsRemove()) {
							snowBalls_.back()->SetRemove(true);
							const float kBallSpeed = 0.5f;
							Vector3 vel = {0.0f, 0.0f, 1.0f};
							vel = vel.Normalize() * kBallSpeed;
							Matrix4x4 rotatePlayer = MakeRotateXYZMatrix(worldTransform_.rotation_);
							vel = TransformNormal(vel, rotatePlayer);
							snowBalls_.back()->SetVelocity(vel);
						}
					}
				}
			}
		}

		if (lifeCount_ == 0) {

			endPlayer_ = false;
			endTime_ = 120;
			Audio::GetInstance()->PlayWave(DethSound_, false, 0.3f);
		}

	}

}

void Player::SetLockOn(const LockOn* target) { lockOn_ = target; }

Vector3 Player::GetCenterPosition() const {

	const Vector3 offset = {0.0f, 1.5f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

Vector3 Player::GetCenterPosisionOrigine() const {

	const Vector3 offset = {0.0f, 0.0f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

//Hammer* Player::GetWeaponCollider() { return hammer_.get(); }

std::list<SnowBall*> Player::GetBallCollider() { return snowBalls_; }

bool Player::GetIsAttack() const {

	if (behavior_ == Behavior::kAttack) {
		return true;
	}
	return false;
}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");
	cycle_ = uint16_t(globalVariables->GetIntValue(groupName, "floatingCycle"));
	floatingAmplitude_ = globalVariables->GetFloatValue(groupName, "floatingAmplitude");
}

void Player::SetLittleEnemy(std::vector<Model*> model) { littleModel_ = model; }
