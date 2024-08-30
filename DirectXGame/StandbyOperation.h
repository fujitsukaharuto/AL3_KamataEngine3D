#pragma once
#include "Model.h"
#include "WorldTransform.h"



class StandbyOperation {
public:
	StandbyOperation();
	~StandbyOperation();

		// type 0 : 四角 1 : 丸
	void Initialize(const Vector3& position, const int type);

	void TypeInitialize(const int type);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void SetTransform(const Vector3& trans);

	void SetRotate(const Vector3& rotate);

	int GetType() const { return type_; }

private:

	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	WorldTransform shadow_;
	ObjectColor color_;
	int type_;
	float time_ = 0;
};
