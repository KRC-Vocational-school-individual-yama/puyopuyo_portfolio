#pragma once
#include"libs/object.h"

class MouseEdit :public Object {
public:
	MouseEdit(Scene* sc);
	~MouseEdit()	override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;

	bool GetPuyoNumber(int& color,int& number);
private:
	int number;
	int mouseWheel;
	Vec2 mouse;

	int GetWheelColor();
	//”ÍˆÍ“à‚¾‚Á‚½‚ç ture ,x y ‚ğ–á‚Á‚Ä”Õ–Ê‚Ì”Ô†‚É’u‚«Š·‚¦‚é
	bool GetMousePointEdhit(int* x,int* y);
	void BoardGetMouseMiddle();
};