#include "Effect.h"

Effect::Effect() : randomEngine(seedGenerator()) {}

Effect::~Effect() {

	for (Particle* particle : particles_) {
		delete particle;
	}
	delete model_;
}

Effect* Effect::GetInstance() {
	static Effect instance;
	return &instance;
}

void Effect::Initialize() {

	model_ = Model::CreateSphere();

}

void Effect::SceneReset() {

	for (Particle* particle : particles_) {
		particle->setDeath();
	}

}

void Effect::Update() {

	particles_.remove_if([](Particle* particle) {
		if (particle->IsDead()) {
			delete particle;
			return true;
		}
		return false;
	});

	for (Particle* particle : particles_) {
		particle->Update();
	}

}

void Effect::Draw(const ViewProjection& viewProjection) {
	for (Particle* particle : particles_) {
		particle->Draw(viewProjection);
	}
}

void Effect::CreateWalk(const Vector3& postion) {

	std::uniform_real_distribution<float> distribution(-0.05f, 0.05f);

	for (int i = 0; i < 5; i++) {
		Particle* newParticle = new Particle();

		Vector3 newVelocity = {distribution(randomEngine), 0.025f, distribution(randomEngine)};
		Vector3 newPosition = {postion.x + (distribution(randomEngine) * 7.5f), 0.0f, postion.z + (distribution(randomEngine) * 7.5f)};

		newParticle->Initialize(model_, newVelocity, newPosition);
		newParticle->SetLiveTime(10);
		newParticle->SetSize(0.2f);
		newParticle->SetColor({0.847f, 0.639f, 0.451f, 1.0f});

		particles_.push_back(newParticle);
	}

}

void Effect::CreateEnemyDeth(const Vector3& position) {

	std::uniform_real_distribution<float> distribution(-0.1f, 0.1f);
	std::uniform_real_distribution<float> distributionPlus(0.0f, 0.15f);

	Vector3 newpos = {position.x + (distribution(randomEngine) * 9.0f), position.y + (distribution(randomEngine) * 6.0f), position.z + (distribution(randomEngine) * 9.0f)};

	for (int i = 0; i < 30; i++) {
		Particle* newParticle = new Particle();

		Vector3 newVelocity = {(distribution(randomEngine) * 2.0f), (distributionPlus(randomEngine) * 4.0f), (distribution(randomEngine) * 2.0f)};
		Vector3 newPosition = {newpos.x + (distribution(randomEngine) * 7.25f), newpos.y, newpos.z + (distribution(randomEngine) * 7.25f)};

		newParticle->Initialize(model_, newVelocity, newPosition);
		newParticle->SetLiveTime(50);
		newParticle->SetSize(0.8f);
		newParticle->SetGravity(true);
		newParticle->SetColor({0.086f, 0.086f, 0.055f, 1.0f});

		particles_.push_back(newParticle);
	}

}

void Effect::CreateSnowBallDeth(const Vector3& position) {

	std::uniform_real_distribution<float> distribution(-0.1f, 0.1f);
	std::uniform_real_distribution<float> distributionPlus(0.0f, 0.15f);

	for (int i = 0; i < 30; i++) {
		Particle* newParticle = new Particle();

		Vector3 newVelocity = {(distribution(randomEngine) * 2.0f), (distributionPlus(randomEngine) * 4.0f), (distribution(randomEngine) * 2.0f)};
		Vector3 newPosition = {position.x + (distribution(randomEngine) * 7.25f), position.y, position.z + (distribution(randomEngine) * 7.25f)};

		newParticle->Initialize(model_, newVelocity, newPosition);
		newParticle->SetLiveTime(50);
		newParticle->SetSize(0.4f);
		newParticle->SetGravity(true);
		newParticle->SetColor({0.580f, 0.518f, 0.416f, 1.0f});

		particles_.push_back(newParticle);
	}

}
