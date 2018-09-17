#pragma once
#include <Siv3D.hpp>
#include "Explodable.h"


class Block {
private:
	bool destroyed;

protected:
	const Rect rect;
	const Point point;
	bool settled;

public:
	Block(const Point& point, const Point& pos, const int cellSize);
	virtual ~Block() = default;

	virtual void draw() const = 0;
	virtual void destroy() { destroyed = true; }
	bool isSettled() const { return settled; }
	void setSettled() { settled = true; }
};


class NormalBlock : public Block {
private:


public:
	NormalBlock(const Point& point_, const Point& pos, const int cellSize);
	~NormalBlock() = default;

	void draw() const override;
};


class ArrowBlock : public Block {
private:
	const ExplosionDirection direction;
	Explodable& field;

public:
	ArrowBlock(const Point& point_, const Point& pos, const int cellSize, ExplosionDirection dir, Explodable& field_);
	~ArrowBlock() = default;
	
	void explode();
	void draw() const override;
};


class InvincibleBlock : public Block {
private:


public:
	InvincibleBlock(const Point& point_, const Point& pos, const int cellSize);
	~InvincibleBlock() = default;

	void draw() const override;
	void destroy() override {} //���ȂȂ�
};