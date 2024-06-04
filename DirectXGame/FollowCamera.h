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


	void SetTarget(const WorldTransform* target) { target_ = target; }


	const ViewProjection& GetViewProjection() { return viewProject_; }

private:
	ViewProjection viewProject_;
	const WorldTransform* target_ = nullptr;
};