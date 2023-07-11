//#pragma once
//#include"Scene.h"
//
//#include<Easing.h>
//#include<Sprite2D.h>
//#include<FrameTimer.h>
//
//#include"Tutorial.h"
//
////テスト用
//#include<ModelObject.h>
//
//
//#include"Enemy.h"
//
//#include"Stage.h"
//
////アクションパートのシーン
//class ActionPart :public MelLib::Scene
//{
//private:
//
//private:
//	enum class GameState 
//	{
//		NOT_PLAYED,
//		PLAY,
//		CLEAR,
//		GAMEOVER,
//	};
//	GameState currentState = GameState::NOT_PLAYED;
//	
//	enum class EventType
//	{
//		NONE,
//		TUTORIAL,
//		BOSS_MOVIE,
//	};
//	EventType currentEvent = EventType::TUTORIAL;
//	std::vector<EventType>endEvents;
//
//	// プレイヤーのポインタ
//	std::shared_ptr<Player>pPlayer;
//
//	//テスト用
//	MelLib::ModelObject obj;
//
//	Tutorial tutorial;
//
//	// 敵の配列(ロックオンなどに利用)
//	static std::vector<std::shared_ptr<Enemy>>pEnemys;
//	std::vector<std::shared_ptr<Enemy>>pDeadEnemys;
//
//	// 敵を倒さないと消えない壁
//	std::vector<std::shared_ptr<MelLib::GameObject>>pWalls;
//
//	// チュートリアル開始するまでの時間を計測するタイマー
//	MelLib::FrameTimer tutorialStartTimer;
//
//	// ゲームクリア判定になってからどのくらいで終了するかの時間を計測するタイマー
//	MelLib::FrameTimer gameClearTimer;
//	// クリアのアニメーションの開始時間
//	const int CLEAR_ANIMATION_START_TIME = 60 * 1;
//
//	MelLib::Sprite2D gameoverBackSpr;
//	MelLib::Sprite2D gameoverTextSpr;
//	MelLib::Easing<float> gameoverBackAlpha = MelLib::Easing<float>(0, 100, 1, 0);
//	float gameoverTextAlpha = 100;
//
//	MelLib::Sprite2D gameoverRetrySpr;
//	MelLib::Sprite2D gameoverReturnTitleSpr;
//
//
//	MelLib::Sprite2D gameClearTextSpr;
//	MelLib::Easing<float>gameClearTextScale = MelLib::Easing<float>(20, 1, 3, 20);
//
//	bool returnTitleFlag = false;
//
//	static bool loadTexture;
//private:
//	void LoadResources();
//	
//	/// <summary>
//	/// シーン遷移時のフェード処理呼び出し
//	/// </summary>
//	void Fade();
//
//	/// <summary>
//	/// イベント関係の処理
//	/// </summary>
//	void Event();
//
//	/// <summary>
//	/// イベントが終了済みかどうかの確認。endEventsがすべて終了していたらtrue
//	/// </summary>
//	/// <param name="checkEvents">終了したかどうか確認するイベント</param>
//	/// <returns></returns>
//	bool CheckEndEvent(const std::vector<EventType>& checkEvents);
//
//	bool CheckEndEvent(const EventType checkEvent);
//
//	void GameClear();
//	void GameOver();
//public:
//	void Initialize()override;//初期化
//	void Update()override;
//	void Draw()override;
//	void Finalize()override;//終了処理
//	Scene* GetNextScene()override;//次のシーン
//
//	static const std::vector<std::shared_ptr<Enemy>>& GetEnemys() { return pEnemys; }
//};