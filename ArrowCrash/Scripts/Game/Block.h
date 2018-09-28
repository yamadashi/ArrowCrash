#pragma once
#include <Siv3D.hpp>
#include "Explodable.h"


enum class RotateDirection {
	Right, Left
};

class Block {
private:
	bool destroyed;

protected:
	Point stdPos;
	Point point;
	const int blockSize;
	Rect rect;
	bool settled;

public:
	Block(const Point& point, const Point& stdPos, const int blockSize);
	virtual ~Block() = default;

	virtual void draw() const = 0;

	virtual void destroy() { destroyed = true; }
	bool isDestroyed() const { return destroyed; }
	
	const Point& getPoint() const { return point; }
	void setPoint(const Point& point_);

	bool isSettled() const { return settled; }
	void setSettled() { settled = true; }

	virtual void rotate(RotateDirection) {}
};


class NormalBlock : public Block {
private:


public:
	NormalBlock(const Point& point_, const Point& stdPos, const int blockSize);
	~NormalBlock() = default;

	void draw() const override;
};


class ArrowBlock : public Block {
private:
	ExplosionDirection direction;
	Explodable& field;

public:
	ArrowBlock(const Point& point_, const Point& stdPos, const int blockSize, ExplosionDirection dir, Explodable& field_);
	~ArrowBlock() = default;

	void draw() const override;
	void explode();
	void rotate(RotateDirection rot) override;
	ExplosionDirection getDirection() const { return direction; }
};


class InvincibleBlock : public Block {
private:


public:
	InvincibleBlock(const Point& point_, const Point& stdPos, const int blockSize);
	~InvincibleBlock() = default;

	void draw() const override;
	void destroy() override {} //Ž€‚È‚È‚¢
};