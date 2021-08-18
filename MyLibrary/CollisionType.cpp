#include"CollisionType.h"
#include"LibMath.h"

using namespace MelLib;

Value2<Vector3> MelLib::Segment3DData::GetRotatePosition()const
{
	Vector3 centerPos = GetCenterPosition();
	
	//中心が原点にいるときの座標で回転
	Value2<Vector3> rotVector
		(
			Quaternion::GetZXYRotateQuaternion(position[0] - centerPos, Vector3(angle.x, angle.y, angle.z)).ToVector3(),
			Quaternion::GetZXYRotateQuaternion(position[1] - centerPos, Vector3(angle.x, angle.y, angle.z)).ToVector3()
		);

	//引いた分戻す
	rotVector.v1 += centerPos;
	rotVector.v2 += centerPos;

	return rotVector;
}

Vector3 MelLib::Segment3DData::GetCenterPosition() const
{
	return LibMath::CalcCenterPosition3D(position[0], position[1]);
}

float MelLib::Segment3DData::GetPositionDistance() const
{
	return LibMath::CalcDistance3D(position[0], position[1]);
}
