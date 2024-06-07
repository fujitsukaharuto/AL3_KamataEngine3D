#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"


class Player {
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm, Vector3 position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection);
	

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


	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

private:

	const ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;
	uint32_t textureHandle_ = 0u;

	float floatingParameter_ = 0.0f;
	uint16_t cycle_ = 75;
	float floatingAmplitude_ = 0.175f;

	float armParameter_ = 0.0f;
	float armAmplitude_ = 0.175f;

};