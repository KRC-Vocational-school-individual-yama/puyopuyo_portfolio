#pragma once
#include"libs/object.h"
class Board;
class Player;
class KeyManager;

class Sequence :public Object {
public:
	Sequence(Scene* sc);
	~Sequence()		override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;
private:
	enum class State {
		start
		,playerFoll
		,fieldSet
		,fieldFoll
		,fieldErase
		,gameOver
		,end
	};

	State sequenser;//satate
	Board * board ;
	Player* player;
	KeyManager* keys;
	int gameSpeedCount;
	int hImage;
	const int gameSpeed = 10;

	void State_Start();
	void State_PlayerFoll();
	void State_FieldSet();
	void State_FieldFoll();
	void State_FieldErase();
	void State_End();
};