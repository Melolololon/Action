#include "Player.h"

#include"Input.h"
#include"Camera.h"
#include"LibMath.h"
#include"GameObjectManager.h"
#include"SceneManager.h"
#include"Title.h"

#include"ActionPart.h"

#include"PlayerSlush.h"
#include"JumpAttack.h"

#include"Stage.h"
#include"Ground.h"
#include"Wall.h"

#include"RecoveryEffect.h"
#include"RecoveryItem.h"

#include"Pause.h"
#include"EditMode.h"

#include"EnemyAttack.h"
#include"NormalEnemyAttack.h"
#include"CapsuleEnemyAttack.h"
#include"JumpEnemy.h"
#include"BossAttack.h"

#include"EventFlag.h"
#include"TutorialEventFlag.h"
#include"Pause.h"


#include<LibMath.h>



// �e�X�g
#include"SlushEffect.h"

#include"HPGauge.h"


std::unordered_map<Player::ActionType, MelLib::PadButton> Player::padConfigData =
{
	{ActionType::JUMP, MelLib::PadButton::A},
	{ActionType::ATTACK, MelLib::PadButton::X},
	{ActionType::DASH, MelLib::PadButton::B},
	{ActionType::GUARD, MelLib::PadButton::LB},
	{ActionType::DEATH_BLOW, MelLib::PadButton::Y}
};

std::unordered_map<Player::ActionType, BYTE> Player::keyboardConfigData =
{
	{ActionType::JUMP, DIK_SPACE},
	{ActionType::DASH, DIK_LSHIFT},
	{ActionType::DEATH_BLOW, DIK_LCONTROL}
};

std::unordered_map<Player::ActionType, MelLib::MouseButton> Player::mouseConfigData =
{
	{ActionType::ATTACK, MelLib::MouseButton::LEFT},
	{ActionType::GUARD, MelLib::MouseButton::RIGHT}
};

const float Player::JUMP_POWER = 3.0f;
const float Player::GROUND_HUND_VELOCITY = -30.0f;
Player* Player::pPlayer = nullptr;

const float Player::JUMP_ATTACK_DROP_SPEED = -3.0f;

std::unordered_map<PlayerSlush::AttackType, const Player::AttackData> Player::attackData =
{
	// AttackData(�p���[,�U���A�j���[�V�����I���܂ł̎���,���̍U�����͎���)
	{PlayerSlush::AttackType::NONE,AttackData(0,0,0 ,AttackEffect::NONE)},
	{PlayerSlush::AttackType::NORMAL_1,AttackData(3,20,10,AttackEffect::NONE)},
	{PlayerSlush::AttackType::NORMAL_2,AttackData(3,30,20,AttackEffect::NONE)},
	{PlayerSlush::AttackType::NORMAL_3,AttackData(5,30,20,AttackEffect::BE_BLOWN_AWAY)},// ����̃A�j���[�V�����o�O��̃A�j���[�V�����I���O�ɃL�����Z������ė����ɖ߂邩��?
	{PlayerSlush::AttackType::DASH_1,AttackData(3,20,10,AttackEffect::BE_BLOWN_AWAY)},
	{PlayerSlush::AttackType::JUMP,AttackData(5,9999,9999,AttackEffect::BE_BLOWN_AWAY)},
};



void Player::LoadResources()
{
	// �^�C�g���ł��g�����玩���폜�폜
	MelLib::ModelData::Load("Resources/Model/Player/Player.fbx", false, "Player");
	//MelLib::ModelData::Load("Resources/Model/Enemy/Mokuzin/Mokuzin.fbx", false, "Player");
	// ���b�V��2�ȏ�̎��Ƀ}�e���A���ǂނƃG���[�ł�
	// ��������Ȃ��Ƃ��ɃG���[�o����}�e���A���m�F���邱��
}


Player::Player(const MelLib::Vector3& pos)
	:GameObject("Player")
{

	//�������Z�̂��߂�seter��geter���?


	collisionFlag.capsule = true;
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(3.0f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>
			(GetPosition() + MelLib::Vector3(0, 15, 0), GetPosition() + MelLib::Vector3(0, -0.1f, 0)));

	segment3DDatas["main"].resize(3);
	segment3DDatas["main"][0] = capsuleDatas["main"][0].GetSegment3DData();


	const float SEGMENT_LENGTH = 6.0f;
	const float SEGMENT_HEIGTH = 60.0f;
	// �ǂƂ̔���
	segment3DDatas["main"][1].SetPosition
	(
		MelLib::Value2<MelLib::Vector3>
		(GetPosition() + MelLib::Vector3(0, SEGMENT_HEIGTH, SEGMENT_LENGTH), GetPosition() + MelLib::Vector3(0, SEGMENT_HEIGTH, -SEGMENT_LENGTH))
	);

	segment3DDatas["main"][2].SetPosition
	(
		MelLib::Value2<MelLib::Vector3>
		(GetPosition() + MelLib::Vector3(SEGMENT_LENGTH, SEGMENT_HEIGTH, 0), GetPosition() + MelLib::Vector3(-SEGMENT_LENGTH, SEGMENT_HEIGTH, 0))
	);



	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), nullptr);
	modelObjects["main"].Create(MelLib::ModelData::Get("Player"), "Player", nullptr);

	modelObjects["main"].SetAnimationPlayFlag(true);

#pragma region �_�b�V��
	dashEasing.SetAddPar(5.0f);


#pragma endregion

	mutekiTimer.SetMaxTime(60 * 1.0f);



	// �^�C�g���p����
	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (currentScene) 
	{
		if (typeid(*currentScene) == typeid(Title))
		{
			modelObjects["main"].SetScale(MelLib::Vector3(3));
			modelObjects["main"].SetAngle(0);
			modelObjects["main"].SetAnimation("No_Cont");
		}
		else
		{
			/*modelObjects["main"].SetScale(MelLib::Vector3(3));
			modelObjects["main"].SetAngle(0);
			modelObjects["main"].SetPosition(MelLib::Vector3(0, -17, -0));*/


			//�����h�~
			FallStart(0.0f);
			jumpStartPosition = GetPosition();
			// �����A�j���[�V��������X�^�[�g
			modelObjects["main"].SetAnimation("Jump_Up");
		}
	}



	pPlayer = this;
	HPGauge::SetPPlayer(this);

	jumpAttackEndTimer.SetMaxTime(60 * 0.5);


	SetCameraData();


	collisionCheckDistance = 500.0f;

	

	/*MelLib::DrawOption op;
	op.cullMode = MelLib::CullMode::NONE;
	downMaterial.Create(op, 1);
	downMaterial.SetTexture(modelObjects["main"].GetPMaterial("Zubon")->GetPTexture());
	modelObjects["main"].SetMaterial(&downMaterial);*/
}

void Player::Initialize()
{
	ShowCursor(FALSE);

	MelLib::Camera* pCamera = MelLib::Camera::Get();
	pCamera->SetRotatePoint(MelLib::Camera::RotatePoint::ROTATE_POINT_TARGET_POSITION);
	pCamera->SetCameraToTargetDistance(35.0f);

}

// �W�����v�U�����ɍU���H�炤�ƃo�O��
// T�|�[�Y�œ����Ȃ��Ȃ�
void Player::Update()
{
	preAttack = currentAttack;

	// �}�E�X�J�[�\���m�F
	//ChangeMouseCursorShow();


	MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())
	{
		if (EditMode::GetInstance()->GetIsEdit())UpdateCamera();
		
		MelLib::Scene* currentScene = MelLib::SceneManager::GetInstance()->GetCurrentScene();
		if (typeid(*currentScene) != typeid(Title))
		{
			SetCameraData();
		}
		return;
	}

	if (isHit)isMuteki = true;
	isHit = false;

	hitEventFlag = false;
	hitTutorialEventFlag = false;

	modelObjects["main"].Update();
	damageEffect.Update();
	
	
	if(isDead)
	{
		Dead();
		return;
	}


	if (isStun)
	{
		Stun();
		UpdateCamera();
		LockOn();
		DashEnd();
		return;
	}

	if (!setStartParam)
	{
		if (typeid(*currentScene) != typeid(Title))
		{
			JumpAnimation();
			ChangeAnimationData();
			UpdateCamera();
			CalcMovePhysics();
		}
		return;
	}

	if (hitGroundNotMove)
	{
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			hitGroundNotMove = false;
		}
		UpdateCamera();

		return;
	}

	prePosition = GetPosition();
	if (!GetIsFall() && !isDash)
	{
		// �O�t���[���̍��W�̊i�[�ꏊ�����炷
		for (int i = 0; i < _countof(notFallPrePosition) - 1; i++)
		{
			notFallPrePosition[i + 1] = notFallPrePosition[i];
		}
		notFallPrePosition[0] = prePosition;
	}


	// �A�j���[�V�����̏I���m�F�t���O���A�j���[�V�����̃t���[����0�ɂ��Ă��؂�ւ��Ȃ�(���̃t���[���ɂȂ�Ȃ���false�ɂȂ�Ȃ�)
	// ���炢�����񂱂��ɏ���

	// �����U������
//if (jumpAttackStartTimer.GetMaxOverFlag()) 
	if (modelObjects["main"].GetCurrentAnimationName() == "Jump_Attack"
		&& modelObjects["main"].GetAnimationEndFlag()
		&& currentAttack == PlayerSlush::AttackType::JUMP)
	{
		FallStart(JUMP_ATTACK_DROP_SPEED);
		jumpStartPosition = GetPosition();
		jumpAttackStartTimer.ResetTimeZero();
		jumpAttackStartTimer.SetStopFlag(true);
	}

	// �W�����v�U���I��
	if (modelObjects["main"].GetCurrentAnimationName() == "Jump_Attack_End"
		&& modelObjects["main"].GetAnimationEndFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		// �����I��
		attackTimer.SetNowTime(attackData[PlayerSlush::AttackType::JUMP].time);

		modelObjects["main"].SetAnimationFrame(0);
		modelObjects["main"].SetAnimation("No_Cont");

	}

	// �W�����v�U���I���A�j���[�V����
	if (jumpAttackEndTimer.GetMaxOverFlag())
	{
		modelObjects["main"].SetAnimation("Jump_Attack_End");
		modelObjects["main"].SetAnimationFrame(0);
		jumpAttackEndTimer.SetStopFlag(true);
		jumpAttackEndTimer.ResetTimeZero();

	}

	CreateAttackSlush();

	if (currentAttack == PlayerSlush::AttackType::NONE) 
	{
		Move();
		Jump();
	}
	else 
	{
		AttackMove();
	}

	preHitGround = hitGround;
	hitGround = false;

	CalcMovePhysics();


	ReturnStage();


	DeathBlow();
	if (isDeathBlow)return;


	//Guard();
	Attack();

	

	Muteki();

	UpdateCamera();
	//LockOn();

	float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	// ��]����(��)
	//modelObjects["main"].SetAngle(MelLib::Vector3(0, angle, 0));
	SetAngle(MelLib::Vector3(0, angle, 0));


	// �A�j���[�V�����ɍ��킹�čĐ����x�⃋�[�v��ݒ肷��
	ChangeAnimationData();

	// ����������ǉ�
	if(GetIsFall())
	{
		dropStartPosY -= prePosition.y - GetPosition().y;
	}

	// ���̐��l�ȏ㗎�������痎������
	static const float DROP_POS_Y = 10.0f;

	// ���ʗ��������痎������
	if (dropStartPosY >= DROP_POS_Y)isDrop = true;



}

bool Player::GetAttackChangeFrame() const
{
	return preAttack != currentAttack;
}

void Player::TitleUpdate()
{
	// �ړ�
	AddPosition(MelLib::Vector3(0, 0, 0.5f));


	// �W�����v
	if (!GetIsFall())
	{
		FallStart(JUMP_POWER);
		jumpStartPosition = GetPosition();
		modelObjects["main"].SetAnimation("Jump_Up");
	}
	CalcMovePhysics();
	ChangeAnimationData();
}

MelLib::Vector3 Player::CalcPlayerVector(const MelLib::Vector3& pos)const
{
	MelLib::Vector3 p = GetPosition();
	return MelLib::Vector3::Normalize(p - pos);
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
	else if (animationName.find("Dead") != std::string::npos)
	{
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}

	//// �f�o�b�O�p
	//if (MelLib::Input::KeyTrigger(DIK_1))
	//{
	//	isTPause = !isTPause;
	//}

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

	// �_�b�V�����܂��̓W�����v�U�����͈ړ��s��
	if (isDash || currentAttack == PlayerSlush::AttackType::JUMP || isGuard)
	{
		return;
	}
	else if (currentAttack != PlayerSlush::AttackType::NONE)
	{
		return;
	}


	static const float FAST_WARK_STICK_PAR = 60.0f;
	static const float WALK_STICK_PAR = 20.0f;
	static const float MAX_FAST_WARK_SPEED = 3.0f;
	static const float MAX_WALK_SPEED = 0.5f;
	static const float FRAME_UP_FAST_WARK_SPEED = MAX_FAST_WARK_SPEED / 10;
	static const float FRAME_UP_WARK_SPEED = MAX_WALK_SPEED / 10;



	// �X������ړ�
	if (MelLib::Input::LeftStickUp(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickDown(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickRight(WALK_STICK_PAR)
		|| MelLib::Input::LeftStickLeft(WALK_STICK_PAR))
	{
		direction = MelLib::Input::LeftStickVector3(20.0f, MelLib::Camera::Get(), false, true);
		direction.y = 0.0f;
		MelLib::Vector3 addPos = direction;

		//�_�b�V���̌X���ʂ𒴂��Ă�����_�b�V��
		if (MelLib::Input::LeftStickUp(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickDown(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickRight(FAST_WARK_STICK_PAR)
			|| MelLib::Input::LeftStickLeft(FAST_WARK_STICK_PAR))
		{
			// ����܂ł��񂾂���Z���Ĉړ����x���グ�Ă���
			if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
			else moveSpeed = MAX_FAST_WARK_SPEED;
			addPos *= moveSpeed;

			modelObjects["main"].SetAnimation("Dash");
		}
		else
		{
			// ����܂ł��񂾂���Z���Ĉړ����x���グ�Ă���
			if (moveSpeed < MAX_WALK_SPEED)moveSpeed += FRAME_UP_WARK_SPEED;
			else moveSpeed = MAX_WALK_SPEED;
			addPos *= moveSpeed;
			addPos *= MAX_WALK_SPEED;

			modelObjects["main"].SetAnimation("Walk");
		}


		AddPosition(addPos);
	}
	else if (MelLib::Input::KeyState(DIK_W)
		|| MelLib::Input::KeyState(DIK_A)
		|| MelLib::Input::KeyState(DIK_S)
		|| MelLib::Input::KeyState(DIK_D)) 
	{
		MelLib::Vector3 moveVector;
		// �L�[�ɉ����Ĉړ�������ݒ�
		if (MelLib::Input::KeyState(DIK_W)) moveVector.z += 1;
		else if (MelLib::Input::KeyState(DIK_S))moveVector.z += -1;
		if (MelLib::Input::KeyState(DIK_A)) moveVector.x += -1;
		else if (MelLib::Input::KeyState(DIK_D)) moveVector.x += 1;
		
		// �J�����ɍ��킹�ĉ�]
		moveVector = MelLib::LibMath::RotateZXYVector3(moveVector,MelLib::Camera::Get()->GetAngle());
		moveVector.y = 0.0f;

		direction = moveVector;
		
		// ���Z�l�Ɍ�������
		MelLib::Vector3 addPos = direction;

		if (moveSpeed < MAX_FAST_WARK_SPEED)moveSpeed += FRAME_UP_FAST_WARK_SPEED;
		else moveSpeed = MAX_FAST_WARK_SPEED;

		// ���x�����Z
		addPos *= moveSpeed;
		AddPosition(addPos);

		modelObjects["main"].SetAnimation("Dash");
	}
	else
	{
		moveSpeed = 0.0;

		modelObjects["main"].SetAnimation("No_Cont");

		// �A�j���[�V�����m�F�p
		//modelObjects["main"].SetAnimation("Jump_Attack");
	}


	//if (position.y <= -100)position.y = 100;

	//�������邽�߂�
	if (!GetIsFall())
	{
		jumpStartPosition = GetPosition();
		FallStart(0.0f);
	}


}

void Player::ReturnStage()
{

	// �J�n�ʒu�҂�����ɖ߂��ƁA�W�����v�����ɗ��������ɁA
	// �����J�n���͂��łɑ����ɑ��ꂪ�Ȃ������ɗ��������邽�߁A�Ώ����K�v
	if(GetPosition().y <= -300)
	{
		//SetPosition(notFallPrePosition[_countof(notFallPrePosition) - 1]);
		

		//DownHP(30);


		AddPosition(MelLib::Vector3(0, 500, 0));
	}
}

void Player::AttackMove()
{

	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:
		AddPosition(direction * 0.2f);
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		AddPosition(direction * 0.3f);
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		AddPosition(direction * 0.3f);
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

	static const float DASH_DISTANCE = 100.0f;

	if ((MelLib::Input::PadButtonTrigger(padConfigData[ActionType::DASH]) 
		|| MelLib::Input::KeyState(keyboardConfigData[ActionType::DASH]))
		&& !isDash
		&& currentAttack == PlayerSlush::AttackType::NONE
		&& !isGuard)
	{
		isDash = true;

		dashEasing.SetStart(GetPosition());
		dashEasing.SetEnd(MelLib::LibMath::FloatDistanceMoveVector3(GetPosition(), direction, DASH_DISTANCE));
	}

	if (isDash)
	{
		MelLib::Vector3 easingPrePos = GetPosition();
		MelLib::Vector3 pos = dashEasing.EaseInOut();
		MelLib::Vector3 addPos = pos - easingPrePos;

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
		DashEnd();
	}
}

void Player::DashEnd()
{
	dashEasing.SetPar(0.0f);
	isDash = false;
}

void Player::Jump()
{
	// �W�����v�̒��n�A�j���[�V�����́A�W�����v�̋t�Đ���OK

	if (attackTimer.GetNowTime() != 0 
		|| isDash
		|| isGuard
		|| isStun)return;

	if (MelLib::Input::PadButtonTrigger(padConfigData[ActionType::JUMP])
		|| MelLib::Input::KeyState(keyboardConfigData[ActionType::JUMP]))
	{
		FallStart(JUMP_POWER);
		jumpStartPosition = GetPosition();
	}


	JumpAnimation();
}

void Player::JumpAnimation()
{

	// ��������������W�����v�A�j���[�V����
	if (!hitGround && !preHitGround && !isDash && currentAttack == PlayerSlush::AttackType::NONE && isDrop)
	{
		if (!jumpResetAnimation)
		{
			jumpResetAnimation = true;
			modelObjects["main"].SetAnimationFrameStart();
		}
		modelObjects["main"].SetAnimation("Jump_Up");
	}
}



void Player::Attack()
{


	// �󒆍U���ƃ_�b�V���U���͂�������Ȃ�
	//if (isDrop)return;

	if (attackTimer.GetMaxOverFlag())
	{
		currentAttack = PlayerSlush::AttackType::NONE;
		attackTimer.ResetTimeZero();

		attackTimer.SetStopFlag(true);
	}

	//if��2�s�ڂ̃^�C�}�[�m�F�́A�R���{�I�����NONE�ɂ��邽�߁A���̍U�����ɓ���Ȃ��悤�ɂ��邽�߂ɏ����Ă�
	if ((MelLib::Input::PadButtonTrigger(padConfigData[ActionType::ATTACK])
		|| MelLib::Input::MouseButtonTrigger(mouseConfigData[ActionType::ATTACK]))
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


			//�A�j���[�V���������Z�b�g
			modelObjects["main"].SetAnimationFrameStart();
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

			modelObjects["main"].SetAnimation("DashAttack");

			isDash = false;
			DashEnd();
		}
		else if (preHitGround)// �ʏ�U��
		{
			currentAttack = PlayerSlush::AttackType::NORMAL_1;

			modelObjects["main"].SetAnimation("Attack_Normal_1");

		}
		else if (GetIsFall() && abs(GetPosition().y - jumpStartPosition.y) >= 10 )// �󒆍U��
		{
			currentAttack = PlayerSlush::AttackType::JUMP;
			FallEnd();
			modelObjects["main"].SetAnimation("Jump_Attack");
			modelObjects["main"].SetAnimationFrame(0);
			jumpAttackStartTimer.SetStopFlag(false);
			jumpAttackStartTimer.SetMaxTime(60 * 0.5);
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
	const int CURRENT_FRAME = modelObjects["main"].GetAnimationFrame();
	const int MAX_FRAME = modelObjects["main"].GetAnimationFrameCount();
	bool addFrame = false;

	// �U���A�j���[�V�����͑S��2�{��������CURRENT_FRAME��if�͋����ɂ���
	switch (currentAttack)
	{
	case PlayerSlush::AttackType::NONE:
		break;
	case PlayerSlush::AttackType::NORMAL_1:

		if (CURRENT_FRAME == 14)
		{
			// MAX_FRAME / 2��/2�̓A�j���[�V�������x�@�������ƂŊ֐��Ŏ擾����悤�ɕύX����
			pRigthSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"],
					startPos, startAngle, startScale, false,"N1");
			addFrame = true;
		}
		break;
	case PlayerSlush::AttackType::NORMAL_2:
		if (CURRENT_FRAME == 24)
		{

			pPSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"], startPos, startAngle, startScale, true, "N2");

			addFrame = true;
		}
		break;
	case PlayerSlush::AttackType::NORMAL_3:
		if (CURRENT_FRAME == 18)
		{
			pPSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"], startPos, startAngle, startScale, true, "N3_L");

			pRigthSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME / 2, modelObjects["main"], startPos, startAngle, startScale, false, "N3_R");

			addFrame = true;
		}
		break;
	case PlayerSlush::AttackType::DASH_1:
		if (CURRENT_FRAME == 14)
		{
			pRigthSlush = std::make_shared<PlayerSlush>
				(GetPosition(), direction, currentAttack, MAX_FRAME, modelObjects["main"], startPos, startAngle, startScale, false, "D1");

			addFrame = true;
		}
		break;
	default:
		break;
	}

	if (addFrame)MelLib::GameObjectManager::GetInstance()->AddObject(pPSlush);
	if (addFrame)MelLib::GameObjectManager::GetInstance()->AddObject(pRigthSlush);
}

void Player::CheckEraseSlush()
{
	if (!GetAttackChangeFrame())return;

	// �؂�ւ��ō폜
	if (pPSlush)
	{
		pPSlush->Erase();
		pPSlush.reset();
	}
	if (pRigthSlush)
	{
		pRigthSlush->Erase();
		pRigthSlush.reset();
	}
}

void Player::DeathBlow()
{


	/*if (!isDeathBlow)return;

	if (modelObjects["main"].GetAnimationEndFlag()) 
	{
		std::string aniName = "DeathBlowAttack_";
		switch (currentAttack)
		{

			case PlayerSlush::AttackType::DEATH_BLOW_1:
				aniName += "2";
				break;

			case PlayerSlush::AttackType::DEATH_BLOW_2:
				aniName += "3";
				break;
			case PlayerSlush::AttackType::DEATH_BLOW_3:
				aniName = "No_Cont";
				break;

			default:
				aniName += "1";
				break;
		}

		modelObjects["main"].SetAnimation(aniName);
		modelObjects["main"].SetAnimationFrame(0);
	}*/


}


void Player::Guard()
{
	// �U�����ĂȂ����Ƀ{�^�������Ă���K�[�h
	if (MelLib::Input::PadButtonState(padConfigData[ActionType::GUARD])
		&& currentAttack == PlayerSlush::AttackType::NONE
		&& !isDash
		&& !isStun)
	{
		isGuard = true;
		modelObjects["main"].SetAnimation("Guard");
	}
	else
	{
		isGuard = false;
	}
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

void Player::Stun()
{
	if (isBeBlownAway) 
	{
		static const float BE_BLOWN_AWAY_SPEED = 0.75f;
		AddPosition(beBlownAwayVector * BE_BLOWN_AWAY_SPEED);

		if (!GetIsFall())isBeBlownAway = false;
	}

	if(modelObjects["main"].GetAnimationEndFlag())
	{
		isStun = false;
	}
}

void Player::Dead()
{
	modelObjects["main"].SetAnimation("Dead");
	ChangeAnimationData();

	//if (!showMouse)
	//{
	//	ShowCursor(TRUE);
	//}
}

void Player::UpdateCamera()
{

	// �}�E�X�J�[�\���������Ă���(�|�[�Y���Ȃ�)��������return
	if (showMouse)return;

	// �J������]����
	RotCamera();
	// �J�����ɒl�Z�b�g
	SetCameraData();

}

void Player::ChangeMouseCursorShow()
{
	if (!setStartParam)return;

	if(Pause::GetInstance()->PauseTiming())
	{
		showMouse = !showMouse;
		ShowCursor(showMouse);
	}
}

void Player::DrawDamageEffect()
{
	damageEffect.Draw();
}

void Player::RotCamera()
{

	if (lockOn)return;

	//�X�e�B�b�N�̓|����ŃX�s�[�h�ς���
	//�ɋ}�������ق�������?���񂾂�������Ă��B���߂ɃX�s�[�h�}�b�N�X�ɂȂ�

	//���C���J�����̃|�C���^�擾
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	MelLib::Vector3 cameraAngle = pCamera->GetAngle();
	MelLib::Vector3 addCameraAngle = 0.0f;
	const float MAX_CAMERA_ANGLE_X = 30.0f;
	const float MIX_CAMERA_ANGLE_X = -5.0f;

	//�ő�l�͈�Uconst(���ƂŐݒ�ōő�l��ς�����悤�ɂ���)
	const float MAX_CAMERA_SPEED = 3.0f;
	const float FRAME_UP_CAMERA_SPEED = MAX_CAMERA_SPEED / 10;

	if (!MelLib::Input::RightStickLeft(30.0f)
		&& !MelLib::Input::RightStickRight(30.0f)
		&& !MelLib::Input::RightStickUp(30.0f)
		&& !MelLib::Input::RightStickDown(30.0f))cameraSpeed = 0.0f;
	else cameraSpeed += FRAME_UP_CAMERA_SPEED;

	// �p�b�h�p
	if (MelLib::Input::RightStickLeft(30.0f))addCameraAngle.y = -cameraSpeed;
	else if (MelLib::Input::RightStickRight(30.0f))addCameraAngle.y = cameraSpeed;
	if (MelLib::Input::RightStickUp(30.0f))addCameraAngle.x = -cameraSpeed;
	else if (MelLib::Input::RightStickDown(30.0f))addCameraAngle.x = cameraSpeed;
	
	bool padInput = addCameraAngle != 0.0f;

#pragma region �L�[�{�[�h�p�J��������
	
	if (!padInput) 
	{

		// �E�B���h�E�T�C�Y���擾
		MelLib::Vector2 winSize(MelLib::Library::GetWindowWidth(), MelLib::Library::GetWindowHeight());
		
		MelLib::Vector2 winPos = MelLib::Library::GetWindowPosition();


		MelLib::Vector2 winHarf = winSize / 2;

		// �E�B���h�E�������Ă��邱�Ƃ��l�����č��W�����Z
		winHarf += winPos;

		// �}�E�X�̃N���C�A���g���W���擾
		MelLib::Vector2 mousePos = MelLib::Input::GetMousePosition() + winPos;

		// �E�B���h�E�������ƃo�O�邩��E�B���h�E���W�擾���ĕ␳���đ΍􂷂邩�t���X�N�{�[�_�[���X�ɂ���̂���������
		// �E�B���h�E�̃o�[�����W������邽��,���Z���Ē���
		mousePos.x += 8.0f;
		mousePos.y += 22.0f;

		// �J��������������Z�b�g
		if (!MelLib::LibMath::Difference(mousePos.x, winHarf.x, 4)
			|| !MelLib::LibMath::Difference(mousePos.y, winHarf.y, 4))
		{
			cameraSpeed += FRAME_UP_CAMERA_SPEED;

			// �����ŃJ�����̈ړ��ʂ��v�Z
			MelLib::Vector2 moveVector = MelLib::Vector2(mousePos - winHarf).Normalize() * cameraSpeed;

			// �p�b�h�̑��x���g���܂킷�ƒx�����߁A���l���|���đ��x���グ��
			const float MUL_CAMERA_SPEED = 20.0f;
			moveVector *= MUL_CAMERA_SPEED;

			// �p�x���Z�b�g
			addCameraAngle = MelLib::Vector3(moveVector.y, moveVector.x, 0);

			frameMousePosition = mousePos;
		}
		else cameraSpeed = 0.0f;

		// ���S�ɃJ�[�\�����ړ�
		if (!showMouse)MelLib::Input::SetMouseFixedPosition(winHarf);

		
	}
#pragma endregion

	if (cameraSpeed >= MAX_CAMERA_SPEED)cameraSpeed = MAX_CAMERA_SPEED;

	cameraAngle += addCameraAngle;
	if (cameraAngle.x >= MAX_CAMERA_ANGLE_X)cameraAngle.x = MAX_CAMERA_ANGLE_X;
	if (cameraAngle.x <= MIX_CAMERA_ANGLE_X)cameraAngle.x = MIX_CAMERA_ANGLE_X;

	pCamera->SetAngle(cameraAngle);
	
}


void Player::SetCameraData()
{
	MelLib::Camera* pCamera = MelLib::Camera::Get();

	if (lockOn)
	{
	}
	else 
	{
		MelLib::Vector3 targetPos = GetPosition() + MelLib::Vector3(0, 15, 0);

		// �������̃J��������
		if (isDrop)
		{
			MelLib::Vector3 jumpUpCameraVector = 0;
			targetPos.y -= (GetPosition().y - jumpStartPosition.y) / 7;
		}

		pCamera->SetRotateCriteriaPosition(targetPos);
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
				if (enemy->GetEraseManager())continue;

				float distance = MelLib::LibMath::CalcDistance3D(GetPosition(), enemy->GetPosition());
				if (distance <= lockOnEnemyDistance)
				{
					lockOnEnemyDistance = distance;
					lockOnEnemy = enemy.get();
				}
			}

			// �G���Ȃ�������A�܂��́A�߂��ɂ��Ȃ�������I��
			
			if (lockOnEnemyDistance == FLT_MAX || lockOnEnemyDistance > LOCK_ON_DISTANCE)
			{
				LockOnEnd();
				return;
			}

		}
	}

	// �؂�ւ�
	if(MelLib::Input::RightStickRight(40.0f))
	{
	}
	else if(MelLib::Input::RightStickLeft(40.0f))
	{
	}

	// ���b�N�I�������������A�܂��́A���b�N�I�������G�������nullptr�ɂȂ�����return
	if (!lockOn || !lockOnEnemy)return;

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

	// �߂Â�����E�n
	static const float MIN_DISTANCE = 50.0f;
	if (lockOnEnemyDistance < MIN_DISTANCE)lockOnEnemyDistance = MIN_DISTANCE;

	// �����_�Ƃ̋�����ς���
	MelLib::Camera::Get()->SetCameraToTargetDistance(lockOnEnemyDistance * 2.0f);


	MelLib::Vector3 playerToEnemy = lockOnEnemy->GetPosition() - GetPosition();
	float xzAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(playerToEnemy.x, playerToEnemy.z), true);

	static const float CONST_ANGLE_X = 35;
	/*float disAngle = lockOnEnemyDistance * 0.3f;
	if (disAngle - CONST_ANGLE_X > 90 - CONST_ANGLE_X)disAngle = 90 - CONST_ANGLE_X;*/
	MelLib::Camera::Get()->SetAngle(MelLib::Vector3(CONST_ANGLE_X , -xzAngle + 90,0));
}

void Player::HitWall()
{
	AddPosition(MelLib::Vector3(prePosition.x - GetPosition().x, 0, prePosition.z - GetPosition().z));
	DashEnd();
}

void Player::StartBeBlownAway(const MelLib::Vector3& hitObjPos)
{
	// ������уA�j���[�V����
	modelObjects["main"].SetAnimation("BeBlownAway");

	// �������(XZ)
	MelLib::Vector3 attackPos = hitObjPos;
	beBlownAwayVector = -(attackPos - GetPosition());
	beBlownAwayVector.y = 0;
	isBeBlownAway = true;

	// �������(Y)
	FallStart(2.0f);
}


void Player::Draw()
{
	AllDraw();
	//modelObjects["main"].Draw();
}

void Player::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	// �G�Ɠ����������̏���
	for (const auto& tag : object.GetTags()) 
	{
		if(tag == "Enemy" || tag == "StageObject")
		{
			// �^�ォ��ړ������ɏ�������Ă������o���悤�ɂ���
			// �G���玩���ւ̃x�N�g�������߁A���̕����ɉ����o���B
			// �^��̏ꍇ�́A�K���ɉ����o��
			MelLib::Vector3 enemyToPlayer = GetPosition() - object.GetPosition();
			enemyToPlayer = enemyToPlayer.Normalize();

			if(GetIsFall())
			{
				AddPosition(MelLib::Vector3(enemyToPlayer.x, 0, enemyToPlayer.z) * 5.0f);
				
			}
			else if (prePosition - GetPosition() == 0)
			{
				if (typeid(object) == typeid(JumpEnemy))
				{
					AddPosition(MelLib::Vector3(1,0,0) * 1.0f);
				}
				else 
				{
					AddPosition(MelLib::Vector3(enemyToPlayer.x, 0, enemyToPlayer.z) * 4.0f);
				}
			}
			else 
			{
				
				AddPosition(MelLib::Vector3(prePosition.x - GetPosition().x, 0, prePosition.z - GetPosition().z));
				
			}
		}

		if (tag == "StageObject")
		{
			DashEnd();
		}
	}


	// �������X�e�[�W�̔���ɓ������������
	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{
		// �X�e�[�W�̎O�p�`�̖@���擾
		MelLib::Vector3 normal = GetHitTriangleData().GetNormal();
		
		// ���ȏゾ�����珰����
		if(normal.y >= 0.6f)
		{
			// 0�ԈȊO�̔���(���p����)�ӊO�Ɠ���������return
			//if (arrayNum != 0)return;

			// �������������Z�b�g
			dropStartPosY = 0.0f;

			// ���n����
			if (!hitGroundNotMove)
			{
				float velocityY = GetVelocity().y;

				// �����Ƃ��납�痎�������������߁A�����Ȃ�����
				if (velocityY <= GROUND_HUND_VELOCITY)
				{
					hitGroundNotMove = true;

					modelObjects["main"].SetAnimation("Jump_End_1");
					modelObjects["main"].SetAnimationSpeedMagnification(1);

					// �_�b�V�����ɒn�ʂɂ����狭���I��
					DashEnd();
				}
				jumpResetAnimation = false;

				isDrop = false;
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


			// �J�v�Z������Ȃ��Đ����Ŕ������Ă�Ƃ��̏���
			float tHitPosY = GetSegmentCalcResult().triangleHitPos.y;
			addPos.y += tHitPosY - segment3DDatas["main"][0].GetPosition().v2.y;
			//segment3DData[0] = capsuleData[0].GetSegment3DData();

			AddPosition(addPos);
			SetCameraData();

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


			if (modelObjects["main"].GetCurrentAnimationName() == "Jump_Attack" && jumpAttackEndTimer.GetNowTime() == 0)
			{
				jumpAttackEndTimer.SetStopFlag(false);

				// �U������ǉ�
				MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<JumpAttack>(GetPosition(),70.0f));
			}

		}
		else // ��������Ȃ�������ǈ���
		{
			HitWall();
		}
	
	}

	if(typeid(object) == typeid(Wall)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{
		HitWall();
	}


	// �G�t�F�N�g�ǉ�
	if (typeid(object) == typeid(RecoveryItem)) 
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<RecoveryEffect>(GetPosition()));
	}


	// �U�����󂯂���(�̗͌��Z��EnemyAttack���ōs���Ă�)
	if (!isMuteki)
	{
		// �ʏ�
		if (typeid(object) == typeid(EnemyAttack)
			|| typeid(object) == typeid(NormalEnemyAttack)
			|| typeid(object) == typeid(CapsuleEnemyAttack))
		{
			damageEffect.StartEffect();
			isHit = true;
			mutekiTimer.SetStopFlag(false);

			
			isStun = true;

			for (auto& tag : object.GetTags())
			{
				if (EnemyAttack::GetAttackType(tag) == EnemyAttack::AttackType::BE_BLOWN_AWAY)
				{
					StartBeBlownAway(object.GetPosition());
				}
				else if(!isDrop)
				{
					modelObjects["main"].SetAnimation("Stun");
				}
			}

		}

		// �������
		if (typeid(object) == typeid(BossAttack))
		{
			damageEffect.StartEffect();
			isHit = true;
			mutekiTimer.SetStopFlag(false);

			isStun = true;

			StartBeBlownAway(object.GetPosition());
		}
		
		
	}

	if(typeid(object) == typeid(EventFlag))
	{
		hitEventFlag = true;
	}

	if (typeid(object) == typeid(TutorialEventFlag))
	{
		hitTutorialEventFlag = true;
	}
}

std::shared_ptr<MelLib::GameObject> Player::GetNewPtr() 
{
	return std::make_shared<Player>();
}


void Player::DownHP(const int power)
{
	if (isMuteki)return;

	hp -= power;
	if (hp <= 0)
	{
		hp = 0;
		isDead = true;

		modelObjects["main"].SetAnimationFrameStart();
		modelObjects["main"].SetAnimationEndStopFlag(true);
		modelObjects["main"].SetAnimation("Dead");
	}
}

void Player::LifeUp(const int upNum)
{
	hp += upNum;
	if (hp > HP_MAX)
	{
		hp = HP_MAX;
	}
}
