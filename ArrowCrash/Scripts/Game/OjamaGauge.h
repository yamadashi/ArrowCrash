#pragma once
#include "BlockUnitManager.h"

class OjamaGauge {
private:
	const Point pos;
	const double scale;
	const BlockUnitManager& mngr;

public:
	OjamaGauge(const Point& pos_, const double scale_,  const BlockUnitManager& mngr_);
	void draw() const;
};