#pragma once
#include "Field.h"

class BlockGenerator {
private:
	Field& field;
	std::vector<ArrowBlock&>& arrowBlocks;
	void generate();

public:
	BlockGenerator(Field& field_, std::vector<ArrowBlock&>& arrowBlocks_);
	~BlockGenerator() = default;
	void update();
};