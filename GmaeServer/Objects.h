#pragma once
#include "Primitives.h"


class Sprite
{
public:
	Sprite() {}
	Sprite(std::string name, int importanceZ = 0) : 
		ptr_image(&Image::images[name]), importance(importanceZ)
	{
		auto [w, h] = ptr_image->size();
		react = { 0, 0, w, h };
	}

	char get_fragment(int x, int y) const
	{
		int bias_x = react.x + x;
		int bias_y = react.y + y;
		int width  = ptr_image->size().width;
		return (*ptr_image)[bias_y * width + bias_x];
	}
	auto size() const
	{
		return Size(react.w, react.h);
	}

private:
	Image* ptr_image = nullptr;

public:
	int       importance = NULL;
	Posithion posithion;
	React	  react;
};





class Ñollider
{
public:
	Ñollider() {}
	Ñollider(std::string name) :
		ptr_image(&Image::images[name])
	{
		auto [w, h] = ptr_image->size();
		react = { 0, 0, w, h };
	}

	char get_fragment(int x, int y) const
	{
		int bias_x = react.x + x;
		int bias_y = react.y + y;
		int width  = ptr_image->size().width;
		return (*ptr_image)[bias_y * width + bias_x];
	}

	auto size() const
	{
		return Size(react.w, react.h);
	}

	bool collision(Posithion global, const Ñollider& other, Posithion global_other)
	{
		auto [ w,  h] = this->size();
		auto [_w, _h] = other.size();

		auto [ pos_x,  pos_y] = (global		  +		  posithion).get_int();
		auto [_pos_x, _pos_y] = (global_other + other.posithion).get_int();

		//-----ïðîâåðêà íà õ-----//
		if (std::max<int>(pos_x, _pos_x) >= std::min<int>(pos_x + w, _pos_x + _w)) return false;

		for (int i = std::max<int>(pos_y, _pos_y); i < std::min<int>(pos_y + h, _pos_y + _h); ++i)
		{
			for (int j = std::max<int>(pos_x, _pos_x); j < std::min<int>(pos_x + w, _pos_x + _w); ++j)
			{
				char  fragment = this->get_fragment(j -  pos_x, i -  pos_y);
				char _fragment = other.get_fragment(j - _pos_x, i - _pos_y);
				if (fragment != ' ' && _fragment != ' ') return true;
			}
		}
		return false;
	}

protected:
	Image* ptr_image = nullptr;

public:
	Posithion posithion;
	React	  react;
};





class AnimFrame
{
public:
	AnimFrame(std::initializer_list<React> reacts, float speed) :
		reacts(reacts), speed(speed), curentFrame(NULL), period(false) {}

	React get_react()
	{
		return reacts[curentFrame];
	}
	bool  end_anim()
	{
		return period;
	}
	void  restart()
	{
		curentFrame = NULL;
	}
	void  update(float elapsedTime)
	{
		period = false;
		curentFrame += speed * elapsedTime;
		if (curentFrame >= reacts.size()) curentFrame = NULL, period = true;
	}

protected:
	float			   curentFrame;
	bool			   period;
	std::vector<React> reacts;
	float			   speed;
};





























//class DependentImage
//{
//public:
//	DependentImage(std::string name) : ptr_image(&Image::images[name])
//	{
//		auto [w, h] = ptr_image->size();
//		react = { 0, 0, w, h };
//	}
//
//	auto size() const
//	{
//		return vector2T<int>(react.w, react.h);
//	}
//
//protected:
//	Image* ptr_image = nullptr;
//
//public:
//	PosithionTest posithion;
//	React	      react;
//};
//
//
//class SpriteTest : public DependentImage
//{
//public:
//	SpriteTest(std::string name, int importance = 0) :
//		DependentImage(name), importance(importance) {}
//
//	char get_fragment(int x, int y) const
//	{
//		int bias_x = react.x + x;
//		int bias_y = react.y + y;
//		int width  = ptr_image->size().width;
//		return (*ptr_image)[bias_y * width + bias_x];
//	}
//
//public:
//	int importance = NULL;
//};
//
//
//
//class ÑolliderTest : public DependentImage
//{
//public:
//	ÑolliderTest(std::string name) : 
//		DependentImage(name) {}
//
//	bool collision(const Ñollider& other)
//	{
//		other.ptr_image;
//
//
//
//		//auto& ref_m = *sprite_ptr;
//		//auto& ref_o = *other.sprite_ptr;
//		//auto [w, h] = ref_m.size();
//		//auto [_w, _h] = ref_o.size();
//		//auto [pos_x, pos_y] = ref_m.posithion.curent();
//		//auto [_pos_x, _pos_y] = ref_o.posithion.curent();
//
//		////-----ïðîâåðêà íà õ-----//
//		//if (std::max(pos_x, _pos_x) >= std::min(pos_x + w, _pos_x + _w)) return false;
//
//		//for (int i = std::max(pos_y, _pos_y); i < std::min(pos_y + h, _pos_y + _h); ++i)
//		//{
//		//	for (int j = std::max(pos_x, _pos_x); j < std::min(pos_x + w, _pos_x + _w); ++j)
//		//	{
//		//		char  fragment = ref_m.get_fragment(j - pos_x, i - pos_y);
//		//		char _fragment = ref_o.get_fragment(j - _pos_x, i - _pos_y);
//		//		if (fragment != ' ' && _fragment != ' ') return true;
//
//		//		//-----------!!!!!!!!!!!!!!--------------------//
//		//		// return point()
//
//		//	}
//		//}
//		return false;
//	}
//};