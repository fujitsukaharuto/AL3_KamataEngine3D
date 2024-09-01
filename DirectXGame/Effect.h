#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Particle.h"
#include <vector>
#include <random>



class Effect {
public:
	Effect();
	~Effect();

	static Effect* GetInstance();

	void Initialize();

	void SceneReset();

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void CreateWalk(const Vector3& postion);

	void CreateEnemyDeth(const Vector3& position);

	void CreateSnowBallDeth(const Vector3& position);


private:

	Model* model_ = nullptr;
	std::list<Particle*> particles_;

	std::random_device seedGenerator;
	std::mt19937 randomEngine;

};
