#include "CollisionRecord.h"

#include <algorithm>


CollisionRecord::CollisionRecord() {}

CollisionRecord::~CollisionRecord() {}

void CollisionRecord::AddRecord(uint32_t number) {

	history_.push_back(number);

}

bool CollisionRecord::CheckHistory(uint32_t number) {
	return std::any_of(history_.begin(), history_.end(), [number](uint32_t i) {
		return i == number;
	});
}

void CollisionRecord::Clear() {

	history_.clear();

}
