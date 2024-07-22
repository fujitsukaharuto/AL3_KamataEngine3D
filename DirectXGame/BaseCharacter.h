#pragma once
#include <vector>

#include "Model.h"
#include "WorldTransform.h"
#include "Collider.h"


class BaseCharacter : public Collider {
public:
	BaseCharacter();
	~BaseCharacter();

	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewProjection);

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	virtual Vector3 GetCenterPosition() const override;

protected:
	std::vector<Model*> models_;
	WorldTransform worldTransform_;

private:


};