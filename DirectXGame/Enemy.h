#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseEnemyState.h"


class Enemy {
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

	void StateMove(const Vector3& v);

	Vector3 GetPosition() const { return worldTransform_.translation_; }

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

private:

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;

	Phase phase_ = Phase::Approach;

	static void (Enemy::*pPhaseTable[])();

	std::unique_ptr<BaseEnemyState> state_;

};
