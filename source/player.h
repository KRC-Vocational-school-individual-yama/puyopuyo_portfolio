#pragma once
#include"libs/object.h"

class Player :public Object {
public:
	Player(Scene* sc);
	~Player()		override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;

	//��莞�Ԃŗ����Ă��� true�����邱�Ƃ��ł��Ȃ��@false�����邱�Ƃ��ł���B
	[[nodiscard]]bool PuyoFoll();

	void SetBoard();
	void Reset();
	void OnDisp();
	void OffDisp();
private:
	struct Puyo{
		int color= -1;
		Vec2 pos ;//�ǂ����ɂ��Ă��邩
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
	//�v���C���[�̂Ղ�F��������
	void InitPlayerPuyo();
	Vec2 GetDirectionPuyo(int dir);
	template<class C=Direction>
	const int EnumCast(C enums) {
		return static_cast<int>(enums);
	}
	//true ������@false �����Ȃ�
	bool CheckMove(int x,int y);
	bool CheckMove(Vec2 pos) { return CheckMove(pos.x, pos.y); }

	int MyDirection();

	//input����
	void Playable();
	void Movement();
	void Rotation();
};