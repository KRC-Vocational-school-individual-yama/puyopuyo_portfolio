#pragma once
#include"libs/object.h"
#include<unordered_map>

class Board :public Object {
public:
	Board(Scene* sc);
	~Board()		override;
	void Init()		override;
	void Update()	override;
	void Draw()		override;

	static const int BOARD_WIDTH = 6;
	static const int BOARD_HEIGHT = 12;
	static const int PUYO_SIZE = 64;
	static const int BOARD_FAST_X = 100;
	static const int BOARD_FAST_Y = 100-PUYO_SIZE;

	//���edit�n�Ŏg���Ă�֐�

	[[nodiscard]] int GetColorMax() { return static_cast<int>(Board::PuyoColor::max) - 1; }
	[[nodiscard]] std::vector<std::string> GetBoardData_String();
	[[nodiscard]] const std::vector<int> GetBoardData();
	void SetBoardData(std::vector<int>color);
	void SetBoardPos(int color,int index);
	void SetBoardPos(int color, int x, int y);
	[[nodiscard]] int GethImages(int num) { return hImage.at(num); }

	//���sequence�Ŏg���֐�

	[[nodiscard]]static int GetIndex(int x, int y) { return (x + y * BOARD_WIDTH); }
	void SetLabeling();
	void PuyoFoll();//�Ղ旎�Ƃ�
	bool PuyoErase();//�Ղ����
	void Reset();

	//���player�Ŏg���֐�

	//1�̂Ղ悾���Ŋm�F���� �Etrue�Ȃ�󔒃}�X �Efalse�Ȃ�board�O �� �Ղ悪����
	bool IsEmptyGrid(int x,int y);
	//�Q�[���I�[�o�[���� �Etrue�����Ɓ@false�@�Z�[�t
	[[nodiscard]]bool GameOver();
private:
	struct Pos {
		float x, y;
	};
	struct Puyo {
		int color=-1;
		Pos move = {0,0};
	};
	std::vector<Puyo>puyos;
	std::vector<int>hImage;
	enum class PuyoColor {
		non = -1
		,green = 0
		,blue
		,purple
		,red
		,yellow
		,max
	};
	std::vector<int>labelList;
	std::vector<int>labelCountList;
	int fontHandle;

	//�S�ė������Ȃ甲���o��
	bool PuyoFoll_Method();

	void Labeling();
	void UpMatch(std::vector<int>& deleteList, int& x, int& y, int& i, int& target, int& targetLeft);
	void Labeling_Method(std::vector<int>&deleteList,int& massNumber);
	
	
	void Recursion();
	void RecursionInLabeling(int index, std::vector<int>& sell, int& labelCount, std::vector<int>&labelNum);
	int GetBoardPuyo(int index);
	int GetBoardPuyo(int x, int y) { return GetBoardPuyo(GetIndex(x, y)); }


};