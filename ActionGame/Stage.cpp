#include "Stage.h"

void Stage::SetStageData(const unsigned int stageNum)
{
	MelLib::Vector3 stagePosition;
	MelLib::Vector3 stageScale = 1;
	MelLib::Vector3 stageAngle;
	if(stageNum == 1)
	{
		stagePosition = MelLib::Vector3(0, 10, 0);
		stageScale = MelLib::Vector3(8, 8, 8);
		stageAngle = MelLib::Vector3(0, 180, 0);
	}

	for (auto& object : modelObjects)
	{
		object.second.SetPosition(stagePosition);
		object.second.SetScale(stageScale);
		object.second.SetAngle(stageAngle);
	}

}

void Stage::LoadResources(unsigned int stageNum)
{
	static const std::string STAGE_NUM_STR = std::to_string(stageNum);
	
	MelLib::ModelData::Load("Resources/Model/Stage/StageGround_" + STAGE_NUM_STR + ".obj", true, "ground" + STAGE_NUM_STR);
	//MelLib::ModelData::Load("Resources/Model/Stage/Stage_" + STAGE_NUM_STR + ".obj",true,"stage" + STAGE_NUM_STR);
	//MelLib::ModelData::Load("Resources/Model/Stage/WallCollision_" + STAGE_NUM_STR + ".obj",true,"wallCollision" + STAGE_NUM_STR);
}

Stage::Stage(const unsigned int stageNum)
	:GameObject("Stage")
	, STAGE_NUM(stageNum)
{
	static const std::string STAGE_NUM_STR = std::to_string(STAGE_NUM);

	//modelObjects["stage"].Create(MelLib::ModelData::Get("stage" + STAGE_NUM_STR),"Stage", nullptr);
	modelObjects["ground"].Create(MelLib::ModelData::Get("ground" + STAGE_NUM_STR), "StageGround", nullptr);
	//modelObjects["wallCollision"].Create(MelLib::ModelData::Get("wallCollision" + STAGE_NUM_STR), "WallCollision", nullptr);

	SetStageData(STAGE_NUM);

	// 地面の三角形座標を取得
	std::vector<MelLib::TriangleData>groundCollision = modelObjects["ground"].GetModelTriangleData()[0];
	
	// 壁の三角形の判定を取得
	//std::vector<MelLib::TriangleData>wallCollision = modelObjects["wallCollision"].GetModelTriangleData()[0];

	// リサイズして判定を追加
	triangleDatas["main"].resize(groundCollision.size()/* + wallCollision.size()*/);
	for (int i = 0; i < groundCollision.size(); i++)
	{
		triangleDatas["main"][i] = groundCollision[i];
	}
	//for (int i = groundCollision.size(); i < triangleDatas.size(); i++)
	//{
	//	triangleDatas["main"][i] = wallCollision[i - groundCollision.size()];
	//}
}

void Stage::Draw()
{
	//modelObjects["stage"].Draw();
	modelObjects["ground"].Draw();
}

std::shared_ptr<MelLib::GameObject> Stage::GetNewPtr()
{
	return std::make_shared<Stage>(STAGE_NUM);
}
