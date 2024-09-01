#include "Enemy.h"
#include "ImGuiManager.h"
#include "MathCal.h"
#include "CollisionTypeIdDef.h"
#include "TextureManager.h"

enum EnemyModelIndex {
	kModelIndexBody = 0,
	kModelIndexHead = 1,
	kModelIndexL_arm = 2,
	kModelIndexR_arm = 3,
	kModelIndexWeapon = 4,
};

uint32_t Enemy::nextNerialNumber_ = 0;

Enemy::Enemy() {

	serialNumber_ = nextNerialNumber_;
	++nextNerialNumber_;
}

Enemy::~Enemy() {

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		delete littleEnemy;
	}

	for (EnemyAttackZone* attackZone : attackZones_) {
		delete attackZone;
	}
	for (StandbyOperation* standby : standbies_) {
		delete standby;
	}

	delete hpSprite_;

}

void Enemy::Initialize(const std::vector<Model*>& models)
{

	BaseCharacter::Initialize(models);
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformL_arm_.Initialize();
	worldTransformR_arm_.Initialize();
	worldTransformWeapon_.Initialize();

	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformWeapon_.parent_ = &worldTransformL_arm_;

	worldTransform_.translation_ = {0.0f, 0.0f, 15.0f};
	worldTransformBody_.translation_ = {0.0f, -0.1f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 2.5f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.734f, 1.565f, 0.0f};
	worldTransformR_arm_.translation_ = {0.734f, 1.565f, 0.0f};
	worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};

	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));

	for (int i = 0; i < 8; i++) {
		StandbyOperation* newstandby = new StandbyOperation();

		newstandby->Initialize({0.0f, 0.0f, 0.0f}, 1);
		newstandby->TypeInitialize(0);
		standbies_.push_back(newstandby);
	}
	StandbyOperation* newstandby = new StandbyOperation();
	newstandby->Initialize({0.0f, 0.0f, 0.0f}, 1);
	newstandby->TypeInitialize(1);
	standbies_.push_back(newstandby);

	attackTypeRequest_ = AttackType::kDefault;

	hpTexture_ = TextureManager::Load("white1x1.png");
	hpSprite_ = Sprite::Create(hpTexture_, {320.0f, 50.0f}, {1.0f, 0.0f, 0.0f, 1.0f});
	hpSprite_->SetSize({640.0f, 15.0f});

}

void Enemy::SceneReset() {

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->SetDeath();
	}
	for (EnemyAttackZone* attackZone : attackZones_) {
		attackZone->SetDisappear();
	}

	lifeCount_ = 400;
	isStandby_ = false;
	attackCooltime_ = 300;
	attackType_ = AttackType::kSummon;
	attackTypeRequest_ = AttackType::kSummon;

	worldTransform_.translation_ = {0.0f, 0.0f, 15.0f};
	worldTransformBody_.translation_ = {0.0f, -0.1f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 2.5f, 0.0f};
	worldTransformL_arm_.translation_ = {-0.734f, 1.565f, 0.0f};
	worldTransformR_arm_.translation_ = {0.734f, 1.565f, 0.0f};
	worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};

	playerPos_ = {0.0f, 0.0f, 0.0f};
	oldPlayerPos_ = playerPos_;
	occurrenceTime_ = 60;
	occurrencesCount_ = 0;

}

void Enemy::Update()
{

	littleEnemys_.remove_if([](LittleEnemy* littleE) {
		if (littleE->IsDead()) {
			delete littleE;
			return true;
		}
		return false;
	});

	attackZones_.remove_if([](EnemyAttackZone* zone) {
		if (zone->IsDisappear()) {
			delete zone;
			return true;
		}
		return false;
	});

	if (attackCooltime_ == 10) {
		if (attackTypeRequest_) {
			attackType_ = attackTypeRequest_.value();
			switch (attackType_) {
			case AttackType::kDefault:

				worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};
				worldTransformWeapon_.rotation_ = {0.0f, 0.0f, 0.0f};

				worldTransformL_arm_.rotation_ = {-2.620f, 0.0f, -0.560f};
				worldTransformWeapon_.rotation_ = {0.9f, -0.9f, 0.0f};
				break;
			case AttackType::kFlattery:

				worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};
				worldTransformWeapon_.rotation_ = {0.0f, 0.0f, 0.0f};

				break;
			case AttackType::kSummon:

				worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};
				worldTransformWeapon_.rotation_ = {0.0f, 0.0f, 0.0f};

				worldTransformL_arm_.rotation_ = {-2.120f, 1.11f, -0.360f};
				worldTransformR_arm_.rotation_ = {-2.120f, -1.11f, 0.360f};
				worldTransformWeapon_.translation_ = {-1.02f, -1.37f, 0.63f};
				worldTransformWeapon_.rotation_ = {-0.26f, -0.17f, 1.81f};
				break;
			case AttackType::kDush:

				worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};
				worldTransformWeapon_.rotation_ = {0.0f, 0.0f, 0.0f};

				worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
				worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};
				worldTransformWeapon_.rotation_ = {0.0f, 0.0f, 0.0f};
				break;
			}
			attackTypeRequest_ = std::nullopt;
		}
	} else if (attackCooltime_ > 10) {
		worldTransformL_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
		worldTransformR_arm_.rotation_ = {0.0f, 0.0f, 0.0f};
		worldTransformWeapon_.translation_ = {-0.9f, -0.66f, 0.0f};
		worldTransformWeapon_.rotation_ = {0.0f, 0.0f, 0.0f};
	}

#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Enemy Body", &worldTransformBody_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy L_arm", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy Rotate L_arm", &worldTransformL_arm_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Enemy R_arm", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy Rotate R_arm", &worldTransformR_arm_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Enemy Head", &worldTransformHead_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy Weapon", &worldTransformWeapon_.translation_.x, 0.01f);
	ImGui::DragFloat3("Enemy Rotate Weapon", &worldTransformWeapon_.rotation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

	//Move();
	/*UpdatePartsGimmick();*/



	Attack();

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->Update();
	}

	for (EnemyAttackZone* attackZone : attackZones_) {
		attackZone->Update();
	}

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{

	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexHead]->Draw(worldTransformHead_, viewProjection);
	models_[kModelIndexL_arm]->Draw(worldTransformL_arm_, viewProjection);
	models_[kModelIndexR_arm]->Draw(worldTransformR_arm_, viewProjection);
	models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);

	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->Draw(viewProjection);
	}
	for (EnemyAttackZone* attackZone : attackZones_) {
		attackZone->Draw(viewProjection);
	}
	if (isStandby_) {
		if (attackType_ == AttackType::kDefault) {
			for (StandbyOperation* standby : standbies_) {
				if (standby->GetType() == 0) {
					standby->Draw(viewProjection);
				}
			}
		}
		if (attackType_ == AttackType::kFlattery) {
			for (StandbyOperation* standby : standbies_) {
				if (standby->GetType() == 1) {
					standby->Draw(viewProjection);
				}
			}
		}
	}
}

void Enemy::DrawSprite() {

	hpSprite_->Draw();

}

void Enemy::Move()
{
	float mpi = 3.14159265f;
	float kCharacterSpeed = 0.05f;
	Vector3 velocity = {0.0f, 0.0f, -kCharacterSpeed};
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	worldTransform_.translation_ += velocity;
	worldTransform_.rotation_.y += 0.01f;
	worldTransform_.rotation_.y = std::fmod(worldTransform_.rotation_.y, 2.0f * mpi);

}

void Enemy::UpdatePartsGimmick()
{
	float mpi = 3.14159265f;
	float kPartsRotateSpeed = 0.2f;
	worldTransformL_arm_.rotation_.x += -kPartsRotateSpeed;
	worldTransformR_arm_.rotation_.x += -kPartsRotateSpeed;
	worldTransformL_arm_.rotation_.x = std::fmod(worldTransformL_arm_.rotation_.x, 2.0f * mpi);
	worldTransformR_arm_.rotation_.x = std::fmod(worldTransformR_arm_.rotation_.x, 2.0f * mpi);
}

void Enemy::GraspVision() {

	if (attackType_ != AttackType::kDush) {
		Vector3 lockOnPosition = playerPos_;
		Vector3 sub = lockOnPosition - worldTransform_.translation_;

		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
	}

}

void Enemy::Attack() {

	if (attackCooltime_ == 0) {
		switch (attackType_) {
		case AttackType::kFlattery:

			if (occurrenceTime_ == 20) {
				oldPlayerPos_ = playerPos_;
			}
			if (occurrenceTime_ == 0) {
				EnemyAttackZone* newAttackZone = new EnemyAttackZone();
				oldPlayerPos_.y = 0.0f;
				Vector3 newPos = oldPlayerPos_;

				newAttackZone->Initialize(models_[kModelIndexWeapon], newPos);
				attackZones_.push_back(newAttackZone);
				occurrenceTime_ = 20;
				occurrencesCount_++;
			} else {
				occurrenceTime_--;
			}

			if (occurrencesCount_ == 10) {
				isStandby_ = false;
				attackTypeRequest_ = AttackType::kDush;
				occurrencesCount_ = 0;
				occurrenceTime_ = 30;
				attackCooltime_ = 300;
			} else {
				isStandby_ = true;
				playerPos_.y += 0.02f;
				for (StandbyOperation* standby : standbies_) {
					standby->SetTransform(playerPos_);
					standby->Update();
				}
			}

			break;
		case AttackType::kSummon:

			SettingLittles();
			if (occurrenceTime_ == 0) {
				attackTypeRequest_ = AttackType::kDefault;
				occurrenceTime_ = 60;
				attackCooltime_ = 300;
			} else {
				occurrenceTime_--;
			}

			break;
		case AttackType::kDush:

			if (occurrenceTime_ == 30) {
				oldPlayerPos_ = playerPos_;
				oldPlayerPos_.y = 0.0f;
				Vector3 sub = oldPlayerPos_ - worldTransform_.translation_;
				sub = sub * 0.6f;
				oldPlayerPos_ = oldPlayerPos_ + sub;
			}
			if (occurrenceTime_ == 0) {
				Vector3 newPos = oldPlayerPos_;

				worldTransform_.translation_ = Leap(worldTransform_.translation_, newPos, 0.1f);

				float worldtoNewPosSubLength = std::abs(newPos.Lenght() - worldTransform_.translation_.Lenght());

				if (worldtoNewPosSubLength < 0.5f) {
					occurrencesCount_++;
					occurrenceTime_ = 50;
				}
			} else {
				occurrenceTime_--;
			}

			if (occurrencesCount_ == 2) {
				attackTypeRequest_ = AttackType::kSummon;
				occurrencesCount_ = 0;
				occurrenceTime_ = 60;
				attackCooltime_ = 300;
			}

			break;
		case AttackType::kDefault:
		default:

			if (occurrenceTime_ == 0) {

				float interval = 6.0f;
				for (int i = 0; i < 7; i++) {
					float zoneRotate = 0.0f;

					for (int j = 0; j < 8; j++) {

						EnemyAttackZone* newAttackZone = new EnemyAttackZone();
						Vector3 newPos = {0.0f, 0.0f, interval};

						Matrix4x4 rotate = MakeRotateYMatrix(zoneRotate);
						newPos = TransformNormal(newPos, rotate);
						newPos += worldTransform_.translation_;
						zoneRotate += 0.785398f;

						newAttackZone->Initialize(models_[kModelIndexWeapon], newPos);
						attackZones_.push_back(newAttackZone);
					}
					interval += 6.0f;
				}
				attackTypeRequest_ = AttackType::kFlattery;
				isStandby_ = false;
				occurrenceTime_ = 30;
				attackCooltime_ = 300;
			} else {
				isStandby_ = true;
				occurrenceTime_--;
				float zoneRotate = 0.0f;
				for (StandbyOperation* standby : standbies_) {
					Vector3 newPos = {0.0f, 0.1f, 21.0f};
					Matrix4x4 rotate = MakeRotateYMatrix(zoneRotate);
					newPos = TransformNormal(newPos, rotate);
					newPos += worldTransform_.translation_;
					standby->SetRotate({0.0f, zoneRotate, 0.0f});
					zoneRotate += 0.785398f;
					standby->SetTransform(newPos);
					standby->Update();
				}
			}
			break;
		}
	} else {
		attackCooltime_--;
	}

}

void Enemy::OnCollision([[maybe_unused]] Collider* other) {

	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kSnowBall)) {
		if (other->GetRadius() <= 0.8f) {

			lifeCount_ -= 1;

			if (lifeCount_ > 400) {
				lifeCount_ = 0;
			}

			float newSize = 640.0f * (lifeCount_ / 400.0f);
			if (newSize <= 0.0f) {
				newSize = 0.0f;
			}
			hpSprite_->SetSize({newSize, 15.0f});

		} else {
			lifeCount_ -= static_cast<int>(other->GetRadius() * 5.0f) * 4;

			if (lifeCount_ > 400) {
				lifeCount_ = 0;
			}

			float newSize = 640.0f * (lifeCount_ / 400.0f);
			if (newSize <= 0.0f) {
				newSize = 0.0f;
			}
			hpSprite_->SetSize({newSize, 15.0f});
		}
	}

}

Vector3 Enemy::GetCenterPosition() const {

	const Vector3 offset = {0.0f, 1.0f, 0.0f};

	Vector3 worldPos = Transform(offset, worldTransformBody_.matWorld_);

	return worldPos;
}

uint32_t Enemy::GetSerialNumber() const { return serialNumber_; }

void Enemy::SettingLittles() {

	if (littleEnemys_.size() == 0) {

		float interval = 6.0f;

		for (int i = 0; i < 2; i++) {
			float zoneRotate = 0.0f;

			for (int j = 0; j < 8; j++) {

				Vector3 newPos = {0.0f, 0.0f, interval};

				Matrix4x4 rotate = MakeRotateYMatrix(zoneRotate);
				newPos = TransformNormal(newPos, rotate);
				newPos += worldTransform_.translation_;
				zoneRotate += 0.785398f;

				LittleEnemy* newLittleEnemy = new LittleEnemy();

				newLittleEnemy->Initialize({models_[kModelIndexHead]});
				newLittleEnemy->SetPosition(newPos);

				littleEnemys_.push_back(newLittleEnemy);
			}
			interval += 6.0f;
		}
	}

}

void Enemy::SetLittleEnemyTarget(const Vector3& target) {
	playerPos_ = target;
	for (LittleEnemy* littleEnemy : littleEnemys_) {
		littleEnemy->SetTargetPosision(target);
	}
}

std::list<LittleEnemy*> Enemy::GetLittleEnemyCollider() { return littleEnemys_; }

std::list<EnemyAttackZone*> Enemy::GetAttackZoneCollider() { return attackZones_; }
