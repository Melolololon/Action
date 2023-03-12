#include "NewEnemy.h"
#include"LibMath.h"

#include"RecoveryItem.h"

#include<Random.h>
#include<GameObjectManager.h>

#include "EnemyDeadCounter.h"

Player* NewEnemy::pPlayer;
const float NewEnemy::MOVE_SPEED = 0.6f;
const float NewEnemy::MIN_DISTANCE = 60.0f;
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
	//if (MelLib::Random::GetRandomNumber(7) == 0) 
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

NewEnemy::NewEnemy(const std::string& name)
	:GameObject(name)
{
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
