#pragma once
#include<Scene.h>
class StageSelect : public MelLib::Scene
{
private:
	static unsigned int stageNum;
public:
	void Initialize()override;//初期化
	void Update()override;
	void Draw()override;
	void Finalize()override;//終了処理
	Scene* GetNextScene()override;//次のシーン

	static unsigned int GetStageNum() { return stageNum; }
};

