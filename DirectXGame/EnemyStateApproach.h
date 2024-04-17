#pragma once
#include "BaseEnemyState.h"

class EnemyStateApproach:public BaseEnemyState {
public:
	EnemyStateApproach();
	EnemyStateApproach(Enemy* enemy);
	~EnemyStateApproach();

	void Update();

};

