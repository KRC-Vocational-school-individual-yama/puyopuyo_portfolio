#pragma once
#include"libs/object.h"
class KeyManager;

class DebugUi :public Object {
public:
	DebugUi(Scene* sc);
	~DebugUi()		override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;
private:
	KeyManager* keys;
	Vec2 mouse;
};