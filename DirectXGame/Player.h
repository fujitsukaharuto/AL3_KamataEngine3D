#pragma once
#include "Model.h"
#include "Input.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "PlayerBullet.h"
#include "Collider.h"
#include <list>

class Player :public Collider {
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
	void Update(const ViewProjection& viewProjection);
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション（参照渡し）</param>
	void Draw(const ViewProjection& viewProjection);

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

	Vector3 GetWorldPosition() override;

	void OnCollision() override;

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	void SetParent(const WorldTransform* parent);

	void DrawUI();

	void ReticleCal(const ViewProjection& viewProjection);

	void ReticleMouse(const ViewProjection& viewProjection);


private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	WorldTransform worldTransfoem3DReticle_;

	Sprite* sprite2DReticle_ = nullptr;

};