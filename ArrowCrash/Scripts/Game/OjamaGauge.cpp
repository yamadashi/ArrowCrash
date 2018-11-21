#include "OjamaGauge.h"

OjamaGauge::OjamaGauge(const Point& pos_, const double scale_, const BlockUnitManager& mngr_)
	:pos(pos_),
	scale(scale_),
	mngr(mngr_)
{}

void OjamaGauge::draw() const {
	const auto& gauge_back = TextureAsset(L"gauge_back").scale(scale);
	gauge_back.draw(pos);

	const auto& memory = TextureAsset(L"memory").scale(scale);
	for (int i = 0; i < mngr.getOjamaCount(); i++) {
		if (i > constants::row_len - 1) break;
		const Point position(pos.movedBy(0, gauge_back.size.y - memory.size.y*(i + 1)));
		memory.draw(position);
	}
	TextureAsset(L"gauge").scale(scale).draw(pos);
}