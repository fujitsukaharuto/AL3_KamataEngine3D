#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>

class RailCamera {
public:
	RailCamera();
	~RailCamera();

	void Initialize(Vector3 world,Vector3 rotate);

	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;

};