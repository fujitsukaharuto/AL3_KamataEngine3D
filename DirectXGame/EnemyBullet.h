#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Player; 

class EnemyBullet {
public:
	EnemyBullet();
	~EnemyBullet();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPos();

	void OnCollision();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Vector3 velocity_;
	static const int32_t kLifeTime = 60 * 5;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;
	Player* player_;
};