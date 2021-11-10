#include "StageObject.h"

#include<GameObjectManager.h>

#include"PlayerSlush.h"



Player* StageObject::pPlayer;

void StageObject::MeshCat(const MelLib::GameObject* const object, MelLib::ModelData*& pFront, MelLib::ModelData*& pBack)
{
	// �v���C���[�̕���ƐڐG������AMeshCat�֐����Ăяo��
	if (typeid(*object) == typeid(PlayerSlush))
	{
		PlayerSlush* pPlayerSlush = pPlayer->GetPPlayerSlush();

		// �U�������ɕ��ʂ̏����쐬
		MelLib::PlaneData playerSlushPlane;


		modelObjects["main"].MeshCat(playerSlushPlane, pFront, pBack, true);

		// GameObject�̒ǉ��͊��N���X�ł��Ȃ��Ƃ����Ȃ�
	}
}

StageObject::StageObject(const MelLib::Vector3& pos, const std::string& modelName)
{
	modelObjects["main"];
	if (modelName == "")
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),nullptr);

		modelObjects["main"].SetScale(3);
	}
	else
	{
		modelObjects["main"].Create(MelLib::ModelData::Get(modelName), nullptr);
	}
	modelObjects["main"].SetPosition(pos);
	SetPosition(pos);
}

StageObject::StageObject(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData)
{
	modelObjects["main"];
	modelObjects["main"].Create(pCatModelData, nullptr);
	modelObjects["main"].SetPosition(pos);
	SetPosition(pos);
}

StageObject::~StageObject()
{
}

void StageObject::Update()
{
}

void StageObject::Draw()
{
	modelObjects["main"].Draw();
}

void StageObject::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{	
	
}
