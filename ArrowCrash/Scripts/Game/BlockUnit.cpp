#include "BlockUnit.h"

BlockUnit::BlockUnit(UnitType type, bool explodable)
	:unit()
{
	//‚Æ‚è‚ ‚¦‚¸ ”š”­‚Í–³Ž‹
	auto& pattern = unitPatterns[(int)type];

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (pattern[i][j])
				unit.at(i).at(j) = std::make_shared<NormalBlock>();
		}
	}
}