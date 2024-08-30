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

		particles_.push_back(newParticle);
	}

}
