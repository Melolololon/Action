#include "PlayerSlush.h"
#include<LibMath.h>

void PlayerSlush::SetAttackParam()
{
	//���W��]�����Ă��A���O�����������ĂȂ������]���������Ȃ�?
	//playerAngle���J�v�Z���̊p�x�ɉ��Z���Z����?

	//0�x��]�Ŕ����X����]��100%�A90�x��]��0%�A180��-100%(���Α�)�ɂȂ�΂悢
	
	

	//����̒���
	static const float COLLISION_LENGTH = 5.0f;
	MelLib::Value2<MelLib::Vector3>pos;
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		
		pos.v1 = position + MelLib::Vector3(0, -1, 1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(-4, 4, 2).Normalize(), COLLISION_LENGTH);
		
		
		break;

	case PlayerSlush::AttackType::NORMAL_2:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		pos.v1 = position + MelLib::Vector3(1, -1, 1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(4, -2, 4).Normalize(), COLLISION_LENGTH);
		
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		pos.v1 = position + MelLib::Vector3(0,1,1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(0,5,4).Normalize(), COLLISION_LENGTH);
		
		break;
	default:
		break;
	}

	//��]
	pos.v1 = MelLib::LibMath::RotateVector3(pos.v1, MelLib::Vector3(0, -1, 0), playerAngle);
	pos.v2 = MelLib::LibMath::RotateVector3(pos.v2, MelLib::Vector3(0, -1, 0), playerAngle);

	capsuleData[0].GetRefSegment3DData().SetPosition(pos);
}

void PlayerSlush::Attack()
{
	//�p�x�𒲐����邽�߂̊���
	 //0�x��]�Ŕ����X����]��100%�A90�x��]��0%�A180��-100%(���Α�)�ɂȂ�悤��
	float angleMulPar = sin(MelLib::LibMath::AngleConversion(0, playerAngle + 90));

	//�ړ����Ȃ����]�����邱�ƂŁA���������ɂȂ�?
	//��]�g��Ȃ��Ă����W���΂߂Ɉړ�������Ζ��Ȃ�?
	//�ړ�����������́A�v���C���[�̌����ɉ����ĕς���?(��]������?)
	//����ω�]�̂ق�������?
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		
		//��Y�̋��������~�������ɉe�������ĂȂ������]������Ək��
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(8, 8, 0));

		break;

	case PlayerSlush::AttackType::NORMAL_2:
		

		//��Y�̋��������~�������ɉe�������ĂȂ������]������Ək��
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(0, -8, 0));

		
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		//��Y�̋��������~�������ɉe�������ĂȂ������]������Ək��
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(8, 0, 0));
		break;
	default:
		break;
	}
}

PlayerSlush::PlayerSlush(const MelLib::Vector3& pos, const MelLib::Vector3& playerDir, const AttackType type):
	attackType(type)
{
	//�v���C���[�̌����Ă�����ɉ�]����ׂ̏���
	playerAngle =
		MelLib::LibMath::Vecto2ToAngle(MelLib::Vector2(playerDir.x, playerDir.z), true);
	//����]����0,0,1�Ƃ��Ă�̂ŁA0,0,1��0�x�ɂȂ�悤�ɂ���
	playerAngle -= 90.0f;

	position = pos;
	

	capsuleData.resize(1);
	capsuleData[0].SetRadius(0.2f);


	eraseTimer.SetStopFlag(false);

	SetAttackParam();
}

void PlayerSlush::Update()
{
	if (eraseTimer.GetSameAsMaxFlag())eraseManager = true;
	Attack();
}

void PlayerSlush::Draw()
{
}

void PlayerSlush::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}
