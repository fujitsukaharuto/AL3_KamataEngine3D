#pragma once
#include <vector>
#include <optional>

#include "ViewProjection.h"
#include "BaseCharacter.h"
#include "Hammer.h"
#include "SnowBall.h"


class LockOn;

class Player : public BaseCharacter {
public:
	Player();
	~Player();


	enum class Behavior {
		kRoot,
		kAttack,
		kDash,
		kJump,
	};

	struct ConstAttack {
		// 振りかぶりの時間
		uint32_t anticipationTime;
		// ための時間
		uint32_t chargeTime;
		// 攻撃振りの時間
		uint32_t swingTime;
		// 硬直時間
		uint32_t recoveryTime;
		// 振りかぶりの移動速さ
		float anticipationSpeed;
		// ための移動速さ
		float chargeSpeed;
		// 攻撃振りの移動速さ
		float swingSpeed;
	};

	static const int ComboNum = 3;
	static const std::array<ConstAttack, ComboNum> kConstAttacks_;

	struct WorkAttack {
		// 攻撃ギミックの媒介変数
		uint32_t attackParameter_ = 0;
		int32_t comboIndex = 0;
		int32_t inComboPhase = 0;
		bool comboNext = false;
	};

	struct WorkDash 
	{
	
		uint32_t dashParameter_ = 0;
	
	};


	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection) override;
	

	// Function

	// 通常行動更新
	void BehaviorRootUpdate();
	// 通常行動初期化
	void BehaviorRootInitialize();

	// 攻撃行動更新
	void BehaviorAttackUpdate();
	// 攻撃行動初期化
	void BehaviorAttackInitialize();

	// ダッシュ行動更新
	void BehaviorDashUpdate();
	// ダッシュ行動初期化
	void BehaviorDashInitialize();

	// ジャンプ行動更新
	void BehaviorJumpUpdate();
	// ジャンプ行動初期化
	void BehaviorJumpInitialize();

	//発射前行動更新
	void BehaviorChargeUpdate();
	//発射前行動初期化
	void BehaviorChargeInitialize();



	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 浮遊ギミック初期化
	/// </summary>
	void InitializeFloatingGimmick();

	/// <summary>
	/// 浮遊ギミック更新
	/// </summary>
	void UpdateFloatingGimmick();

	/// <summary>
	/// 腕をぶらぶらさせる初期化
	/// </summary>
	void InitializeArmGimmick();

	/// <summary>
	/// 腕をぶらぶらさせる更新
	/// </summary>
	void UpdateArmGimmick();

	void OnCollision([[maybe_unused]] Collider* other) override;

	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	void SetLockOn(const LockOn* target);

	Vector3 GetCenterPosition() const override;

	Vector3 GetCenterPosisionOrigine() const;

	Hammer* GetWeaponCollider();

	std::list<SnowBall*> GetBallCollider();

	bool GetIsAttack() const;

	//調整項目の適用
	void ApplyGlobalVariables();

	void SetLittleEnemy(std::vector<Model*> model);

private:

	const ViewProjection* viewProjection_ = nullptr;

	std::unique_ptr<Hammer> hammer_ = nullptr;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	float behaviorTimer_;
	Vector3 attackMove_;
	float attackSpeed_ = 0.2f;

	WorkAttack workAttack_;

	Vector3 velocity_ = {};
	float weight = 1.0f;

	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;
	uint16_t cycle_ = 75;
	float floatingAmplitude_ = 0.175f;

	float armParameter_ = 0.0f;
	float armAmplitude_ = 0.175f;


	WorkDash workDash_;
	float destinationAngleY_ = 0;

	const LockOn* lockOn_ = nullptr;

	std::list<SnowBall*> snowBalls_;
	bool addSnowSize_ = false;

	std::vector<Model*> littleModel_;

};