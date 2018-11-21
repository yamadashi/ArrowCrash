#pragma once
#include "BlockUnit.h"
#include "Item.h"
#include <queue>

class BlockUnitManager {
private:
	int playerNum;
	Field& field;
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;
	const Point stdPos;
	bool hasExchanged; //ストック交換フラグ
	int ojamaBuffer;
	const int ItemPropability;

	//描画用
	std::vector<Rect>& nextUnitFrames;
	Rect& stockFrame;

	std::list<std::shared_ptr<Unit>> nextUnits; //queue
	std::shared_ptr<Unit> currentUnit;
	std::shared_ptr<Unit> stock;
	
	void generate();
	void resetField();

	std::shared_ptr<Unit> Item;

public:
	BlockUnitManager(Field& field_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks_, GameData& gameData, int player_num);
	~BlockUnitManager() = default;
	void init() { managers.emplace_back(this); }
	void update();
	void draw() const;
	Unit& getCurrentUnit() { return *currentUnit; }
	void exchangeStock();
	void bother(int numOfDestroyed);
	int calculateRising(int numOfDestroyed);
	void changeSpeed(bool active);
	void changeForbid(bool active);
	void GuardOn();
	void closeField() { field.closeLine(); }
	int getOjamaCount() const { return ojamaBuffer; }

	static std::vector<BlockUnitManager*> managers;
	static void clearManagerPtr() { managers.clear(); }
};