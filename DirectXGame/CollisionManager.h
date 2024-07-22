#pragma once
#include "Collider.h"
#include "Vector3.h"

#include <list>

class CollisionManager {
public:
	CollisionManager() = default;
	~CollisionManager() = default;

	void Reset();

	/// <summary>
	/// コライダー2つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	void CheckAllCollisions();

	void AddCollider(Collider* collider);

private:

	std::list<Collider*> colliders_;

};
