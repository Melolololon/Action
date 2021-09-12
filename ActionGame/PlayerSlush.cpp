#include "PlayerSlush.h"
#include<LibMath.h>

void PlayerSlush::SetAttackParam()
{
	//����̒���(����̒���)
	static const float COLLISION_LENGTH = 5.0f;

	//v1�ɉ��Z������W
	MelLib::Vector3 v1AddPos;
	//v2�̈ړ�����
	MelLib::Vector3 v2MoveVector;
	
	const float PLAYER_ANGLE = playerAngle;
	auto rotSegmentPosData = [&v1AddPos, &v2MoveVector,&PLAYER_ANGLE]()
	{
		const float V1_ADD_POS_Y = v1AddPos.y;
		v1AddPos = 
			MelLib::LibMath::RotateVector3(v1AddPos, MelLib::Vector3(0, 1, 0), PLAYER_ANGLE);
		v1AddPos.y = V1_ADD_POS_Y;

		const float V2_MOVE_VECTOR_Y = v2MoveVector.y;
		v2MoveVector =
			MelLib::LibMath::RotateVector3(v2MoveVector, MelLib::Vector3(0, 1, 0), PLAYER_ANGLE);
		v2MoveVector.y = V2_MOVE_VECTOR_Y;
	};

	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		
		v1AddPos = MelLib::Vector3(0, -1, 1);
		v2MoveVector = MelLib::Vector3(-4, 4, 2).Normalize();

		break;

	case PlayerSlush::AttackType::NORMAL_2:

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		v1AddPos = MelLib::Vector3(1, -1, 1);
		v2MoveVector = MelLib::Vector3(4, -2, 4).Normalize();

		break;
	case PlayerSlush::AttackType::NORMAL_3:

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		v1AddPos = MelLib::Vector3(0, 1, 1);
		v2MoveVector = MelLib::Vector3(0, 5, 4).Normalize();
		
		break;
	default:
		break;
	}

	//��]
	//���ꃉ���_���ɂ��Ȃ��Ă�������
	rotSegmentPosData();
	//���W�Z�b�g
	MelLib::Value2<MelLib::Vector3>pos;
	pos.v1 = GetPosition() + v1AddPos;
	pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, v2MoveVector, COLLISION_LENGTH);
	capsuleData[0].GetRefSegment3DData().SetPosition(pos);

	//����]
	capsuleData[0].GetRefSegment3DData().SetAxisAngle(MelLib::Vector3(0, -playerAngle, 0));

}

void PlayerSlush::Attack()
{

	MelLib::Vector3 attackAngle = 0;

	//����̊p�x(0,0,1�����Ă�Ƃ�)�Z�b�g
	//SetAxisAngle�Ŏ��̊p�x�ς��Ă邽�߁A0,0,1�����Ă�Ƃ��̂ł���
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		attackAngle = MelLib::Vector3(8, 8, 0);
		break;

	case PlayerSlush::AttackType::NORMAL_2:
		attackAngle = MelLib::Vector3(0, -8, 0);
		break;

	case PlayerSlush::AttackType::NORMAL_3:
		attackAngle = MelLib::Vector3(8, 0, 0);
		break;

	default:
		break;
	}

	capsuleData[0].GetRefSegment3DData().SetAngle
	(capsuleData[0].GetRefSegment3DData().GetAngle() + attackAngle);
}

PlayerSlush::PlayerSlush(const MelLib::Vector3& pos, const MelLib::Vector3& playerDir, const AttackType type,const int attackTime):
	attackType(type)
{

	eraseTimer.SetMaxTime(attackTime);

	//�v���C���[�̌����Ă�����ɉ�]����ׂ̏���
	playerAngle =
		MelLib::LibMath::Vecto2ToAngle(MelLib::Vector2(playerDir.x, playerDir.z), true);
	
	//playerAngle = 0;

	//����]����0,0,1�Ƃ��Ă�̂ŁA0,0,1��0�x�ɂȂ�悤�ɂ���
	playerAngle -= 90.0f;



	capsuleData.resize(1);
	capsuleData[0].SetRadius(0.2f);	
	SetPosition(pos);



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
