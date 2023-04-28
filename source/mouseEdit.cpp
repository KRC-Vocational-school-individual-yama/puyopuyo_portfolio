#include "mouseEdit.h"
#include"libs/scene.h"
#include"board.h"
MouseEdit::MouseEdit(Scene* sc)
	:Object(sc)
	,number(0)
	,mouseWheel(0)
	,mouse(Vec2::zero())
{
}

MouseEdit::~MouseEdit(){

}

void MouseEdit::Init(){
	
}

void MouseEdit::Update(){
	mouseWheel += GetMouseWheelRotVol();
	GetMousePoint(&mouse.x,&mouse.y);

	if (GetMouseInput() & MOUSE_INPUT_MIDDLE)
		mouseWheel = 0;

	BoardGetMouseMiddle();
}

void MouseEdit::Draw(){
	printfDx("wheel= %d\n",mouseWheel);



	int color = GetWheelColor();
	bool mouseDown = GetMouseInput() & MOUSE_INPUT_LEFT;

	if (color > -1 && !mouseDown) {
		int graph = GetScene()->Invoke<Board>()->GethImages(color);
		int size = Board::PUYO_SIZE / 1.1;
		Vec2 dispPos = mouse - Vec2{ size / 2,size / 2 };

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 / 1.1);
		DrawExtendGraph(dispPos.x, dispPos.y, dispPos.x + size, dispPos.y + size, graph, TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}

int MouseEdit::GetWheelColor(){
	int sendColor = mouseWheel;
	int maxColor = GetScene()->Invoke<Board>()->GetColorMax();

	sendColor = min(sendColor, maxColor+1);
	sendColor = max(sendColor, -1);

	if (sendColor == (maxColor + 1))
		sendColor = GetRand(maxColor);

	return sendColor;
}

bool MouseEdit::GetMousePointEdhit(int* x, int* y) {
	int& xPointer = (*x);
	int& yPointer = (*y);

	Vec2 boardFastPos = { Board::BOARD_FAST_X ,Board::BOARD_FAST_Y };
	Vec2 PuyoSize = { Board::PUYO_SIZE ,Board::PUYO_SIZE };
	Vec2 ans = mouse - boardFastPos;
	
	if (ans.x < 0)
		ans.x = -PuyoSize.x;
	if (ans.y < 0)
		ans.y = -PuyoSize.y;

	ans /= PuyoSize;

	if (x) {//nullptr !=
		xPointer = ans.x;

		if (xPointer >= Board::BOARD_WIDTH)
			return false;

		if (xPointer < 0)
			return false;
	}

	if (y) {//nullptr !=
		yPointer = ans.y;

		if (yPointer >= Board::BOARD_HEIGHT)
			return false;

		if (yPointer < 0)
			return false;
	}
	return true;
}

void MouseEdit::BoardGetMouseMiddle(){
	if ( !(GetMouseInput() & MOUSE_INPUT_MIDDLE))
		return;
	Vec2 math = Vec2::zero();
	bool rangeOut = GetMousePointEdhit(&math.x, &math.y);

	//分岐----------------------------

	if (rangeOut) {
		if (!CheckClass<Board>())
			return;
	
		std::vector<int> puyos = GetScene()->Invoke<Board>()->GetBoardData();
		int index = Board::GetIndex(math.x, math.y);

		mouseWheel = puyos.at(index);
		return;
	}

	//分岐-----------------------------

	mouseWheel = 0;
}

bool MouseEdit::GetPuyoNumber(int& color, int& number){
		Vec2 math = Vec2::zero();
		bool rangeOut=GetMousePointEdhit(&math.x, &math.y);

		//範囲外の対処
		if (!rangeOut) //falseだったら
			return false;
		
		int index = Board::GetIndex(math.x , math.y);

		//クリック時、色を変更
		if (GetMouseInput() & MOUSE_INPUT_LEFT) {
			//クリックしても無反応
			int sendColor = GetWheelColor();
			if (sendColor < 0)
				return false;

			color = sendColor;
			
			number = index;
			return true;
		}
		//右クリックは消す
		if (GetMouseInput() & MOUSE_INPUT_RIGHT) {
			color  = -1;
			number = index;
			return true;
		}
	
	return false;
}
