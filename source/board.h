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

	//主にedit系で使ってる関数

	[[nodiscard]] int GetColorMax() { return static_cast<int>(Board::PuyoColor::max) - 1; }
	[[nodiscard]] std::vector<std::string> GetBoardData_String();
	[[nodiscard]] const std::vector<int> GetBoardData();
	void SetBoardData(std::vector<int>color);
	void SetBoardPos(int color,int index);
	void SetBoardPos(int color, int x, int y);
	[[nodiscard]] int GethImages(int num) { return hImage.at(num); }

	//主にsequenceで使う関数

	[[nodiscard]]static int GetIndex(int x, int y) { return (x + y * BOARD_WIDTH); }
	void SetLabeling();
	void PuyoFoll();//ぷよ落とす
	bool PuyoErase();//ぷよ消す
	void Reset();

	//主にplayerで使う関数

	//1つのぷよだけで確認する ・trueなら空白マス ・falseならboard外 か ぷよがある
	bool IsEmptyGrid(int x,int y);
	//ゲームオーバー判定 ・trueあうと　false　セーフ
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

	//全て落ちたなら抜け出す
	bool PuyoFoll_Method();

	void Labeling();
	void UpMatch(std::vector<int>& deleteList, int& x, int& y, int& i, int& target, int& targetLeft);
	void Labeling_Method(std::vector<int>&deleteList,int& massNumber);
	
	
	void Recursion();
	void RecursionInLabeling(int index, std::vector<int>& sell, int& labelCount, std::vector<int>&labelNum);
	int GetBoardPuyo(int index);
	int GetBoardPuyo(int x, int y) { return GetBoardPuyo(GetIndex(x, y)); }


};