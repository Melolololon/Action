#include "PlayerSlush.h"
#include<LibMath.h>

void PlayerSlush::SetAttackParam()
{
	//剣の長さは変わらないので、手元の座標(v1)から、
	//判定生成時の切っ先へのベクトル方向へCOLLISION_LENGTH分動かした座標をv2とする

	//判定サイズ
	static const float COLLISION_LENGTH = 5.0f;
	MelLib::Value2<MelLib::Vector3>pos;
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		attackTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		
		pos.v1 = position + MelLib::Vector3(0, -1, 1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(-4, 4, 2).Normalize(), COLLISION_LENGTH);
		
		
		capsuleData[0].GetRefSegment3DData().SetPosition(pos);

		break;

	case PlayerSlush::AttackType::NORMAL_2:
		attackTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		pos.v1 = position + MelLib::Vector3(1, -1, 1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(4, -2, 4).Normalize(), COLLISION_LENGTH);
		capsuleData[0].GetRefSegment3DData().SetPosition(pos);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		break;
	default:
		break;
	}
}

void PlayerSlush::Attack()
{
	//移動しながら回転させることで、いい感じになる?
	//回転使わなくても座標を斜めに移動させれば問題ない?
	//移動させる方向は、プレイヤーの向きに応じて変える?(回転させる?)
	//やっぱ回転のほうがいい?
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		
		//今Yの距離しか円柱部分に影響させてないから回転させると縮む
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(8, 8, 0));

		break;

	case PlayerSlush::AttackType::NORMAL_2:
		

		//今Yの距離しか円柱部分に影響させてないから回転させると縮む
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(0, -8, 0));

		
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		break;
	default:
		break;
	}
}

PlayerSlush::PlayerSlush(const MelLib::Vector3& pos, const MelLib::Vector3& playerDir, const AttackType type)
{
	position = pos;
	attackType = type;

	capsuleData.resize(1);
	capsuleData[0].SetRadius(0.2f);


	attackTimer.SetStopFlag(false);

	SetAttackParam();
}

void PlayerSlush::Update()
{

	/*capsuleData[0].GetRefSegment3DData().SetPosition
	(MelLib::Value2<MelLib::Vector3>(position + MelLib::Vector3(-4,4,2), position + MelLib::Vector3(0, -1, 4)));*/


	if (attackTimer.GetSameAsMaxFlag())eraseManager = true;


	Attack();
}

void PlayerSlush::Draw()
{
}

void PlayerSlush::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}
