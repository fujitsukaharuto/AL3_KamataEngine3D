#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

class FollowCamera {
public:
	FollowCamera();
	~FollowCamera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();


	void SetTarget(const WorldTransform* target);


	void Reset();

	Vector3 OffsetCal() const;

	const ViewProjection& GetViewProjection() { return viewProject_; }

private:
	ViewProjection viewProject_;
	const WorldTransform* target_ = nullptr;
	Vector3 interTarget_ = {};
	float destinationAngleY_ = 0;
};