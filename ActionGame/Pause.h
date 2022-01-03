#pragma once
#include<Easing.h>
#include<Sprite2D.h>

class Pause
{
private:
	bool loadResource = false;

	bool isPause = false;

	// ポーズが終わったか(終了処理を行うか)
	bool pauseEnd = false;

	//現在選ばれている項目
	int currentPauseSelect = 0;
	//ボタンを押されて選ばれた項目
	int pushPauseSelect = 0;
	// ポーズ中の項目
	struct PauseMenu
	{
		static const int PAUSE_END = 0;
		static const int RESTART = 1;
		static const int OPTION = 2;
		static const int CHECK_OPERATION = 3;
		static const int RETURN_TITLE = 4;
		static const int NUM_MAX = 4;
	};
	// ポーズメニューのスプライトの大きさ
	std::unordered_map<int, MelLib::Sprite2D>menuStringSprites;

	// 非選択時の項目の大きさ
	const MelLib::Vector2 UN_SELECTED_SCALE = MelLib::Vector2(1, 1);
	// 選択した項目の大きさ
	const MelLib::Vector2 SELECT_SCALE = UN_SELECTED_SCALE * 1.2f;


	// ポーズ関係のスプライトのアルファ値
	MelLib::Easing<float>pauseSubAlpha;
	MelLib::Easing<float>pauseBackSubAlpha;

	// これObject2Dにしてもいいかも
	// ポーズ中の背景
	MelLib::Sprite2D pauseBackSpr;
	MelLib::Sprite2D pauseStringSpr;


private:
	Pause(){}
	~Pause(){}

	void ActionPartPauseUpdate();
	void ActionPartPauseDraw();

	void CreateSprite();
public:
	Pause(Pause& p) = delete;
	Pause& operator=(Pause& p) = delete;
	static Pause* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsPause() { return isPause; }
};

