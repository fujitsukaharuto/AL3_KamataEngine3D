#pragma once
#include "Collider.h"

#include <list>

class CollisionManager {
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	void Initialize();

	void UpdateWorldTransform();

	void Draw(const ViewProjection& viewProjection);

	void Reset();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	void CheckAllCollisions();

	void AddCollider(Collider* collider);

	// 調整項目の適用
	void ApplyGlobalVariables();

private:

	std::list<Collider*> colliders_;
	std::unique_ptr<Model> icoModel_;
	bool isShowCollision_ = false;

};
