#include "CollisionManager.h"

CollisionManager::CollisionManager() {}

CollisionManager::~CollisionManager() {}

void CollisionManager::ClearCollider() {
	if (colliders_.size()>0) {
		colliders_.clear();
	}
}

void CollisionManager::CheckCollision() {
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	if (0xffffffff == (colliderA->GetAttribute() ^ colliderB->GetMask()) || 0xffffffff == (colliderB->GetAttribute() ^ colliderA->GetMask())) {
		return;
	}

	Vector3 posA, posB;
	posA = colliderA->GetWorldPosition();
	posB = colliderB->GetWorldPosition();
	Vector3 leng = posB - posA;
	float length = leng.Lenght();
	if (length <= 3.0f) {
		colliderA->OnCollision();
		colliderB->OnCollision();
	}
}