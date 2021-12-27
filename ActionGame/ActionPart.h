#pragma once
#include"Scene.h"

#include<Easing.h>
#include<Sprite2D.h>

//テスト用
#include<ModelObject.h>

#include"Enemy.h"

//アクションパートのシーン
class ActionPart :public MelLib::Scene
{
private:

private:
	//テスト用
	MelLib::ModelObject obj;

	// ロックオン用
	static std::vector<std::shared_ptr<Enemy>>pEnemys;
private:
	void LoadResources();

public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン

	static const std::vector<std::shared_ptr<Enemy>>& GetEnemys() { return pEnemys; }
};