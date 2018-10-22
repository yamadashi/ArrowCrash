#pragma once
#include <Siv3D.hpp>
#include "Field.h"
#include "Block.h"

using namespace std;

class Item{

	private:
		

	protected:
		
		bool destroyed;
		std::array<std::array<std::shared_ptr<Block>, 2>, 2> geometry;

		Field& field;
		Point point;
		Stopwatch timer;
		const int blockSize;
		Point stdPos;
		Rect rect;
		bool settled;
		bool checkCollision(const Point& point_) const;
		void settle();

	public:
		Item(const Point& point, const Point& stdPos, const int blockSize, Field& field_);
		virtual ~Item() = default;

		void update();

		virtual void draw() const = 0;

		virtual void destroy() { destroyed = true; }
		bool isDestroyed() const { return destroyed; }

		const Point& getPoint() const { return point; }
		void setPoint(const Point& point_);

		bool isSettled() const { return settled; }
		void setSettled() { settled = true; }
};

//âÒì]ã÷é~
class ForbidRotating : public Item {
private:


public:
	ForbidRotating(const Point& point_, const Point& stdPos, const int blockSize, Field& field_);
	~ForbidRotating() = default;

	void draw() const override;
};


//óéâ∫ë¨ìxè„è∏
class IncreaseFallVelocity : public Item {
private:
	
public:
	IncreaseFallVelocity(const Point& point_, const Point& stdPos, const int blockSize, Field& field_);
	~IncreaseFallVelocity() = default;

	void draw() const override;
};


//Ç®é◊ñÇñ≥å¯
class InvalidateInterruption : public Item {
private:


public:
	InvalidateInterruption(const Point& point_, const Point& stdPos, const int blockSize, Field& field_);
	~InvalidateInterruption() = default;

	void draw() const override;
};