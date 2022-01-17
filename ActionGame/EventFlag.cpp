#include "EventFlag.h"

EventFlag::EventFlag(const MelLib::Vector3& pos, float radius)
{
	SetPosition(pos);

	sphereData.resize(1);
	sphereData[0].SetPosition(GetPosition());
	sphereData[0].SetRadius(radius);
}
