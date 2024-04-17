#pragma once
#include "BaseEnemyState.h"

class EnemyStateLeave : public BaseEnemyState {
public:
	EnemyStateLeave();
	EnemyStateLeave(Enemy* enemy);
	~EnemyStateLeave();

	void Update();

};