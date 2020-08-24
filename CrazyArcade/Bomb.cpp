#include "stdafx.h"
#include "Player.h"
#include "CollisionManager.h"
#include "Bomb.h"

extern Singleton* singleton;

Bomb::Bomb(int Owner, POINT pos, int power)
{
	AnimObject::Init(TEXT("images/effect/Popo.bmp"), RePosition(pos), { -26, -52 });

	Sprite_Size.x = bitImage.bmWidth / 3;
	Sprite_Size.y = bitImage.bmHeight;
	ImageScale = 1.2f;

	AnimObject::InitAnimation();
	Anim_Speed = 180;

	AnimObject::InitCollider({ 26, 24 }, 36);
	IsColliderActive = false;

	Power = power;
	PlayerNum = Owner;
	IsDetonated = false;
	Timer = 0;
}

Bomb::~Bomb()
{
	for (AnimObject* Wave : BombWaves)
	{
		delete Wave;
	}

	BombWaves.clear();
}

void Bomb::Update()
{
	Timer += ElapseTime;

	if (Timer > DETONATE_TIME)
	{
		Timer = 0;
		Explosion();
	}

	Start.x = Anim_Frame_Cur * Sprite_Size.x;
	Start.y = Anim_Frame_Flag * Sprite_Size.y;

	UpdateFrame();

	if (!BombWaves.empty())
		for (AnimObject* Waves : BombWaves)
		{
			Waves->Update();
			if (RRCollision(&PLAYER->GetCollider(), &Waves->GetCollider()))
				cout << "Player Hit!" << endl;

			Block* blocks = BLOCKS;

			for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++)
			{
				if (!blocks[i].GetColliderState() || !blocks[i].GetDestructible())
					continue;

				RECT BlockArea = blocks[i].GetCollider();

				if (RRCollision(& Waves->GetCollider(), &BlockArea))
				{
					blocks[i].SetPos({ -1, -1 });
					blocks[i].SetColliderState(false);
				}
			}
		}
}

void Bomb::UpdateFrame()
{
	Anim_Timer += ElapseTime;

	if (Anim_Timer > Anim_Speed)
	{
		Anim_Frame_Cur++;
		Anim_Timer = 0;
		IsColliderActive = true;
	}

	if (Anim_Frame_Cur > Anim_Frame_Max)
		Anim_Frame_Cur = Anim_Frame_Min;
}

void Bomb::Render(HDC front, HDC back, bool colliderdraw)
{
	AnimObject::Render(front, back, colliderdraw);

	if (!BombWaves.empty())
		for (AnimObject* Waves : BombWaves)
		{
			Waves->Render(front, back, colliderdraw);
			if (Timer >= WAVE_ANIM_SPEED * 14)
				IsDetonated = true;
		}
}

// Set In Tile
POINT Bomb::RePosition(POINT pos)
{
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		if (i * 52 < pos.x && pos.x < (i + 1) * 52)
		{
			pos.x = i * 52;
			break;
		}
	}

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		if (i * 52 < pos.y && pos.y < (i + 1) * 52)
		{
			pos.y = i * 52;
			break;
		}
	}

	return pos;
}

void Bomb::Explosion()
{
	// Wave Image Flag Index
	// Flag 0: Up, 1: Down, 2: Left, 3: Right, 4: Center
	for (int Flag = 0; Flag < 5; Flag++)
	{
		for (int i = 0; i < Power; i++)
		{
			// Skiped Details [Update Later]
			InitWave(i, Flag);
		}
	}
}

void Bomb::InitWave(int index, int flag)
{
	POINT WavePos = Pos;
	if (flag == 0) WavePos = { Pos.x, Pos.y - 52 * (index + 1) };
	else if (flag == 1) WavePos = { Pos.x, Pos.y + 52 * (index + 1) };
	else if (flag == 2) WavePos = { Pos.x - 52 * (index + 1), Pos.y };
	else if (flag == 3) WavePos = { Pos.x + 52 * (index + 1), Pos.y };

	AnimObject* Wave = new AnimObject;
	Wave->Init(TEXT("images/effect/Explosion.bmp"),
				WavePos, { 0, 0 });
	Wave->InitAnimation(3, 4, 14, 5, flag);
	Wave->SetAnimSpeed(WAVE_ANIM_SPEED);
	Wave->SetImageSize(1.3f);
	Wave->InitCollider({ 26, 24 }, 36);
	BombWaves.push_back(Wave);
}
