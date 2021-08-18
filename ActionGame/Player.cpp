#include "Player.h"
#include"Input.h"
#include"Camera.h"

Player::Player(const MelLib::Vector3& pos)
{
	//ï®óùââéZÇÃÇΩÇﬂÇ…seterÇ∆geterçÏÇÈ?
	position = pos;
	
	collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].r = 1.0f;
	
}

void Player::Update()
{
	velocity = MelLib::Input::LeftStickVector3(1, MelLib::Camera::Get(), false, true);

	position += velocity;
	capsuleData[0].segmentData.position[0] = position + MelLib::Vector3(0,2,0);
	capsuleData[0].segmentData.position[1] = position + MelLib::Vector3(0,-2,0);
}

void Player::Draw()
{
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}
