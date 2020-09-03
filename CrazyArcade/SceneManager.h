#pragma once
#include "TitleScene.h"
#include "LobbyScene.h"
#include "GameScene.h"
//#include "GameOverScene.h"

class SceneManager
{
private:
	vector<Scene*> Manager;
	Scene* CurrentScene;
	HDC* FrontBuffer;
	HDC* BackBuffer;

	string IP;
	wstring ID;
	int score;
public:
	SceneManager() {};
	~SceneManager() { Destroy(); };

	Scene* GetInstance() { return CurrentScene; };

	void InitManager(HDC* Front, HDC* Back);
	void Destroy();

	void NextScene();
	void PrevScene();

	void UpdateScore(int input) { score = input; };
	int GetScore() { return score; };
};

