#pragma once

#include <functional>

class TimedCall {
public:
	TimedCall(std::function<void(void)> function, uint32_t time);
	~TimedCall();

	void Update();
	bool IsFinished() const { return isFinish_; }

private:
	std::function<void(void)> CallBack;
	uint32_t time_;
	bool isFinish_ = false;
};
