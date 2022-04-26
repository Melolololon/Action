#include "StageObject.h"

#include<GameObjectManager.h>
#include<LibMath.h>

#include"PlayerSlush.h"



Player* StageObject::pPlayer;

MelLib::PlaneData StageObject::SegmentToPlane()
{
	// moveVectorY�̊p�x�ϊ��͊֐�������ق�������
	// (���ƁA���̂�������_���B(0.7�̎��A63�x�Ƃ��ɂȂ�))

#pragma region �e�X�g�p
	MelLib::Vector3 normal(0, 1, 0);
	MelLib::Vector3 moveVector = (MelLib::Vector3(1, 1, 0) - MelLib::Vector3(0, 0, 0)).Normalize();


	// x��z���l������ꍇ�A�ǂ����邩
	// ��Ƀv���C���[����(z��+����)�������Ă�Ɖ��肵�ĉ�]�����āA���̂��ƃv���C���[�̌�������]������?
	// �������p�x�ɕϊ����āA��]������΂�������?

	// moveVector.y���p�x�ɕϊ�
	float angle = moveVector.y * 90.0f;

	// �@������]
	normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, 0, -1), angle);

	// �v���C���[�̌������擾
	MelLib::Vector3 playerDirection = MelLib::Vector3(1, 0, 0);
	float playerDirAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerDirection.x, playerDirection.z), true);

	// z+�������Ă�Ƃ��͉�]������K�v�Ȃ�����-90;
	playerDirAngle -= 90;

	// �����ɍ��킹�čX�ɉ�]
	normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, -1, 0), playerDirAngle);

	// �U�������ɕ��ʂ̏����쐬
	MelLib::PlaneData playerSlushPlane;

	// �Փ˓_����
	playerSlushPlane.SetPosition(capsuleDatas[0].GetRefSegment3DData().GetCalcResult().lineSegment3DHitPos);

	playerSlushPlane.SetNormal(normal);

	return playerSlushPlane;
#pragma endregion


	//// Y�̈ړ������Ŗ@������]������΂�����?

	//PlayerSlush* pPlayerSlush = pPlayer->GetPPlayerSlush();

	//// ��������̐�[�̍��W�����ł���������
	//MelLib::Value2<MelLib::Vector3>sgmentPosition = pPlayerSlush->GetSegmentPosition();
	//MelLib::Value2<MelLib::Vector3>sgmentPrePosition = pPlayerSlush->GetSegmentPosition();

	//MelLib::Vector3 normal(0, 1, 0);
	//MelLib::Vector3 moveVector = (sgmentPosition.v2 - sgmentPrePosition.v2).Normalize();
	//

	//// x��z���l������ꍇ�A�ǂ����邩
	//// ��Ƀv���C���[����(z��+����)�������Ă�Ɖ��肵�ĉ�]�����āA���̂��ƃv���C���[�̌�������]������?
	//// �������p�x�ɕϊ����āA��]������΂�������?

	//// moveVector.y���p�x�ɕϊ�
	//float angle = moveVector.y * 90.0f;

	//// �@������]
	//normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, 0, -1), angle);

	//// �v���C���[�̌������擾
	//MelLib::Vector3 playerDirection = pPlayer->GetDirection();
	//float playerDirAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerDirection.x, playerDirection.z), true);

	//// z+�������Ă�Ƃ��͉�]������K�v�Ȃ�����-90;
	//playerDirAngle -= 90;

	//// �����ɍ��킹�čX�ɉ�]
	//normal = MelLib::LibMath::RotateVector3(normal, MelLib::Vector3(0, -1, 0), playerDirAngle);

	//// �U�������ɕ��ʂ̏����쐬
	//MelLib::PlaneData playerSlushPlane;

	//// �Փ˓_����
	//playerSlushPlane.SetPosition(capsuleData[0].GetRefSegment3DData().GetCalcResult().lineSegment3DHitPos);

	//playerSlushPlane.SetNormal(normal);

	//return playerSlushPlane;
}

void StageObject::MeshCat(const MelLib::GameObject* const object, MelLib::ModelData*& pFront, MelLib::ModelData*& pBack)
{
	// �v���C���[�̕���ƐڐG������AMeshCat�֐����Ăяo��
	if (typeid(*object) == typeid(PlayerSlush))
	{
		modelObjects["main"].MeshCat(SegmentToPlane(), pFront, pBack, true);

		// GameObject�̒ǉ��͊��N���X�ł��Ȃ��Ƃ����Ȃ�
	}
}

StageObject::StageObject(const MelLib::Vector3& pos, const std::string& modelName)
{

	SetPosition(pos);

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


	// �e�X�g�p
	//SegmentToPlane();


	tags.push_back("StageObject");
}

StageObject::StageObject(const MelLib::Vector3& pos, MelLib::ModelData* pCatModelData)
{
	modelObjects["main"].Create(pCatModelData, nullptr);
	modelObjects["main"].SetPosition(pos);
	SetPosition(pos);

	tags.push_back("StageObject");
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
