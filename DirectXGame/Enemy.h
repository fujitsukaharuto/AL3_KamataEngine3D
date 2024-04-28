#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include "TimedCall.h"
#include "Collider.h"
#include <list>


class Player;
class GameScene;

class Enemy :public Collider{
public:

	enum class Phase {
		Approach, // 接近
		Leave,    // 離脱
	};

	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, uint32_t textureHandle);

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
	/// 接近処理
	/// </summary>
	void ApproachMove();

	/// <summary>
	/// 離脱処理
	/// </summary>
	void LeaveMove();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 弾発射してタイマーリセット
	/// </summary>
	void FireRoop();

	/// <summary>
	/// 接近フェーズ初期化
	/// </summary>
	void InitApproach();

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetWorldPosition() override;

	void OnCollision() override;

	//const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead() { return isDead_; }

	void SetPosition(Vector3 pos);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	GameScene* gameScene_ = nullptr;

	static void (Enemy::*pPhaseTable[])();

	static const int32_t kFireTime = 60;
	int32_t fireTimer_ = kFireTime;

	std::list<TimedCall*> timedCalls_;
	Player* player_;

	bool isDead_;

};
