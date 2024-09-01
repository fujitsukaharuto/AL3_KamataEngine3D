#pragma once
#include <vector>
#include <optional>

#include "BaseCharacter.h"
#include "ViewProjection.h"
#include "LittleEnemy.h"
#include "EnemyAttackZone.h"
#include "StandbyOperation.h"
#include "Sprite.h"


enum class AttackType : uint32_t {

	kDefault,
	kFlattery,
	kSummon,
	kDush,
};

class Enemy : public BaseCharacter {
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*>& models) override;

	void SceneReset();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection) override;

	void DrawSprite();

	void Move();

	void UpdatePartsGimmick();

	void GraspVision();

	void Attack();

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override;

	uint32_t GetSerialNumber() const; 

	void SettingLittles();

	void SetLittleEnemyTarget(const Vector3& target);

	std::list<LittleEnemy*> GetLittleEnemyCollider();

	std::list<EnemyAttackZone*> GetAttackZoneCollider();

	uint32_t GetLifeCount() { return lifeCount_; }

	bool IsEndEnemy() { return endEnemy_; }

private:

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;
	uint32_t serialNumber_ = 0;
	static uint32_t nextNerialNumber_;

	std::list<LittleEnemy*> littleEnemys_;
	std::list<EnemyAttackZone*> attackZones_;

	Vector3 playerPos_ = {0.0f, 0.0f, 0.0f};
	Vector3 oldPlayerPos_ = playerPos_;
	uint32_t occurrenceTime_ = 60;
	uint32_t occurrencesCount_ = 0;

	AttackType attackType_ = AttackType::kDefault;
	std::optional<AttackType> attackTypeRequest_ = std::nullopt;
	uint32_t attackCooltime_ = 60;

	std::list<StandbyOperation*> standbies_;
	bool isStandby_ = false;


	uint32_t lifeCount_ = 300;
	uint32_t hpTexture_ = 0;
	Sprite* hpSprite_;

	bool endEnemy_ = false;
	uint32_t endTime_ = 0;


	uint32_t hpFrameTexture_ = 0;
	Sprite* hpFrameSprite_;

	uint32_t nameTexture_ = 0;
	Sprite* nameSprite_;


	uint32_t damageSound_ = 0;
	uint32_t bigDamageSound_ = 0;
	uint32_t deathSound_ = 0;
};