#include "JumpAttack.h"

JumpAttack::JumpAttack(const MelLib::Vector3& pos, const float radius):GameObject("JumpAttack")
{
	sphereDatas["main"].resize(1);
	sphereDatas["main"][0].SetPosition(pos);
	SetScale(radius);

	collisionCheckDistance = FLT_MAX;

}

void JumpAttack::Update()
{
	// ��U��������
	eraseManager = true;
}

void JumpAttack::Draw()
{
}
