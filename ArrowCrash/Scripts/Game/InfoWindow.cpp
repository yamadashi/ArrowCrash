#include "InfoWindow.h"


InfoWindow::InfoWindow(int playerNum, GameData& data, const int& score_)
	:playerNumber(playerNum),
	score(score_),
	size(1.0 * data.fieldSize.x, 0.2 * data.fieldSize.x),
	//•ª‚©‚è‚É‚­‚­‚Ä‚²‚ß‚ñ‚â‚Å
	centerPos(data.playerRegion.x * (0.5 + playerNumber), data.stdPositions[playerNumber].y + data.fieldSize.y + (Window::Height() - (data.stdPositions[playerNumber].y + data.fieldSize.y)) * 0.5)
{}

void InfoWindow::draw() const {
	TextureAsset(L"infoFrame").resize(size).drawAt(centerPos);
	(*font)(playerNumber+1, L"P score:", score).drawAt(centerPos);
}

void InfoWindow::setFont(String handler, int size) {
	font.reset(new Font(size, handler));
}

std::unique_ptr<Font> InfoWindow::font(nullptr);