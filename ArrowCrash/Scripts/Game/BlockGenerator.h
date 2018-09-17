#pragma once
#include "Field.h"

class BlockGenerator {
private:
	Field& field;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;
	void generate();

public:
	BlockGenerator(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_);
	~BlockGenerator() = default;
	void update();
};