#include "stdafx.h"
#include "Block.h"

Block::Block()
{
	Pos = { -1, -1 };
	Pivot = { 0, 0 };
}

Block::~Block()
{
}

void Block::Init(bool Passable, bool Destructible, POINT pos, POINT pivot)
{
	//hImage = (HBITMAP)LoadImage(NULL, Path.c_str(), IMAGE_BITMAP,
	//	0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//GetObject(hImage, sizeof(BITMAP), &bitImage);
	Pivot = pivot;
	Pos = pos;

	Pos.x -= Pivot.x;
	Pos.y -= Pivot.y;

	IsPassable = Passable;
	IsDestructible = Destructible;
}

void Block::SetImage(HBITMAP & image, BITMAP & bitmap)
{
	hImage = image;
	bitImage = bitmap;

	Sprite_Size.x = bitImage.bmWidth;
	Sprite_Size.y = bitImage.bmHeight;

	Anim_Frame_Max = bitImage.bmWidth / Sprite_Size.x - 1;
	Anim_Frame_Min = 0;
	Anim_Frame_Cur = Anim_Frame_Min;
	Anim_Frame_Flag = 0;

	Start.x = Anim_Frame_Cur * Sprite_Size.x;
	Start.y = Anim_Frame_Flag * Sprite_Size.y;
}

void Block::Render(HDC front, HDC back)
{
	HBITMAP oldbuffer = (HBITMAP)SelectObject(back, hImage);

	TransparentBlt(front, Pos.x, Pos.y, Sprite_Size.x, Sprite_Size.y,
		back, Start.x, Start.y, Sprite_Size.x, Sprite_Size.y, FILTER);

	SelectObject(back, oldbuffer);
	DeleteObject(oldbuffer);
}

void Block::UpdateFrame()
{

}
