#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

#include"Tutorial.h"
//テスト用

#include<ModelObject.h>

#include"Enemy.h"

#include"Stage.h"

//アクションパートのシーン
class ActionPart :public MelLib::Scene
{
private:

private:
	enum class EventType
	{
		NONE,
		TUTORIAL,
		BOSS_MOVIE,
	};
	EventType currentEvent = EventType::TUTORIAL;
	std::vector<EventType>endEvents;

	// プレイヤーのポインタ
	std::shared_ptr<Player>pPlayer;

	//テスト用
	MelLib::ModelObject obj;



	Tutorial tutorial;

	// ロックオン用
	static std::vector<std::shared_ptr<MelLib::GameObject>>pEnemys;
private:
	void LoadResources();
	
	/// <summary>
	/// シーン遷移時のフェード処理呼び出し
	/// </summary>
	void Fade();

	/// <summary>
	/// イベント関係の処理
	/// </summary>
	void Event();

	/// <summary>
	/// イベントが終了済みかどうかの確認。endEventsがすべて終了していたらtrue
	/// </summary>
	/// <param name="checkEvents">終了したかどうか確認するイベント</param>
	/// <returns></returns>
	bool CheckEndEvent(const std::vector<EventType>& checkEvents);

	bool CheckEndEvent(const EventType checkEvent);
public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン

	static const std::vector<std::shared_ptr<MelLib::GameObject>>& GetEnemys() { return pEnemys; }
};