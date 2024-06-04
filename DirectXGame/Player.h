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
	void Initialize(Model* model, uint32_t textureHandle, Vector3 position);

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


	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }


	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	const ViewProjection* viewProjection_ = nullptr;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
};