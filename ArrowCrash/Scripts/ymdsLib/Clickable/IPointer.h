#pragma once
#include <Siv3D.hpp>

namespace ymds {

	class IPointer {
	private:
		
	public:
		IPointer() = default;
		virtual ~IPointer() = default;

		virtual void update() = 0;
		virtual void draw() const = 0;

		virtual bool isClicked() const = 0;
		virtual Point getPos() const = 0;
	};

}