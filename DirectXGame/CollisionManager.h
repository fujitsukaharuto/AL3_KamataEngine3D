#pragma once
#include "Collider.h"
#include <list>

class CollisionManager {
public:
	
	CollisionManager();
	~CollisionManager();

	void SetCollider(Collider* collider) { colliders_.push_back(collider); }
	void ClearCollider();

	void CheckCollision();

private:

	std::list<Collider*> colliders_;
	/// <summary>
	/// コライダー２つの衝突判定
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);
};
