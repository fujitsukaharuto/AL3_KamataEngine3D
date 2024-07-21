#pragma once
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include <memory>
#include <list>

class Enemy;

class LockOn {
public:
	LockOn() = default;
	~LockOn() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="enemies"></param>
	/// <param name="viewProjection"></param>
	void Update(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void Search(const std::list<std::unique_ptr<Enemy>>& enemies, const ViewProjection& viewProjection);

	bool SearchAreaAngle(const ViewProjection& viewProjection);

	Vector3 WorldToScreen(Vector3 worldPosition, const ViewProjection& viewProjection);

	Vector3 GetTargetPosition() const;

	bool ExistTarget() const;

private:
	std::unique_ptr<Sprite> lockOnMark_;
	uint32_t lockOnMarkHandle_ = 0;

	const Enemy* target_ = nullptr;

	float mpi = 3.14159265358f;
	const float kDegreeToRadian = mpi / 180.0f;

	// 最小距離
	float minDistance_ = 10.0f;
	// 最大距離
	float maxDistance_ = 30.0f;
	// 角度範囲
	float angleRange_ = 20.0f * kDegreeToRadian;

	bool prebutton_ = false;

};
