#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> function, uint32_t time):CallBack(function),time_(time){}

TimedCall::~TimedCall() {}

void TimedCall::Update()
{
	if (IsFinished()) {
		return;
	}
	time_--;
	if (time_<=0) {
	
		isFinish_ = true;
		CallBack();
	
	}
}
