#pragma once
#include "Model.h"
#include "WorldTransform.h"

/// <summary>
/// 天球
/// </summary>
class SkyDome {
public:
	
	SkyDome();
	~SkyDome();

	void Initialize(Model* model);
	void Update();
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

};