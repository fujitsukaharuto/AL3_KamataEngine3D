#pragma once
#include <vector>
#include <optional>

#include "ViewProjection.h"
#include "BaseCharacter.h"


class Player : public BaseCharacter {
public:
	Player();
	~Player();


	enum class Behavior {
		kRoot,
		kAttack,
		kDash,
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
	

	//Function

	//通常行動更新
	void BehaviorRootUpdate();
	//通常行動初期化
	void BehaviorRootInitialize();

	//攻撃行動更新
	void BehaviorAttackUpdate();
	//攻撃行動初期化
	void BehaviorAttackInitialize();

	//ダッシュ行動更新
	void BehaviorDashUpdate();
	//ダッシュ行動初期化
	void BehaviorDashInitialize();

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


	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

private:

	const ViewProjection* viewProjection_ = nullptr;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformWeapon_;

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;
	float behaviorTimer_;
	Vector3 attackMove_;

	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;
	uint16_t cycle_ = 75;
	float floatingAmplitude_ = 0.175f;

	float armParameter_ = 0.0f;
	float armAmplitude_ = 0.175f;


	WorkDash workDash_;
	float destinationAngleY_ = 0;

};