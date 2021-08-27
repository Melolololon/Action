#include "PlayerSlush.h"
#include<LibMath.h>

void PlayerSlush::SetAttackParam()
{
	//座標回転させてもアングルをいじってないから回転おかしくなる?
	//playerAngleをカプセルの角度に加算減算する?

	//0度回転で判定のX軸回転を100%、90度回転で0%、180で-100%(反対側)になればよい
	
	

	//判定の長さ
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

	//回転
	pos.v1 = MelLib::LibMath::RotateVector3(pos.v1, MelLib::Vector3(0, -1, 0), playerAngle);
	pos.v2 = MelLib::LibMath::RotateVector3(pos.v2, MelLib::Vector3(0, -1, 0), playerAngle);

	capsuleData[0].GetRefSegment3DData().SetPosition(pos);
}

void PlayerSlush::Attack()
{
	//角度を調整するための割合
	 //0度回転で判定のX軸回転を100%、90度回転で0%、180で-100%(反対側)になるように
	float angleMulPar = sin(MelLib::LibMath::AngleConversion(0, playerAngle + 90));

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
		//今Yの距離しか円柱部分に影響させてないから回転させると縮む
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
	//プレイヤーの向いてる方向に回転する為の処理
	playerAngle =
		MelLib::LibMath::Vecto2ToAngle(MelLib::Vector2(playerDir.x, playerDir.z), true);
	//未回転時を0,0,1としてるので、0,0,1が0度になるようにする
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
