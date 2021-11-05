#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//テスト用
#include<ModelObject.h>

//アクションパートのシーン
class ActionPart :public MelLib::Scene
{
private:

private:
	//テスト用
	MelLib::ModelObject obj;

#pragma region ポーズ
	// 一気にメニューを表示せずに、少しづつアルファ値を上げて表示すること

	static bool isPause;
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
		static const int RETURN_TITLE = 3;
		static const int NUM_MAX = 3;
	};

	// ポーズ関係のスプライトのアルファ値
	MelLib::Easing<float>pauseSubAlpha;
	MelLib::Easing<float>pauseBackSubAlpha;

	// これObject2Dにしてもいいかも
	// ポーズ中の背景
	MelLib::Sprite2D pauseBackSpr;
#pragma endregion

	bool isOption;

#pragma region エディット関係
	bool editOn = true;
	static bool isEdit;

#pragma endregion

private:
	void LoadResources();

	void PauseUpdate();
	void PauseDraw();

public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン

	static bool GetStopFlag() { return isPause || isEdit; }
};

