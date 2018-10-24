#pragma once
#include "GIFData.h"


namespace ymds {
	//�G�t�F�N�g��GIF�A�j���̏ꍇ�ƘA���摜�̏ꍇ
	class Strategy {
	protected:
		const Point leftUpPos;
		double scale;
		double speedCoeff;

	public:
		Strategy(const Point& pos, double scale, double speedRatio)
			:leftUpPos(pos),
			scale(scale),
			speedCoeff(1.0 / speedRatio)
		{}
		virtual bool update() = 0; //kill����Ƃ�true��Ԃ�
		virtual void draw() const = 0;
	};


	//GIF�A�j���̏ꍇ
	class AnimationGIFStrategy : public Strategy {
	private:
		const AnimationGIFData data;
		size_t t;

	public:
		AnimationGIFStrategy(const FilePath& path, const Point& pos, double scale, double speedRatio);
		AnimationGIFStrategy(const AnimationGIFData& data, const Point& pos, double scale, double speedRatio);
		bool update() override;
		void draw() const override;
	};


	//�A���摜�̏ꍇ
	class LinkedImageStrategy : public Strategy {
	private:
		String textureHandler;
		size_t counter;
		size_t cellWidth;
		size_t size;

	public:
		LinkedImageStrategy(const String textureHandler_, const size_t cellWidth_, const Point& pos, double scale, double speeRatio);
		bool update() override;
		void draw() const override;
	};
}