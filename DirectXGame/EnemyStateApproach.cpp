#include"EnemyStateApproach.h"
#include "EnemyStateLeave.h"
#include "Enemy.h"

EnemyStateApproach::EnemyStateApproach() {}

EnemyStateApproach::EnemyStateApproach(Enemy* enemy) : BaseEnemyState("State Approach", enemy) {}

EnemyStateApproach::~EnemyStateApproach() {}

void EnemyStateApproach::Update() 
{
	Vector3 move = {0, 0, 0};
	const float kApproachSpeedZ = 0.2f;
	const Vector3 kApproachSpeed{0.0f, 0.0f, -kApproachSpeedZ};
	move += kApproachSpeed;
	enemy_->StateMove(move);
	if (enemy_->GetPosition().z<=0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyStateLeave>(enemy_));
	}
}
