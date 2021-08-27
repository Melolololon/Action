#include "PlayerSlush.h"
#include<LibMath.h>

void PlayerSlush::SetAttackParam()
{
	//À•W‰ñ“]‚³‚¹‚Ä‚àƒAƒ“ƒOƒ‹‚ğ‚¢‚¶‚Á‚Ä‚È‚¢‚©‚ç‰ñ“]‚¨‚©‚µ‚­‚È‚é?
	//playerAngle‚ğƒJƒvƒZƒ‹‚ÌŠp“x‚É‰ÁZŒ¸Z‚·‚é?

	//0“x‰ñ“]‚Å”»’è‚ÌX²‰ñ“]‚ğ100%A90“x‰ñ“]‚Å0%A180‚Å-100%(”½‘Î‘¤)‚É‚È‚ê‚Î‚æ‚¢
	
	

	//”»’è‚Ì’·‚³
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

	//‰ñ“]
	pos.v1 = MelLib::LibMath::RotateVector3(pos.v1, MelLib::Vector3(0, -1, 0), playerAngle);
	pos.v2 = MelLib::LibMath::RotateVector3(pos.v2, MelLib::Vector3(0, -1, 0), playerAngle);

	capsuleData[0].GetRefSegment3DData().SetPosition(pos);
}

void PlayerSlush::Attack()
{
	//Šp“x‚ğ’²®‚·‚é‚½‚ß‚ÌŠ„‡
	 //0“x‰ñ“]‚Å”»’è‚ÌX²‰ñ“]‚ğ100%A90“x‰ñ“]‚Å0%A180‚Å-100%(”½‘Î‘¤)‚É‚È‚é‚æ‚¤‚É
	float angleMulPar = sin(MelLib::LibMath::AngleConversion(0, playerAngle + 90));

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

PlayerSlush::PlayerSlush(const MelLib::Vector3& pos, const MelLib::Vector3& playerDir, const AttackType type):
	attackType(type)
{
	//ƒvƒŒƒCƒ„[‚ÌŒü‚¢‚Ä‚é•ûŒü‚É‰ñ“]‚·‚éˆ×‚Ìˆ—
	playerAngle =
		MelLib::LibMath::Vecto2ToAngle(MelLib::Vector2(playerDir.x, playerDir.z), true);
	//–¢‰ñ“]‚ğ0,0,1‚Æ‚µ‚Ä‚é‚Ì‚ÅA0,0,1‚ª0“x‚É‚È‚é‚æ‚¤‚É‚·‚é
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
