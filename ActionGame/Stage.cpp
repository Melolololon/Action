#include "Stage.h"

void Stage::SetStageData(const unsigned int stageNum)
{
	if(stageNum == 1)
	{
		modelObjects["stage"].SetPosition(MelLib::Vector3(0, 10, 0));
		modelObjects["stage"].SetScale(MelLib::Vector3(8, 8, 8));
		modelObjects["stage"].SetAngle(MelLib::Vector3(0, 180, 0));

		modelObjects["ground"].SetPosition(MelLib::Vector3(0, 10, 0));
		modelObjects["ground"].SetScale(MelLib::Vector3(8, 8, 8));
		modelObjects["ground"].SetAngle(MelLib::Vector3(0, 180, 0));
	}
}

void Stage::LoadResources(unsigned int stageNum)
{
	static const std::string STAGE_NUM_STR = std::to_string(stageNum);
	MelLib::ModelData::Load("Resources/Model/Stage/StageGround.obj",true,"ground" + STAGE_NUM_STR);
	MelLib::ModelData::Load("Resources/Model/Stage/Stage.obj",true,"stage" + STAGE_NUM_STR);
}

Stage::Stage(const unsigned int stageNum)
{
	static const std::string STAGE_NUM_STR = std::to_string(stageNum);

	modelObjects["stage"].Create(MelLib::ModelData::Get("stage" + STAGE_NUM_STR), nullptr);
	modelObjects["ground"].Create(MelLib::ModelData::Get("ground" + STAGE_NUM_STR), nullptr);
	
	SetStageData(stageNum);

	// 地面の三角形座標を取得
	 triangleData = modelObjects["ground"].GetModelTriangleData()[0];
	
}

void Stage::Draw()
{
	modelObjects["stage"].Draw();
	modelObjects["ground"].Draw();
}
