#include "NewEnemy.h"
#include"LibMath.h"

#include"RecoveryItem.h"

#include<Random.h>
#include<GameObjectManager.h>

#include "EnemyDeadCounter.h"

#include"EnemyDarknessEffect.h"

Player* NewEnemy::pPlayer;
const float NewEnemy::MOVE_SPEED = 0.6f;
const float NewEnemy::MIN_DISTANCE = 15.0f;
//
//void NewEnemy::ToPlayerMove()
//{
//	MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(GetPosition());
//	MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * MOVE_SPEED;
//
//	AddPosition(moveVector);
//
//
//	// 飛ぶ敵かそれ以外かのフラグを持たせて判断するようにする
//	CalcMovePhysics();
//}
//
//bool NewEnemy::CheckMove()
//{
//	return MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition()) <= MIN_DISTANCE;
//}

void NewEnemy::DropItem()
{
	if (MelLib::Random::GetRandomNumber(7) == 0) 
	{
		// ドロップした瞬間座標ズレてる?
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<RecoveryItem>(GetPosition() + MelLib::Vector3(0,3,0)));
	}
}

void NewEnemy::BeBlownAwayMove()
{
	MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(GetPosition());
	MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * -5.0f;

	AddPosition(moveVector);
}

void NewEnemy::Dead()
{

	if (deadEndTimer.GetMaxOverFlag())
	{
		SetScale(GetScale() - 0.1f);
		if (GetScale().x <= 0)
		{
			DropItem();
			eraseManager = true;
			EnemyDeadCounter::GetInstance()->CountUp();
		}
	}
}

void NewEnemy::AddParticle()
{
	// 自分と相手の最近点(相手に一番近い点)を取得
	MelLib::CapsuleCalcResult result = GetCapsuleCalcResult();
	MelLib::Vector3 myCapsuleNearPos = result.segment3DCalcResult.thisCapsuleLineClosestPoint;
	MelLib::Vector3 otherCapsuleNearPos = result.segment3DCalcResult.otherCapsuleLineClosestPoint;


}

NewEnemy::NewEnemy(const std::string& name)
	:GameObject(name)
{
	addDarknessEffectTimer.SetMaxTime(60 * 0.3);
	addDarknessEffectTimer.SetStopFlag(false);

	deadEndTimer.SetMaxTime(60 * 0.75);
	hpGauge = std::make_unique<EnemyHPGauge>(hp);
}

void NewEnemy::Update()
{
}

void NewEnemy::Draw()
{
}

void NewEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
}

void NewEnemy::SetStartAttackAnimation()
{
}

void NewEnemy::AddDarknessEffect()
{
	if (addDarknessEffectTimer.GetMaxOverFlag()) 
	{
		//MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyDarknessEffect>(GetPosition()));
		addDarknessEffectTimer.ResetTimeZero();

	}
}
