#pragma once
#include <vector>

#include "ViewProjection.h"
#include "BaseCharacter.h"


class Player : public BaseCharacter {
public:
	Player();
	~Player();

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

	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;
	uint16_t cycle_ = 75;
	float floatingAmplitude_ = 0.175f;

	float armParameter_ = 0.0f;
	float armAmplitude_ = 0.175f;

};