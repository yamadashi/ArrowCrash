#pragma once
#include <Siv3D.hpp>
#include "Explodable.h"
#include "../ymdsLib/Effect/EffectGenerator.h"


enum class RotateDirection {
	Right, Left
};


class Block {
private:

protected:
	bool destroyed;
	Point stdPos;
	Point point;
	const int blockSize;
	Rect rect;
	bool settled;

public:
	Block(const Point& point, const Point& stdPos, const int blockSize);
	virtual ~Block() = default;

	virtual void draw() const = 0;
	virtual void draw(const Point& pos, double scale) const = 0;

	bool isDestroyed() const { return destroyed; }
	const Point& getPoint() const { return point; }
	void setPoint(const Point& point_);
	bool isSettled() const { return settled; }
	void setSettled() { settled = true; }

	virtual void destroy();

	virtual void rotate(RotateDirection) {}
};


class NormalBlock : public Block {
private:


public:
	NormalBlock(const Point& point_, const Point& stdPos, const int blockSize);
	~NormalBlock() = default;
	
	void draw() const override;
	void draw(const Point& pos, double scale) const;
};


class ArrowBlock : public Block {
private:
	ExplosionDirection direction;
	Explodable& field;

public:
	ArrowBlock(const Point& point_, const Point& stdPos, const int blockSize, ExplosionDirection dir, Explodable& field_);
	~ArrowBlock() = default;

	void draw() const override;
	void draw(const Point& pos, double scale) const;
	void explode();
	void rotate(RotateDirection rot) override;
	ExplosionDirection getDirection() const { return direction; }
};


enum class PartPlace {
	UpRight, DownRight, DownLeft, UpLeft
};

enum class ItemCondition {
	Exist, Destroyed, NotExist
};

class ItemBlock : public Block {
private:
	const PartPlace Part;
	static ItemCondition condition;

public:
	ItemBlock(const Point& point_, const Point& stdPos, const int blockSize, const PartPlace Part);
	~ItemBlock() = default;
	
	void draw() const override;
	void draw(const Point& pos, double scale) const;
	static const ItemCondition CheckItem() { return condition; }
	static void DeleteItem() { condition = ItemCondition::NotExist; }
	static void Generate() { condition = ItemCondition::Exist; }
	void destroy() override;
};


class InvincibleBlock : public Block {
private:


public:
	InvincibleBlock(const Point& point_, const Point& stdPos, const int blockSize);
	~InvincibleBlock() = default;

	void draw() const override;
	void draw(const Point& pos, double scale) const {}
	void destroy() override {} //Ž€‚È‚È‚¢
};