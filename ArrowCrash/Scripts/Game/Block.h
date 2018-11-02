#pragma once
#include <Siv3D.hpp>
#include "Explodable.h"
#include "../ymdsLib/Effect/EffectGenerator.h"
#include "../Enum.h"

class Block {
private:

protected:
	bool destroyed;
	Point stdPos;
	Point point;
	Rect rect;
	bool settled;

public:
	Block(const Point& point, const Point& stdPos);
	virtual ~Block() = default;

	virtual void draw() const = 0;
	virtual void draw(const Point& pos, double scale) const = 0;

	bool isDestroyed() const { return destroyed; }
	const Point& getPoint() const { return point; }
	void setPoint(const Point& point_);
	bool isSettled() const { return settled; }
	void setSettled() { settled = true; }

	virtual bool ItemCheck() { return false; }
	virtual void destroy();

	virtual void rotate(RotateDirection) = 0;

	static int blockSize;
};


class NormalBlock : public Block {
private:
	UnitType type;

public:
	NormalBlock(const Point& point_, const Point& stdPos, UnitType type);
	~NormalBlock() = default;
	
	void draw() const override;
	void draw(const Point& pos, double scale) const;
	void rotate(RotateDirection) override {}
};


class ArrowBlock : public Block {
private:
	ExplosionDirection direction;
	Explodable& field;

public:
	ArrowBlock(const Point& point_, const Point& stdPos, ExplosionDirection dir, Explodable& field_);
	~ArrowBlock() = default;

	void draw() const override;
	void draw(const Point& pos, double scale) const;
	int explode(); //破壊したブロックの数を返す
	void rotate(RotateDirection rot) override;
};


enum class PartPlace {
	UpRight, DownRight, DownLeft, UpLeft
};

class ItemBlock : public Block {
private:
	const PartPlace Part;

public:
	ItemBlock(const Point& point_, const Point& stdPos, const PartPlace Part);
	~ItemBlock() = default;

	void draw() const override;
	void draw(const Point& pos, double scale) const;
	void rotate(RotateDirection) override {}
	bool ItemCheck() override { return true; }
};


class InvincibleBlock : public Block {
private:

public:
	InvincibleBlock(const Point& point_, const Point& stdPos);
	~InvincibleBlock() = default;

	void draw() const override;
	void draw(const Point&, double) const {}
	void destroy() override {} //死なない
	void rotate(RotateDirection) override {}
};