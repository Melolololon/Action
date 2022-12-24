#include "Stage.h"

void Stage::SetStageData(const unsigned int stageNum)
{
	MelLib::Vector3 stagePosition;
	MelLib::Vector3 stageScale = 1;
	MelLib::Vector3 stageAngle;
	if(stageNum == 1)
	{
	/*	stagePosition = MelLib::Vector3(0, 10, 0);
		stageScale = MelLib::Vector3(8, 8, 8);
		stageAngle = MelLib::Vector3(0, 180, 0);*/
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
	const std::string STAGE_NUM_STR = std::to_string(stageNum);
	
	MelLib::ModelData::Load("Resources/Model/Stage/StageGround_" + STAGE_NUM_STR + ".obj", true, "ground" + STAGE_NUM_STR);
	MelLib::ModelData::Load("Resources/Model/Stage/Stage_" + STAGE_NUM_STR + ".obj",true,"stage" + STAGE_NUM_STR);
	MelLib::ModelData::Load("Resources/Model/Stage/WallCollision_" + STAGE_NUM_STR + ".obj",true,"wallCollision" + STAGE_NUM_STR);
}

Stage::Stage(const unsigned int stageNum)
	:GameObject("Stage")
	, STAGE_NUM(stageNum)
{
	const std::string STAGE_NUM_STR = std::to_string(STAGE_NUM);

	MelLib::ModelData* pGround = MelLib::ModelData::Get("ground" + STAGE_NUM_STR);
	modelObjects["ground"].Create(pGround, "StageGround" + STAGE_NUM_STR, nullptr);

	MelLib::ModelData* pStage = MelLib::ModelData::Get("stage" + STAGE_NUM_STR);
	if(pStage)modelObjects["stage"].Create(pStage,"Stage" + STAGE_NUM_STR, nullptr);

	MelLib::ModelData* pColl = MelLib::ModelData::Get("wallCollision" + STAGE_NUM_STR);
	if (pColl)modelObjects["wallCollision"].Create(pColl, "WallCollision" + STAGE_NUM_STR, nullptr);

	SetStageData(STAGE_NUM);

	
	collisionCheckDistance = FLT_MAX;
}

void Stage::Initialize()
{
	MelLib::ModelData* pColl = MelLib::ModelData::Get("wallCollision" + std::to_string(STAGE_NUM));
	
	// 地面の三角形座標を取得
	std::vector<std::vector<MelLib::TriangleData>>groundCollision;
	modelObjects["ground"].GetModelTriangleData(groundCollision);

	// 壁の三角形の判定を取得
	std::vector<std::vector<MelLib::TriangleData>>wallCollision;
	if (pColl)
	{
		modelObjects["wallCollision"].GetModelTriangleData(wallCollision);
	}

	// リサイズして判定を追加
	size_t triDataSize = groundCollision[0].size();
	if (pColl)triDataSize += wallCollision[0].size();

	triangleDatas["main"].resize(triDataSize);
	for (int i = 0; i < groundCollision[0].size(); i++)
	{
		triangleDatas["main"][i] = groundCollision[0][i];
	}
	if (pColl)
	{
		const size_t GROUND_TRI_SIZE = groundCollision[0].size();
		for (int i = GROUND_TRI_SIZE; i < triangleDatas["main"].size(); i++)
		{
			triangleDatas["main"][i] = wallCollision[0][i - GROUND_TRI_SIZE];
		}
	}

}

void Stage::Draw()
{
	if (STAGE_NUM != 0)
	{
		/*std::vector<MelLib::Texture*> p;
		modelObjects["stage"].GetPMaterial()->GetPTextures(p);*/
		modelObjects["stage"].Draw();
		modelObjects["wallCollision"].Draw();
	}
	modelObjects["ground"].Draw();

}

std::shared_ptr<MelLib::GameObject> Stage::GetNewPtr()
{
	return std::make_shared<Stage>(STAGE_NUM);
}
