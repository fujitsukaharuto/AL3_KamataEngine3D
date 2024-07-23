#pragma once
#include <vector>


class CollisionRecord {
public:
	CollisionRecord();
	~CollisionRecord();

	void AddRecord(uint32_t number);

	bool CheckHistory(uint32_t number);

	void Clear();

private:
	std::vector<uint32_t> history_;
};