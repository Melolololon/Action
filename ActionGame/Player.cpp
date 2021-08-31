#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"
#include"PlayerSlush.h"

#include"Ground.h"

Player::Player(const MelLib::Vector3& pos)
{
	//�������Z�̂��߂�seter��geter���?
	
	
	collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].SetRadius(0.5f); 
	SetPosition(pos);
	
	attackTimer.SetMaxTime(ATTACK_END_TIME);

	//�����h�~
	StartThrowUp(0.0f);
}

void Player::Update()
{
	Move();
	Jump();
	Attack();
	
	CalcMovePhysics();
	SetCollisionPosition();

	Camera();
}

void Player::Move()
{
	//�U�����͓����Ȃ��悤��
	if (attackTimer.GetNowTime() > 0)return;


	static const float DASH_STICK_PAR = 60.0f;
	static const float WALK_STICK_PAR = 20.0f;
	static const float DASH_SPEED = 0.7f;
	static const float WALK_SPEED = 0.25f;

	if (MelLib::Input::LeftStickUp(1, WALK_STICK_PAR)
		|| MelLib::Input::LeftStickDown(1, WALK_STICK_PAR)
		|| MelLib::Input::LeftStickRight(1, WALK_STICK_PAR)
		|| MelLib::Input::LeftStickLeft(1, WALK_STICK_PAR)) 
	{
		playerDir = MelLib::Input::LeftStickVector3(1, MelLib::Camera::Get(), false, true);

		MelLib::Vector3 addPos = playerDir;

		//�_�b�V���̌X���ʂ𒴂��Ă�����_�b�V��
		if (MelLib::Input::LeftStickUp(1, DASH_STICK_PAR)
			|| MelLib::Input::LeftStickDown(1, DASH_STICK_PAR)
			|| MelLib::Input::LeftStickRight(1, DASH_STICK_PAR)
			|| MelLib::Input::LeftStickLeft(1, DASH_STICK_PAR))
		{
			addPos *= DASH_SPEED;
		}
		else
		{
			addPos *= WALK_SPEED;
		}

		position += addPos;
	}
	
}

void Player::Jump()
{
	if(MelLib::Input::ButtonTrigger(1,MelLib::GamePadButton::A))
	{
		StartThrowUp(3.0f);
	}
	
}

void Player::SetCollisionPosition()
{
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 3, 0), GetPosition() + MelLib::Vector3(0, -3, 0)));
}

void Player::Attack()
{
	if(attackTimer.GetSameAsMaxFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		attackTimer.ResetTimeZero();

		attackTimer.SetStopFlag(true);
	}

	//if��2�s�ڂ̃^�C�}�[�m�F�́A�R���{�I�����NONE�ɂ��邽�߁A���̍U�����ɓ���Ȃ��悤�ɂ��邽�߂ɏ����Ă�
	if (MelLib::Input::ButtonTrigger(1, MelLib::GamePadButton::X)
		&& (attackTimer.GetNowTime() == 0 && currentAttack == PlayerSlush::AttackType::NONE
			|| attackTimer.GetNowTime() >= ATTACK_NEXT_TIME && currentAttack != PlayerSlush::AttackType::NONE))
	{
		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(false);

		SetAttackType();

		//�R���{�̍Ō�̎��Ƀ{�^����������NONE���Z�b�g���邽�߁A�R���{���I��点�邽�߂�if
		if (currentAttack != PlayerSlush::AttackType::NONE) 
		{
			MelLib::GameObjectManager::GetInstance()->AddObject(
				std::make_shared<PlayerSlush>(GetPosition(), playerDir, currentAttack));
		}



	}
}

void Player::SetAttackType()
{
	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		currentAttack = PlayerSlush::AttackType::NORMAL_1;
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		currentAttack = PlayerSlush::AttackType::NORMAL_2;
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		currentAttack = PlayerSlush::AttackType::NORMAL_3;
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;
	default:
		break;
	}
}

void Player::Camera()
{
	//�X�e�B�b�N�̓|����ŃX�s�[�h�ς���
	//�ɋ}�������ق�������?���񂾂�������Ă��B���߂ɃX�s�[�h�}�b�N�X�ɂȂ�

	//���C���J�����̃|�C���^�擾
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0;
	float MAX_CAMERA_ANGLE_X = 30.0f;
	float MIX_CAMERA_ANGLE_X = -5.0f;


	//�J�������x(�ݒ�ŕς�����悤�ɂ��邽�߂ɁAconst�ɂ��ĂȂ�)
	float cameraSpeed = 3.0f;
	if (MelLib::Input::RightStickLeft(1, 30.0f))addCameraAngle.y = -cameraSpeed;
	if (MelLib::Input::RightStickRight(1, 30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(1, 30.0f))addCameraAngle.x = -cameraSpeed;
	if (MelLib::Input::RightStickDown(1, 30.0f))addCameraAngle.x = cameraSpeed;

	cameraAngle += addCameraAngle;
	if (cameraAngle.x >= MAX_CAMERA_ANGLE_X)cameraAngle.x = MAX_CAMERA_ANGLE_X;
	if (cameraAngle.x <= MIX_CAMERA_ANGLE_X)cameraAngle.x = MIX_CAMERA_ANGLE_X;

	pCamera->SetAngle(cameraAngle);
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(80.0f);

	//pCamera->SetRotateCriteriaPosition(position);
	SetCameraPosition();
}

void Player::SetCameraPosition()
{
	MelLib::Camera* pCamera = MelLib::Camera::Get();
	pCamera->SetRotateCriteriaPosition
	(MelLib::LibMath::FloatDistanceMoveVector3
	(GetPosition(), MelLib::LibMath::OtherVector(pCamera->GetCameraPosition(), pCamera->GetTargetPosition()), 30.0f)
	);
}

void Player::Draw()
{
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D collisionType, const int arrayNum, const MelLib::ShapeType3D hitObjColType, const int hitObjArrayNum)
{
	
	if(typeid(*object) == typeid(Ground))
	{
		MelLib::Vector3 addPos;
		
		//velocity���߂��Ɩ߂��������Ⴄ
		//�J�v�Z���̉��̍��W-���a(��[���W)����Փ˓_�ւ̃x�N�g���̑傫�����߂���OK?
		
		//�d�͉����x���グ��
		//addPos.y += MelLib::GameObject::GetGravutationalAcceleration();
		
		//�����グ�����I��
		StopThrowUp();

		//�J�v�Z���̉��̐�[����Փ˓_�̃x�N�g�������߁A���̕������o���B
		addPos.y += capsuleData[0].GetSegment3DData().GetCalcResult().boardHitPos.y -
			 (capsuleData[0].GetSegment3DData().GetPosition().v2.y - capsuleData[0].GetRadius());

		
		AddPosition(addPos);
		
		MelLib::Vector3 vel = GetVelocity();
		int z = 0;
	}
}

void Player::AddPosition(const MelLib::Vector3& vec)
{
	SetPosition(GetPosition() + vec);
	SetCollisionPosition();

	SetCameraPosition();
}

void Player::SetPosition(const MelLib::Vector3& pos)
{
	position = pos;
	SetCollisionPosition();

	SetCameraPosition();
}
