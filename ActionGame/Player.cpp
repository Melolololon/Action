#include "Player.h"

Player::Player(const MelLib::Vector3& pos)
{
	//ï®óùââéZÇÃÇΩÇﬂÇ…seterÇ∆geterçÏÇÈ?
	position = pos;
	
	capsuleData.resize(1);
	capsuleData[0].r = 1.0f;
	capsuleData[0].length = 4.0f;
}

void Player::Update()
{


	capsuleData[0].position = position;
}

void Player::Draw()
{
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}
