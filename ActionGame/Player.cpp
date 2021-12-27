#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Title.h"

#include"ActionPart.h"

#include"PlayerSlush.h"

#include"Ground.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"

std::unordered_map<Player::ActionType, MelLib::PadButton> Player::keyConfigData =
{
	{ActionType::JUMP, MelLib::PadButton::A},
	{ActionType::ATTACK, MelLib::PadButton::X},
	{ActionType::DASH, MelLib::PadButton::B},

};

const float Player::JUMP_POWER = 2.0f;

std::unordered_map<PlayerSlush::AttackType, const Player::AttackData> Player::attackData =
{
	{PlayerSlush::AttackType::NONE,AttackData(0,0,0)},
	{PlayerSlush::AttackType::NORMAL_1,AttackData(1,20,10)},
	{PlayerSlush::AttackType::NORMAL_2,AttackData(1,30,20)},
	{PlayerSlush::AttackType::NORMAL_3,AttackData(1,20,10)},
	{PlayerSlush::AttackType::DASH_1,AttackData(1,20,10)},
};



void Player::LoadResources()
{
	// �^�C�g���ł��g�����玩���폜�폜
	MelLib::ModelData::Load("Resources/Model/Player/Player_Test.fbx", false, "Player");

}

Player::Player(const MelLib::Vector3& pos)
{

	//�������Z�̂��߂�seter��geter���?


	collisionFlag.capsule = true;
	capsuleData.resize(1);
	capsuleData[0].SetRadius(2.5f);
	capsuleData[0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 3, 0), GetPosition() + MelLib::Vector3(0, -18, 0)));

	segment3DData.resize(1);
	segment3DData[0] = capsuleData[0].GetSegment3DData();


	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	modelObjects["main"].Create(MelLib::ModelData::Get("Player"), nullptr);

	modelObjects["main"].SetAnimationPlayFlag(true);
#pragma region �_�b�V��
	dashEasing.SetAddPar(5.0f);


#pragma endregion

	mutekiTimer.SetMaxTime(60 * 1.5);



	// �^�C�g���p����
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (typeid(*currentScene) == typeid(Title))
	{
		modelObjects["main"].SetScale(MelLib::Vector3(3));
		modelObjects["main"].SetAngle(0);
		modelObjects["main"].SetAnimation("No_Cont");
	}
	else
	{
		modelObjects["main"].SetScale(MelLib::Vector3(3));
		modelObjects["main"].SetAngle(0);
		modelObjects["main"].SetPosition(MelLib::Vector3(0, -16, -0));


		//�����h�~
		FallStart(0.0f);
	}

	SetPosition(pos);
}

void Player::Update()
{

	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())
	{

		MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
		if (typeid(*currentScene) != typeid(Title))
		{
			SetCameraPosition();
		}
		return;
	}

	modelObjects["main"].Update();
	if (!setStartParam)
	{
		if (typeid(*currentScene) != typeid(Title))
		{
			JumpAnimation();
			ChangeAnimationData();

			SetCameraPosition();
			CalcMovePhysics();
		}
		return;
	}


	if (hitGroundNotMove)
	{
		if (hitGroundNotMoveTimer.GetMaxOverFlag())
		{
			hitGroundNotMoveTimer.ResetTimeZero();
			hitGroundNotMoveTimer.SetStopFlag(true);
			hitGroundNotMove = false;
		}
		else
		{
			Camera();
			return;
		}
	}

	prePos = GetPosition();
	Move();
	Jump();
	Attack();


	CalcMovePhysics();

	Camera();
	LockOn();

	float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	// ��]����(��)
	modelObjects["main"].SetAngle(MelLib::Vector3(0, angle, 0));



	preHitGround = hitGround;
	hitGround = false;

	// �A�j���[�V�����ɍ��킹�čĐ����x�⃋�[�v��ݒ肷��
	ChangeAnimationData();




}

void Player::TitleUpdate()
{
	// �ړ�
	AddPosition(MelLib::Vector3(0, 0, 0.5f));


	// �W�����v
	if (!GetIsFall())
	{
		FallStart(JUMP_POWER);
	}
	CalcMovePhysics();

	JumpAnimation();
	ChangeAnimationData();
}

void Player::ChangeAnimationData()
{
	std::string animationName = modelObjects["main"].GetCurrentAnimationName();

	modelObjects["main"].SetAnimationSpeedMagnification(1);
	modelObjects["main"].SetAnimationEndStopFlag(false);

	if (animationName == "Dash") {
		modelObjects["main"].SetAnimationSpeedMagnification(2);
	}
	else if (animationName.find("Attack") != std::string::npos) {

		modelObjects["main"].SetAnimationSpeedMagnification(2);
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Dash_02") != std::string::npos) {

		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
	else if (animationName.find("Jump_Up") != std::string::npos)
	{
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}

	// �f�o�b�O�p
	if (MelLib::Input::KeyTrigger(DIK_1))
	{
		isTPause = !isTPause;
	}

	// ����T�|�[�Y
	if (isTPause)
	{
		modelObjects["main"].SetAnimation("_T");
		modelObjects["main"].SetAnimationSpeedMagnification(1);
		modelObjects["main"].SetAnimationEndStopFlag(false);
	}

}

void Player::Move()
{
	// ��
	modelObjects["main"].SetAnimationSpeedMagnification(1);

	Dash();

	if (isDash)
	{
		return;
	}
	else if (attackTimer.GetNowTime() > 0)
	{
		AttackMove();
		return;
	}


	static const float FAST_WARK_STICK_PAR = 60.0f;
	static const float WALK_STICK_PAR = 20.0f;
	static const float MAX_FAST_WARK_SPEED = 2.5f;
	static const float MAX_WALK_SPEED = 0.5f;
	static const float FRAME_UP_FAST_WARK_SPEED = MAX_FAST_WARK_SPEED / 10;
	static const float FRAME_UP_WARK_SPEED = MAX_WALK_SPEED / 10;

	if (MelLib::Input::LeftStickUp(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickDown(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickRight(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickLeft(WALK_STICK_PAR))
	{
		direction = MelLib::Input::LeftStickVector3(MelLib::Camera::Get(), false, true);

		MelLib::Vector3 addPos = direction;

		//�_�b�V���̌X���ʂ𒴂��Ă�����_�b�V��
		if (MelLib::Input::LeftStickUp(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickDown(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickRight(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickLeft(FAST_WARK_STICK_PAR))
		{
			if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
			else moveSpeed = MAX_FAST_WARK_SPEED;
			addPos *= moveSpeed;

			modelObjects["main"].SetAnimation("Dash");
		}
		else
		{
			if (moveSpeed < MAX_WALK_SPEED)moveSpeed += FRAME_UP_WARK_SPEED;
			else moveSpeed = MAX_WALK_SPEED;
			addPos *= moveSpeed;
			addPos *= MAX_WALK_SPEED;

			modelObjects["main"].SetAnimation("Walk");
		}

		AddPosition(addPos);


	}
	else
	{
		moveSpeed = 0.0;

		modelObjects["main"].SetAnimation("No_Cont");
	}


	//if (position.y <= -100)position.y = 100;

	//�������邽�߂�
	if (!GetIsFall())FallStart(0.0f);



}

void Player::AttackMove()
{

	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		//AddPosition(direction * 0.1f);
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::DASH_1:
		break;
	default:
		break;
	}



}

void Player::Dash()
{
	static const float MAX_DASH_SPEED = 1.5f;
	static const float START_DASH_SPEED = 1.0f;

	static const float DASH_DISTANCE = 200.0f;

	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::DASH])
		&& !isDash
		&& currentAttack == PlayerSlush::AttackType::NONE)
	{
		isDash = true;

		dashEasing.SetStart(GetPosition());
		dashEasing.SetEnd(MelLib::LibMath::FloatDistanceMoveVector3(GetPosition(), direction, DASH_DISTANCE));
	}

	if (isDash)
	{
		MelLib::Vector3 prePos = GetPosition();
		MelLib::Vector3 pos = dashEasing.EaseInOut();
		MelLib::Vector3 addPos = pos - prePos;

		// Y��0�ɂ��Ȃ��ƃ_�b�V������Y���ς�����炨�������Ȃ����Ⴄ
		AddPosition(MelLib::Vector3(addPos.x, 0, addPos.z));


		// �_�b�V���U��������Ȃ�������_�b�V���A�j���[�V����
		if (currentAttack != PlayerSlush::AttackType::DASH_1)
		{
			modelObjects["main"].SetAnimation("Dash_02");
		}

	}

	if (dashEasing.GetPar() >= 100.0f)
	{
		dashEasing.SetPar(0.0f);
		isDash = false;
	}
}

void Player::Jump()
{
	// �W�����v�̒��n�A�j���[�V�����́A�W�����v�̋t�Đ���OK

	if (attackTimer.GetNowTime() != 0)return;

	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::JUMP]))
	{
		FallStart(JUMP_POWER);
	}


	JumpAnimation();
}

void Player::JumpAnimation()
{
	// ��������������W�����v�A�j���[�V����
	if (!hitGround && !preHitGround && !isDash && currentAttack == PlayerSlush::AttackType::NONE)
	{
		if (!jumpResetAnimation)
		{
			jumpResetAnimation = true;
			modelObjects["main"].ResetAnimation();
		}
		modelObjects["main"].SetAnimation("Jump_Up");
	}
}



void Player::Attack()
{
	if (attackTimer.GetMaxOverFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		attackTimer.ResetTimeZero();

		attackTimer.SetStopFlag(true);
	}

	//if��2�s�ڂ̃^�C�}�[�m�F�́A�R���{�I�����NONE�ɂ��邽�߁A���̍U�����ɓ���Ȃ��悤�ɂ��邽�߂ɏ����Ă�
	if (MelLib::Input::PadButtonTrigger(keyConfigData[ActionType::ATTACK])
		&& (attackTimer.GetNowTime() == 0 && currentAttack == PlayerSlush::AttackType::NONE
			|| attackTimer.GetNowTime() >= attackData[currentAttack].nextTime && currentAttack != PlayerSlush::AttackType::NONE))
	{


		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(false);


		// �����؂�ւ�
		Move();

		//CurrentAttack�X�V
		SetAttackType();
		//�U�����ԃZ�b�g
		attackTimer.SetMaxTime(attackData[currentAttack].time);

		//�R���{�̍Ō�̎��Ƀ{�^����������NONE���Z�b�g���邽�߁A�R���{���I��点�邽�߂�if
		if (currentAttack != PlayerSlush::AttackType::NONE)
		{
			if (pPSlush)pPSlush.reset();
			if (pRigthSlush)pRigthSlush.reset();


			CreateAttackSlush();

			//�A�j���[�V���������Z�b�g
			modelObjects["main"].ResetAnimation();
		}
	}

	//�U�������������
	//if (pPSlush)pPSlush->AddPosition(GetPosition() - prePos);
}

void Player::SetAttackType()
{


	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:

		// �_�b�V���U��
		if (isDash)
		{
			currentAttack = PlayerSlush::AttackType::DASH_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");
		}
		else if (hitGround)// �ʏ�U��
		{
			currentAttack = PlayerSlush::AttackType::NORMAL_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");

		}
		else// �W�����v�U��
		{
		}

		break;
	case PlayerSlush::AttackType::NORMAL_1:
		currentAttack = PlayerSlush::AttackType::NORMAL_2;

		modelObjects["main"].SetAnimation("Attack_Normal_2");
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		currentAttack = PlayerSlush::AttackType::NORMAL_3;

		modelObjects["main"].SetAnimation("Attack_Normal_3");
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;

	case PlayerSlush::AttackType::DASH_1:
		currentAttack = PlayerSlush::AttackType::NONE;
		break;

	default:
		break;
	}



}

void Player::CreateAttackSlush()
{
	// �����鎞�Ԃ����̍U���ֈڍs�ł���悤�ɂȂ鎞�ԂƓ����ɂ��邱��


	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		pRigthSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);

		break;
	case PlayerSlush::AttackType::NORMAL_2:


		pPSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, true);

		break;
	case PlayerSlush::AttackType::NORMAL_3:
		pPSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, true);

		pRigthSlush = std::make_shared<PlayerSlush>
			(GetPosition(), direction, currentAttack, attackData[currentAttack].nextTime, modelObjects["main"], startPos, startAngle, startScale, false);


		break;
	case PlayerSlush::AttackType::DASH_1:
		break;
	default:
		break;
	}

	if (pPSlush)MelLib::GameObjectManager::GetInstance()->AddObject(pPSlush);
	if (pRigthSlush)MelLib::GameObjectManager::GetInstance()->AddObject(pRigthSlush);
}

void Player::Muteki()
{
	if (mutekiTimer.GetMaxOverFlag())
	{
		isMuteki = false;
		mutekiTimer.ResetTimeZero();
		mutekiTimer.SetStopFlag(true);
	}
}

void Player::Camera()
{

	if (lockOn)return;

	//�X�e�B�b�N�̓|����ŃX�s�[�h�ς���
	//�ɋ}�������ق�������?���񂾂�������Ă��B���߂ɃX�s�[�h�}�b�N�X�ɂȂ�

	//���C���J�����̃|�C���^�擾
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0;
	static const float MAX_CAMERA_ANGLE_X = 30.0f;
	static const float MIX_CAMERA_ANGLE_X = -5.0f;

	//�ő�l�͈�Uconst(���ƂŐݒ�ōő�l��ς�����悤�ɂ���)
	static const float MAX_CAMERA_SPEED = 3.0f;
	static const float FRAME_UP_CAMERA_SPEED = MAX_CAMERA_SPEED / 10;

	if (!MelLib::Input::RightStickLeft(30.0f)
		&& !MelLib::Input::RightStickRight(30.0f)
		&& !MelLib::Input::RightStickUp(30.0f)
		&& !MelLib::Input::RightStickDown(30.0f))cameraSpeed = 0.0f;
	else cameraSpeed += FRAME_UP_CAMERA_SPEED;

	if (MelLib::Input::RightStickLeft(30.0f))addCameraAngle.y = -cameraSpeed;
	else if (MelLib::Input::RightStickRight(30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(30.0f))addCameraAngle.x = -cameraSpeed;
	else if (MelLib::Input::RightStickDown(30.0f))addCameraAngle.x = cameraSpeed;

	if (cameraSpeed >= MAX_CAMERA_SPEED)cameraSpeed = MAX_CAMERA_SPEED;

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
	/*pCamera->SetRotateCriteriaPosition
	(MelLib::LibMath::FloatDistanceMoveVector3
	(GetPosition(), MelLib::LibMath::OtherVector3(pCamera->GetCameraPosition(), pCamera->GetTargetPosition()), 30.0f));*/

	if (!lockOn)
	{
		pCamera->SetRotateCriteriaPosition(GetPosition() + MelLib::Vector3(0, 10, 0));
	}
}

void Player::LockOn()
{
	// �S�G���擾
	// �����ȓ����ŒZ�̂�ɃJ����������
	// ���b�N�I�����̓J�����͓������Ȃ�
	// �v���C���[���f�邩�A�����_��G�ɃZ�b�g���Ă��������ɉf��悤�ɂ���B
	// �����̃Q�[�����Q�l�ɂ����ق�����������
	// �v���C���[�ƓG�̕����x�N�g���ɉ����ăJ������]������΁A���������ɂȂ邩��?

	// �X�e�B�b�N�Ń��b�N�I���Ώۂ�؂�ւ����悤�ɂ��邱��

	// ���b�N�I���̍ō�����
	static const float LOCK_ON_DISTANCE = 200.0f;

	// ���b�N�I���L���A����
	if (MelLib::Input::PadButtonTrigger(MelLib::PadButton::R_STICK))
	{
		lockOn = !lockOn;
		if(lockOn)
		{
			lockOnEnemyDistance = FLT_MAX;
			// �߂��G���擾
			for (const auto& enemy : ActionPart::GetEnemys())
			{
				float distance = MelLib::LibMath::CalcDistance3D(GetPosition(), enemy->GetPosition());
				if (distance <= lockOnEnemyDistance)
				{
					lockOnEnemyDistance = distance;
					lockOnEnemy = enemy.get();
				}
			}

			// �G���Ȃ�������I��
			if (lockOnEnemyDistance == FLT_MAX)return;

			// �߂��ɂ��Ȃ�������I��
			if (lockOnEnemyDistance > LOCK_ON_DISTANCE)return;

		}
	}

	if (!lockOn)return;

	// �J��������
	// �J�����̓�����⊮���邱��

	// �܂��͎��_�؂�ւ���D�悵�Ď������邱��
	
	lockOnEnemyDistance = MelLib::LibMath::CalcDistance3D(GetPosition(), lockOnEnemy->GetPosition());

	// ���ꂽ�����
	if(lockOnEnemyDistance >= LOCK_ON_DISTANCE)
	{
		lockOn = false;
		return;
	}

	// �����_��G�̍��W��
	MelLib::Camera::Get()->SetRotateCriteriaPosition(lockOnEnemy->GetPosition());

	// �����_�Ƃ̋�����ς���
	MelLib::Camera::Get()->SetCameraToTargetDistance(lockOnEnemyDistance * 2.0f);


	MelLib::Vector3 playerToEnemy = lockOnEnemy->GetPosition() - GetPosition();
	float xzAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerToEnemy.x, playerToEnemy.z), true);

	static const float CONST_ANGLE_X = 35;
	/*float disAngle = lockOnEnemyDistance * 0.3f;
	if (disAngle - CONST_ANGLE_X > 90 - CONST_ANGLE_X)disAngle = 90 - CONST_ANGLE_X;*/
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(CONST_ANGLE_X , -xzAngle + 90,0));
}


void Player::Draw()
{
	modelObjects["main"].Draw();
}

void Player::Hit(const GameObject* const object, const MelLib::ShapeType3D& collisionType, const int arrayNum, const MelLib::ShapeType3D& hitObjColType, const int hitObjArrayNum)
{

	if (typeid(*object) == typeid(Ground)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		// ���n����
		if (!hitGroundNotMove)
		{
			float velocityY = GetVelocity().y;

			if (velocityY <= -3.0f)// ������
			{
				hitGroundNotMoveTimer.SetMaxTime(60 * 0.75);
				hitGroundNotMoveTimer.SetStopFlag(false);
				hitGroundNotMove = true;

				modelObjects["main"].SetAnimation("Jump_End_1");
			}
			jumpResetAnimation = false;
		}




		MelLib::Vector3 addPos;

		//velocity���߂��Ɩ߂��������Ⴄ
		//�J�v�Z���̉��̍��W-���a(��[���W)����Փ˓_�ւ̃x�N�g���̑傫�����߂���OK?

		//�d�͉����x���グ��
		//addPos.y += MelLib::GameObject::GetGravutationalAcceleration();

		//�����グ�����I��
		FallEnd();

		//�J�v�Z���̉��̐�[����Փ˓_�̃x�N�g�������߁A���̕������o���B
		//-0.15f�́A�����o���������Hit���Ă΂�Ȃ��Ȃ��āA��W�����v���̗����̏����ŉ������Ă�����������̂�h�~���邽�߂ɂ���
		//�����̐�[���q�b�g������J�v�Z���̐�[����ɗ���܂ŉ����Ԃ�����A-0.15�����Ȃ��Ƃ�����������
		/*addPos.y -= (capsuleData[0].GetSegment3DData().GetPosition().v2.y - capsuleData[0].GetRadius()) -
			capsuleData[0].GetSegment3DData().GetCalcResult().boardHitPos.y;*/


			//�J�v�Z������Ȃ��Đ����Ŕ������Ă�Ƃ��̏���
		addPos.y += segment3DData[0].GetCalcResult().boardHitPos.y - segment3DData[0].GetPosition().v2.y;
		//segment3DData[0] = capsuleData[0].GetSegment3DData();

		AddPosition(addPos);
		SetCameraPosition();

		hitGround = true;



		if (!setStartParam)
		{
			setStartParam = true;

			// �U������ړ��p
			// �U��������Z�b�g�����Ƃ��̃v���C���[�̃f�[�^��ۑ�
			startPos = modelObjects["main"].GetPosition();
			startAngle = modelObjects["main"].GetAngle();
			startScale = modelObjects["main"].GetScale();
		}
	}

	// �U�����󂯂���(�̗͌��Z��EnemyAttack���ōs���Ă�)
	if (!isMuteki)
	{

		if (typeid(*object) == typeid(EnemyAttack))
		{
			isMuteki = true;
			mutekiTimer.SetStopFlag(false);
		}
	}

}
