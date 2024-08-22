#pragma once
#include "BaseCharacter.h"
#include "WorldTransform.h"
#include "Model.h"

class SnowBall;

class LittleEnemy : public BaseCharacter{
public:
	LittleEnemy();
	~LittleEnemy();

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

	void Move();

	void UpdatePartsGimmick();

	void OnCollision([[maybe_unused]] Collider* other) override;

	Vector3 GetCenterPosition() const override;

	Vector3 GetOrigineScale();

	void Setparent(const WorldTransform& pare);

	void SetRotate(const Vector3& rotate);

	void SetSclae(const Vector3& scale);

	void SetPosition(const Vector3& pos);

	bool IsDead() const { return isDead_; }

private:

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	SnowBall* snowball_;
	bool isGetCaughtUp_ = false;
	bool isDead_ = false;

};
