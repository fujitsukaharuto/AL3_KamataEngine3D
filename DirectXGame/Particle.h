#pragma once
#include "Model.h"
#include "WorldTransform.h"



class Particle {
public:
	Particle();
	~Particle();

	void Initialize(Model* model, Vector3 velocity, Vector3 position);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	void SetGravity(const bool is) { isGravity_ = is; }

	void SetColor(const Vector4& color);

	void SetLiveTime(const uint32_t lifeTime) { liveTime_ = lifeTime; }

	void SetSize(const float size);

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	ObjectColor color_;

	Vector3 velocity_;
	uint32_t liveTime_ = 0;

	bool isGravity_ = false;
	bool isDead_ = false;

};
