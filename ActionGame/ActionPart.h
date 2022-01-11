#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//テスト用
#include<ModelObject.h>

#include"Enemy.h"

#include"Stage.h"

//アクションパートのシーン
class ActionPart :public MelLib::Scene
{
private:

private:
	// プレイヤーのポインタ
	std::shared_ptr<Player>pPlayer;

	//テスト用
	MelLib::ModelObject obj;

	//MelLib::ModelObject stage;
	std::shared_ptr<Stage>stage;

	// ロックオン用
	static std::vector<std::shared_ptr<MelLib::GameObject>>pEnemys;
private:
	void LoadResources();
	void Fade();
public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン

	static const std::vector<std::shared_ptr<MelLib::GameObject>>& GetEnemys() { return pEnemys; }
};