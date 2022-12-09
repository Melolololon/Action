#pragma once
#include<Scene.h>
class StageSelect : public MelLib::Scene
{
private:
	static unsigned int stageNum;
public:
	void Initialize()override;//‰Šú‰»
	void Update()override;
	void Draw()override;
	void Finalize()override;//I—¹ˆ—
	Scene* GetNextScene()override;//Ÿ‚ÌƒV[ƒ“

	static unsigned int GetStageNum() { return stageNum; }
};

