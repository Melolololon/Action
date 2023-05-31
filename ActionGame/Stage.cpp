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
	// �Ȃ���2��Stage0���ǂݍ��܂�Ă�

	const std::string STAGE_NUM_STR = std::to_string(stageNum);
	
	/*bool res = MelLib::ModelData::Load("Resources/Model/Stage/StageGround_" + STAGE_NUM_STR + ".obj", true, "ground" + STAGE_NUM_STR);
	res = MelLib::ModelData::Load("Resources/Model/Stage/Stage_" + STAGE_NUM_STR + ".obj",true,"stage" + STAGE_NUM_STR);
	res = MelLib::ModelData::Load("Resources/Model/Stage/WallCollision_" + STAGE_NUM_STR + ".obj",true,"wallCollision" + STAGE_NUM_STR);*/

	bool res = MelLib::ModelData::Load("Resources/Model/Stage/StageCollision_" + STAGE_NUM_STR + ".obj", false, "stageCollision" + STAGE_NUM_STR);
	res = MelLib::ModelData::Load("Resources/Model/Stage/Stage_" + STAGE_NUM_STR + ".obj", false, "stage" + STAGE_NUM_STR);
}

Stage::Stage(const unsigned int stageNum)
	:GameObject("Stage")
{
	// Gui�̏����Z�b�g
	this->stageNum.SetData(static_cast<int>(stageNum), GetObjectName(), "StageNumber", 0, 1);
	// �f�[�^����������㏑��
	//this->stageNum.SetLoadData();

	const std::string STAGE_NUM_STR = std::to_string(this->stageNum.GetValue());

	collisionCheckDistance = FLT_MAX;


	MelLib::ModelData* pCollision = MelLib::ModelData::Get("stageCollision" + STAGE_NUM_STR);
	if(pCollision)modelObjects["stageCollision"].Create(pCollision, "stageCollision" + STAGE_NUM_STR, nullptr);


	MelLib::ModelData* pStage = MelLib::ModelData::Get("stage" + STAGE_NUM_STR);
	modelObjects["stage"].Create(pStage, "stage" + STAGE_NUM_STR, nullptr);
	
	// �V�F�[�_�[�ݒ�
	MelLib::ShaderDataSet set;
	set.pShaderData = { L"StagePixelShader.hlsl","main","ps_5_0" };

	// �`��ݒ��3D���f���p�̃f�t�H���g�ݒ��K��
	MelLib::DrawOption op;
	op.SetModelDefData();

	// �}�e���A���쐬(�`��ݒ�,�e�N�X�`������,�V�F�[�_�[�̐ݒ�)
	material.Create(op, 3, set);

	// �e�N�X�`�����Z�b�g
	material.SetTexture(MelLib::Texture::Get("Grass"), "Grass");
	material.SetTexture(MelLib::Texture::Get("Rock"), "Rock");
	material.SetTexture(MelLib::Texture::Get("StageMask_1"), "StageMask_1");

	MelLib::ADSAMaterialData mtlData;
	mtlData.ambient = 0.8f;
	mtlData.diffuse = 0.8f;
	material.SetMaterialData(mtlData);

	// �}�e���A�����Z�b�g
	modelObjects["stage"].SetMaterial(&material);

	tags.push_back("Stage");
}

void Stage::Initialize()
{
	std::vector<std::vector<MelLib::TriangleData>>stageCollision;

	// �������b�V�������w�肵�ČĂяo����悤�ɂ���
	modelObjects["stage"].GetModelTriangleData(stageCollision);

	size_t stageTriSize = stageCollision[0].size();
	
	//// �X�e�[�W�ɍ��킹�Ċg�k���Ȃ��Ƃ����Ȃ�
	//std::vector<std::vector<MelLib::TriangleData>>stageColCollision;
	//MelLib::ModelData* pCollision = MelLib::ModelData::Get("stageCollision" + std::to_string(stageNum.GetValue()));
	//if (pCollision) 
	//{
	//	modelObjects["stageCollision"].SetPosition(modelObjects["stage"].GetPosition());
	//	modelObjects["stageCollision"].SetAngle(modelObjects["stage"].GetAngle());
	//	modelObjects["stageCollision"].SetScale(modelObjects["stage"].GetScale());

	//	modelObjects["stageCollision"].GetModelTriangleData(stageColCollision);
	//	resizeNum += stageColCollision[0].size();
	//}

	//for (size_t i = 0; i < stageColCollision[0].size(); i++)
	//{
	//	triangleDatas["main"][i + stageCollision[0].size()] = stageCollision[0][i];
	//}


	//triangleDatas["main"].resize(stageTriSize);


	modelObjects["stageCollision"].SetPosition(modelObjects["stage"].GetPosition());
	modelObjects["stageCollision"].SetAngle(modelObjects["stage"].GetAngle());
	modelObjects["stageCollision"].SetScale(modelObjects["stage"].GetScale());

	std::vector<std::vector<MelLib::TriangleData>> stageWallColl;
	modelObjects["stageCollision"].GetModelTriangleData(stageWallColl);

	// �����ȕǂ̔��萔
	int wallCollNum = stageWallColl[0].size();

	// ����ǉ�
	triangleDatas["main"].reserve(stageCollision[0].size() + wallCollNum);
	for (int i = 0; i < stageTriSize; i++)
	{
		if (stageCollision[0][i].GetNormal().y >= 0.6f)triangleDatas["main"].push_back(stageCollision[0][i]);
	}

	for (int i = 0; i < wallCollNum; i++)
	{
		triangleDatas["main"].push_back(stageWallColl[0][i]);
	}
	
#pragma region ��
	//MelLib::ModelData* pColl = MelLib::ModelData::Get("wallCollision" + std::to_string(STAGE_NUM));

	//// �n�ʂ̎O�p�`���W���擾
	//std::vector<std::vector<MelLib::TriangleData>>groundCollision;
	//modelObjects["ground"].GetModelTriangleData(groundCollision);

	//// �ǂ̎O�p�`�̔�����擾
	//std::vector<std::vector<MelLib::TriangleData>>wallCollision;
	//if (pColl)
	//{
	//	modelObjects["wallCollision"].GetModelTriangleData(wallCollision);
	//}

	//// ���T�C�Y���Ĕ����ǉ�
	//size_t triDataSize = groundCollision[0].size();
	//if (pColl)triDataSize += wallCollision[0].size();

	//triangleDatas["main"].resize(triDataSize);
	//for (int i = 0; i < groundCollision[0].size(); i++)
	//{
	//	triangleDatas["main"][i] = groundCollision[0][i];
	//}
	//if (pColl)
	//{
	//	const size_t GROUND_TRI_SIZE = groundCollision[0].size();
	//	for (int i = GROUND_TRI_SIZE; i < triangleDatas["main"].size(); i++)
	//	{
	//		triangleDatas["main"][i] = wallCollision[0][i - GROUND_TRI_SIZE];
	//	}
	//}

#pragma endregion

}

void Stage::Draw()
{
#pragma region ��


	//if (STAGE_NUM != 0)
	//{
	//	/*std::vector<MelLib::Texture*> p;
	//	modelObjects["stage"].GetPMaterial()->GetPTextures(p);*/
	//	modelObjects["stage"].Draw();

	//	// ����R�����g�A�E�g���ĂȂ��ƕǔ���쐬�O�ɕ\����������O�o�邩�璍��
	//	//modelObjects["wallCollision"].Draw();
	//}
	//modelObjects["ground"].Draw();
#pragma endregion


	modelObjects["stage"].Draw();
	//modelObjects["stageCollision"].Draw();
}

std::shared_ptr<MelLib::GameObject> Stage::GetNewPtr()
{
	return std::make_shared<Stage>(stageNum.GetValue());
}
