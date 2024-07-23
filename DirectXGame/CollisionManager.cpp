#include "CollisionManager.h"
#include "MathCal.h"
#include "GlobalVariables.h"

void CollisionManager::Initialize() {

	icoModel_.reset(Model::CreateFromOBJ("ICO", true));
	GlobalVariables* globalvariables = GlobalVariables::GetInstance();
	const char* groupName = "CollisionModel";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalvariables->AddItem(groupName, "ShowCollision", isShowCollision_);

}

void CollisionManager::UpdateWorldTransform() {

	ApplyGlobalVariables();

	if (!isShowCollision_) {
		return;
	}

	for (Collider* collider : colliders_) {
		collider->UpdateWorldTransform();
	}

}

void CollisionManager::Draw(const ViewProjection& viewProjection) {

	if (!isShowCollision_) {
		return;
	}

	for (Collider* collider : colliders_) {
		collider->Draw(icoModel_.get(), viewProjection);
	}

}

void CollisionManager::Reset() {

	colliders_.clear();

}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {

	Vector3 positionA = colliderA->GetCenterPosition();
	Vector3 positionB = colliderB->GetCenterPosition();

	Vector3 subtract = positionB - positionA;
	float distance = subtract.Lenght();

	if (distance < colliderA->GetRadius()+colliderB->GetRadius()) {

		colliderA->OnCollision(colliderB);

		colliderB->OnCollision(colliderA);

	}

}

void CollisionManager::CheckAllCollisions() {

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {

			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void CollisionManager::AddCollider(Collider* collider) {

	colliders_.push_back(collider);

}

void CollisionManager::ApplyGlobalVariables() {
	GlobalVariables* globalvariables = GlobalVariables::GetInstance();
	const char* groupName = "CollisionModel";
	isShowCollision_ = globalvariables->GetBoolValue(groupName, "ShowCollision");
}
