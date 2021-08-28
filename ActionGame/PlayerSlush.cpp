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

	//v1に加算する座標
	MelLib::Vector3 v1AddPos;
	//v2の移動方向
	MelLib::Vector3 v2MoveVector;
	
	const float PLAYER_ANGLE = playerAngle;
	auto rotAddPos = [&v1AddPos, &v2MoveVector,&PLAYER_ANGLE]()
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
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		
		v1AddPos = MelLib::Vector3(0, -1, 1);
		v2MoveVector = MelLib::Vector3(-4, 4, 2).Normalize();

		break;

	case PlayerSlush::AttackType::NORMAL_2:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		v1AddPos = MelLib::Vector3(1, -1, 1);
		v2MoveVector = MelLib::Vector3(4, -2, 4).Normalize();

		break;
	case PlayerSlush::AttackType::NORMAL_3:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		v1AddPos = MelLib::Vector3(0, 1, 1);
		v2MoveVector = MelLib::Vector3(0, 5, 4).Normalize();
		
		break;
	default:
		break;
	}

	//回転
	rotAddPos();
	pos.v1 = position + v1AddPos;
	pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, v2MoveVector, COLLISION_LENGTH);

	capsuleData[0].GetRefSegment3DData().SetPosition(pos);
}

void PlayerSlush::Attack()
{
	//45回転の時0.7,0,0.7になるからおかしくなる(まわりすぎる?)
	

	MelLib::Vector3 attackAngle = 0;
	const float PLAYER_ANGLE = playerAngle;
	
	//プレイヤーの向きに応じて回転させるラムダ式
	auto rotAttackAngleXZ = [&attackAngle,&PLAYER_ANGLE]()
	{
		float ATTACK_ANGLE_Y = attackAngle.y;
		attackAngle = 
			MelLib::LibMath::RotateVector3(attackAngle, MelLib::Vector3(0,1,0), PLAYER_ANGLE);
		attackAngle.y = ATTACK_ANGLE_Y;

		
		

		const float hosei = MelLib::Vector2(0.5, 0.5).Normalize().x/* - 0.5f*/;
		if(attackAngle.x > 0)attackAngle.x -= hosei;
		else attackAngle.x += hosei;
		if (attackAngle.z > 0)attackAngle.z -= hosei;
		else attackAngle.z += hosei;
	};


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

	rotAttackAngleXZ();
	//今Yの距離しか円柱部分に影響させてないから回転させると縮む
	capsuleData[0].GetRefSegment3DData().SetAngle
	(capsuleData[0].GetRefSegment3DData().GetAngle() + attackAngle);
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
