#pragma once
#include <string>
#include <format>

class Enemy;

class BaseEnemyState {
public:
	BaseEnemyState();
	BaseEnemyState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy){};
	virtual ~BaseEnemyState() = 0;

	virtual void Update() = 0;

protected:

	std::string name_;
	Enemy* enemy_ = nullptr;

};
