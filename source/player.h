#pragma once
#include"libs/object.h"

class Player :public Object {
public:
	Player(Scene* sc);
	~Player()		override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;

	//一定時間で落ちていく true落ちることができない　false落ちることができる。
	[[nodiscard]]bool PuyoFoll();

	void SetBoard();
	void Reset();
	void OnDisp();
	void OffDisp();
private:
	struct Puyo{
		int color= -1;
		Vec2 pos ;//どっちについているか
	};
	std::vector<Puyo>puyo;
	Vec2 mainPos;
	int puyoDirection;
	int follCount;
	bool isDisp;
	enum class Direction {
		non=-1
		,left=0
		, up
		, right
		, down
		, max
	};
	//プレイヤーのぷよ色を初期化
	void InitPlayerPuyo();
	Vec2 GetDirectionPuyo(int dir);
	template<class C=Direction>
	const int EnumCast(C enums) {
		return static_cast<int>(enums);
	}
	//true 動ける　false 動けない
	bool CheckMove(int x,int y);
	bool CheckMove(Vec2 pos) { return CheckMove(pos.x, pos.y); }

	int MyDirection();

	//input操作
	void Playable();
	void Movement();
	void Rotation();
};