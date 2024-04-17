#include "EnemyStateLeave.h"
#include"EnemyStateApproach.h"
#include "Enemy.h"

EnemyStateLeave::EnemyStateLeave() {}

EnemyStateLeave::EnemyStateLeave(Enemy* enemy) : BaseEnemyState("State Leave", enemy) {}

EnemyStateLeave::~EnemyStateLeave() {}

void EnemyStateLeave::Update()
{

	Vector3 move = {0, 0, 0};
	const float kSpeed = 0.1f;
	const Vector3 kLeaveSpeed{-kSpeed, kSpeed, 0.0f};
	move += kLeaveSpeed;
	enemy_->StateMove(move);

}
