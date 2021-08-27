#include "PlayerSlush.h"
#include<LibMath.h>

void PlayerSlush::SetAttackParam()
{
	//Œ•‚Ì’·‚³‚Í•Ï‚í‚ç‚È‚¢‚Ì‚ÅAèŒ³‚ÌÀ•W(v1)‚©‚çA
	//”»’è¶¬‚ÌØ‚Áæ‚Ö‚ÌƒxƒNƒgƒ‹•ûŒü‚ÖCOLLISION_LENGTH•ª“®‚©‚µ‚½À•W‚ğv2‚Æ‚·‚é

	//”»’èƒTƒCƒY
	static const float COLLISION_LENGTH = 5.0f;
	MelLib::Value2<MelLib::Vector3>pos;
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);
		
		pos.v1 = position + MelLib::Vector3(0, -1, 1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(-4, 4, 2).Normalize(), COLLISION_LENGTH);
		
		
		capsuleData[0].GetRefSegment3DData().SetPosition(pos);

		break;

	case PlayerSlush::AttackType::NORMAL_2:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		pos.v1 = position + MelLib::Vector3(1, -1, 1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(4, -2, 4).Normalize(), COLLISION_LENGTH);
		capsuleData[0].GetRefSegment3DData().SetPosition(pos);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		eraseTimer.SetMaxTime(10);

		capsuleData[0].GetRefSegment3DData().SetRotatePoint(0.1f);

		pos.v1 = position + MelLib::Vector3(0,1,1);
		pos.v2 = MelLib::LibMath::FloatDistanceMoveVector3(pos.v1, MelLib::Vector3(0,5,4).Normalize(), COLLISION_LENGTH);
		capsuleData[0].GetRefSegment3DData().SetPosition(pos);
		break;
	default:
		break;
	}
}

void PlayerSlush::Attack()
{
	//ˆÚ“®‚µ‚È‚ª‚ç‰ñ“]‚³‚¹‚é‚±‚Æ‚ÅA‚¢‚¢Š´‚¶‚É‚È‚é?
	//‰ñ“]g‚í‚È‚­‚Ä‚àÀ•W‚ğÎ‚ß‚ÉˆÚ“®‚³‚¹‚ê‚Î–â‘è‚È‚¢?
	//ˆÚ“®‚³‚¹‚é•ûŒü‚ÍAƒvƒŒƒCƒ„[‚ÌŒü‚«‚É‰‚¶‚Ä•Ï‚¦‚é?(‰ñ“]‚³‚¹‚é?)
	//‚â‚Á‚Ï‰ñ“]‚Ì‚Ù‚¤‚ª‚¢‚¢?
	switch (attackType)
	{
	case PlayerSlush::AttackType::NORMAL_1:
		
		//¡Y‚Ì‹——£‚µ‚©‰~’Œ•”•ª‚É‰e‹¿‚³‚¹‚Ä‚È‚¢‚©‚ç‰ñ“]‚³‚¹‚é‚Æk‚Ş
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(8, 8, 0));

		break;

	case PlayerSlush::AttackType::NORMAL_2:
		

		//¡Y‚Ì‹——£‚µ‚©‰~’Œ•”•ª‚É‰e‹¿‚³‚¹‚Ä‚È‚¢‚©‚ç‰ñ“]‚³‚¹‚é‚Æk‚Ş
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(0, -8, 0));

		
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		//¡Y‚Ì‹——£‚µ‚©‰~’Œ•”•ª‚É‰e‹¿‚³‚¹‚Ä‚È‚¢‚©‚ç‰ñ“]‚³‚¹‚é‚Æk‚Ş
		capsuleData[0].GetRefSegment3DData().SetAngle
		(capsuleData[0].GetRefSegment3DData().GetAngle() + MelLib::Vector3(8, 0, 0));
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
