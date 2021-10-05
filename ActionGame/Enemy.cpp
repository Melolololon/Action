#include "Enemy.h"

#include<LibMath.h>
#include<Collision.h>

Player* Enemy::pPlayer;

Enemy::Enemy(const MelLib::Vector3& pos)
{
	SetPosition(pos);

	
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}

void Enemy::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{
}

void Enemy::MoveToPlayer()
{
	MelLib::Vector3 playerPos = pPlayer->GetPosition();


}
