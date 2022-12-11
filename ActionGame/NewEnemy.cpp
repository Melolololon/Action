#include "NewEnemy.h"
#include"LibMath.h"
Player* NewEnemy::pPlayer;
const float NewEnemy::MOVE_SPEED = 0.2f;
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
//	// ”ò‚Ô“G‚©‚»‚êˆÈŠO‚©‚Ìƒtƒ‰ƒO‚ðŽ‚½‚¹‚Ä”»’f‚·‚é‚æ‚¤‚É‚·‚é
//	CalcMovePhysics();
//}
//
//bool NewEnemy::CheckMove()
//{
//	return MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition()) <= MIN_DISTANCE;
//}

void NewEnemy::BeBlownAwayMove()
{
	MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(GetPosition());
	MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * -5.0f;

	AddPosition(moveVector);
}

NewEnemy::NewEnemy( const std::string& name)
	:GameObject(name)
{
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