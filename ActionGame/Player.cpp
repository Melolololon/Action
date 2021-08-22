#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"

Player::Player(const MelLib::Vector3& pos)
{
	//物理演算のためにseterとgeter作る?
	position = pos;
	
	collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].SetRadius(0.5f);
	
}

void Player::Update()
{
	Move();
	Camera();
}

void Player::Move()
{
	velocity = 0;
	if (MelLib::Input::LeftStickUp(1, 70)
		|| MelLib::Input::LeftStickDown(1, 70)
		|| MelLib::Input::LeftStickRight(1, 70)
		|| MelLib::Input::LeftStickLeft(1, 70)) 
	{
		velocity = MelLib::Input::LeftStickVector3(1, MelLib::Camera::Get(), false, true);
	}

	position += velocity;
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(position + MelLib::Vector3(0, 3, 0), position + MelLib::Vector3(0, -3, 0)));


}

void Player::Camera()
{

	//メインカメラのポインタ取得
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0;
	float MAX_CAMERA_ANGLE_X = 60.0f;

	if (MelLib::Input::RightStickLeft(1, 30.0f))addCameraAngle.y = -cameraSpeed;
	if (MelLib::Input::RightStickRight(1, 30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(1, 30.0f))addCameraAngle.x = -cameraSpeed;
	if (MelLib::Input::RightStickDown(1, 30.0f))addCameraAngle.x = cameraSpeed;

	if (cameraAngle.x >= MAX_CAMERA_ANGLE_X)cameraAngle.x = MAX_CAMERA_ANGLE_X;
	if (cameraAngle.x <= -MAX_CAMERA_ANGLE_X)cameraAngle.x = -MAX_CAMERA_ANGLE_X;

	pCamera->SetAngle(cameraAngle + addCameraAngle);
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(80.0f);

	//pCamera->SetRotateCriteriaPosition(position);
	pCamera->SetRotateCriteriaPosition
	(MelLib::LibMath::FloatDistanceMoveVector3
	(position,MelLib::LibMath::OtherVector(pCamera->GetCameraPosition(),pCamera->GetTargetPosition()),30.0f)
	);
}

void Player::Draw()
{
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
}
