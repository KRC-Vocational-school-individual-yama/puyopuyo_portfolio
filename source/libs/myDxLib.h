#pragma once

#include<DxLib.h>
#include<cassert>
#include"../main/config.h"

namespace DxLib {
	struct Vec2 {
		int x=0, y=0;

		static Vec2 zero() { return Vec2{ 0,0 }; }
		
		Vec2 operator +(const Vec2& a) {return Vec2{ this->x + a.x,this->y + a.y };		}
		Vec2 operator -(const Vec2& a) {return Vec2{ this->x - a.x,this->y - a.y };		}
		Vec2 operator *(const Vec2& a) {return Vec2{ this->x * a.x,this->y * a.y };		}
		Vec2 operator /(const Vec2& a) {return Vec2{ this->x / a.x,this->y / a.y };		}
		Vec2 operator %(const Vec2& a) {return Vec2{ this->x % a.x,this->y % a.y };		}
		const Vec2& operator +=(const Vec2& a) {
			this->x += a.x;
			this->y += a.y;
			return (*this);
		}
		const Vec2& operator -=(const Vec2& a) {
			this->x -= a.x;
			this->y -= a.y;
			return (*this);
		}
		const Vec2& operator *=(const Vec2& a) {
			this->x *= a.x;
			this->y *= a.y;
			return (*this);
		}
		const Vec2& operator /=(const Vec2& a) {
			this->x /= a.x;
			this->y /= a.y;
			return (*this);
		}
		const Vec2& operator %=(const Vec2& a) {
			this->x %= a.x;
			this->y %= a.y;
			return (*this);
		}
		const bool operator ==(const Vec2& a) {return ((this->x == a.x) && (this->y == a.y));}
	};


	struct Rect {
		Rect(float x, float y, float sx, float sy)
			:posX(x)
			, posY(y)
			, sizeX(sx)
			, sizeY(sy)
		{}

		float posX;
		float posY;
		float sizeX;
		float sizeY;
	};

	//ŽlŠp“–‚½‚è”»’è
	inline bool SquareCollision(Rect a, Rect b) {
		if (a.posY >= b.posY + b.sizeY)
			return false;//up
		if (a.posY + a.sizeY <= b.posY)
			return false;//down

		if (a.posX >= b.posX + b.sizeX)
			return false;//left
		if (a.posX + a.sizeX <= b.posX)
			return false;//right


		return true;
	}
}