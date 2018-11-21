#pragma once
#include <Siv3D.hpp>
#include "Block.h"
#include "../Constants.h"
#include "GameData.h"



using BlockGrid = std::vector<std::vector<std::shared_ptr<Block>>>;

class Field : public Explodable {
private:
	BlockGrid blocks;
	const Point stdPos; //基準点
	std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks;

	bool deathFlag;

	const Rect fieldShape;

	bool contains(const Point& point, bool ignoreEdge = false) const;
	std::array< bool, constants::numOfItemType > activated; //各アイテムについて起動中はtrue
	Stopwatch ItemTimers[constants::numOfItemType];

public:
	static std::vector<Field*> fields; 
	
	Field(const Point& stdPos_, std::vector<std::weak_ptr<ArrowBlock>>& arrowBlocks);
	~Field() = default;

	int explode(const Point& start, ExplosionDirection direction) override; //破壊したブロックの数を返す
	void setBlockAt(std::shared_ptr<Block> block, const Point point);
	std::shared_ptr<Block> getAt(const Point& point) const {
		return contains(point) ?
			blocks[point.x][point.y] : nullptr;
	}
	void reset();
	void riseFloor(int rising);
	void closeLine(); //行詰め

	void update();
	void draw() const;

	void init() { fields.emplace_back(this); }
	const Rect& getFieldShape() const { return fieldShape; }
	  
	bool CheckItemExistence() const;
	std::array< bool, constants::numOfItemType > getActivatedEffect() const { return activated; }
	void effectOn(int type);
	void effectEnd(int type);
	int pickUpRandomFlat();//アイテムのランダム生成の左端を返す。平地がなければ-1を返す

	bool deathCheck() { return deathFlag; }
	void restart() { deathFlag = false; }

	static void clearFieldPtr() { fields.clear(); }
};
