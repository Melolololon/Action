#pragma once
#include<Scene.h>
class StageSelect : public MelLib::Scene
{
private:
	static unsigned int stageNum;
public:
	void Initialize()override;//������
	void Update()override;
	void Draw()override;
	void Finalize()override;//�I������
	Scene* GetNextScene()override;//���̃V�[��

	static unsigned int GetStageNum() { return stageNum; }
};

