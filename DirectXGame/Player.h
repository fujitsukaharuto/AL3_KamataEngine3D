#pragma once
#include "Model.h"
#include "Input.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include <list>

class Player {
public:
	Player();
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model,uint32_t textureHandle);
	
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// 旋回
	/// </summary>
	void Rotate();

	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	Vector3 GetworldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;

};