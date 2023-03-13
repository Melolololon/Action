#include "Boss.h"

#include"Stage.h"
#include"LibMath.h"

#include"EnemyAttack.h"
#include"BossAttack.h"

#include"JumpAttack.h"

#include"SlushHitEffect.h"

#include<GameObjectManager.h>

Player* Boss::pPlayer;

void Boss::Move()
{
	static const float MOVE_SPEED = 0.4f;
	
	// �߂��Ɉړ�
	MoveToPlayer(MOVE_SPEED);
}

void Boss::SelectAction()
{
	// �U������������X�L�b�v
	if (currentState != Boss::CurrentState::NONE)return;
	else if (actionTimer.GetMaxOverFlag())// �^�C�}�[�����ȏ�ɂȂ�����W�����v�U��
	{
		actionTimer.SetStopFlag(true);

		currentState = Boss::CurrentState::JUMP_ATTACK;
		modelObjects["main"].SetAnimation("Attack_Jump.001");

		return;

	}
	else actionTimer.SetStopFlag(false);


	MelLib::Vector3 myPos = GetPosition();
	MelLib::Vector3 playerPos = pPlayer->GetPosition();

	float playerDir = MelLib::LibMath::CalcDistance2D(MelLib::Vector2(myPos.x, myPos.z), MelLib::Vector2(playerPos.x, playerPos.z));

	static const float MIN_DIR = 30.0f;
	
	// ����������ړ�
	if(playerDir >= MIN_DIR)Move();
	else if (playerDir < MIN_DIR && playerDir >= 6.0f) // �����ł͂Ȃ��Ȃ�U�� 
	{
		currentState = Boss::CurrentState::ROLL_ATTACK;
		modelObjects["main"].SetAnimation("Attack_Roll");
	}
	else 
	{
		
		currentState = Boss::CurrentState::NORMAL_1;
		modelObjects["main"].SetAnimation("Attack");
	}
	
}

void Boss::AttackUpdate()
{
	switch (currentState)
	{
	case Boss::CurrentState::NORMAL_1:
		NormalAttackUpdate();
		break;
	case Boss::CurrentState::ROLL_ATTACK:
		RollAttackUpdate();
		break;
	case Boss::CurrentState::JUMP_ATTACK:
		JumpAttackUpdate();
		break;
	default:
		break;
	}

	// �W�����v�U���̂Ƃ��̏���
	if (jumpAttackTimer.GetMaxOverFlag())
	{

		static const float ONE_ATTACK_MOVE_DIR = 20.0f;

		MelLib::Vector3 AttackPos = MelLib::LibMath::FloatDistanceMoveVector3
		(
			GetPosition(),
			jumpAttackDir,
			(jumpAttackCount + 2) * ONE_ATTACK_MOVE_DIR
		);

		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<BossAttack>(AttackPos));

		jumpAttackCount++;
		jumpAttackTimer.ResetTimeZero();

	}
	if (jumpAttackCount >= JUMP_ATTACK_MAX_COUNT)
	{
		jumpAttackCount = 0;
		jumpAttackTimer.SetStopFlag(true);
	}
}

void Boss::AttackEnd()
{
	// �W�����v�U���I������
	if (modelObjects["main"].GetAnimationEndFlag())
	{
		currentState = CurrentState::NONE;
		modelObjects["main"].SetAnimation("No_Cont");

		actionTimer.SetStopFlag(false);
		actionTimer.ResetTimeZero();
	}
}

void Boss::NormalAttackUpdate()
{
	AttackEnd();
}

void Boss::RollAttackUpdate() 
{
	const int FRAME = modelObjects["main"].GetAnimationFrame();
	const int ROLL_START_FLAME = 31;

	if (FRAME == ROLL_START_FLAME)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject
		(std::make_shared<EnemyAttack>(10,modelObjects["main"],10.0f,EnemyAttack::AttackType::BE_BLOWN_AWAY));
	}

	if (FRAME >= ROLL_START_FLAME && FRAME <= 56)
	{
		MoveToPlayer(2.5f);
	}

	AttackEnd();
}

void Boss::JumpAttackUpdate()
{
	

	if (modelObjects["main"].GetAnimationFrame() == 17) FallStart(3.0f);

	if (modelObjects["main"].GetAnimationFrame() == 47) 
	{
		jumpAttackTimer.SetStopFlag(false);

	}

	AttackEnd();
	
}

void Boss::Rotate()
{
	// �i�s�����Ɍ����悤��
	MelLib::Vector3 directionP = (pPlayer->GetPosition() - GetPosition()).Normalize();
	
	if (jumpAttackTimer.GetNowTime() == 0)
	{
		jumpAttackDir = MelLib::Vector3(directionP.x, 0, directionP.z);
	}

	// ��]
	float directionAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(directionP.x, directionP.z), false) - 270;
	float preAngle = modelObjects["main"].GetAngle().y;

	// �v���C���[�̌����̊p�x�ƍ��̊p�x��5�x�ȓ���������return
	if (MelLib::LibMath::AngleDifference(directionAngle, preAngle, 5))return;

	SetAngle(MelLib::Vector3(0, directionAngle, 0));

	//// �����̌v�Z
	//MelLib::Vector2 angle2 = MelLib::LibMath::AngleToVector2(GetAngle().y + 90, true);
	//direction = MelLib::Vector3(angle2.x, 0, angle2.y);
}

void Boss::MoveToPlayer(const float speed)
{
	MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(GetPosition());
	MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * speed;
	AddPosition(moveVector);
}

void Boss::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx",true,"boss");
}

Boss::Boss()
	:GameObject("boss")
{
	modelObjects["main"].Create(MelLib::ModelData::Get("boss"), "boss", nullptr);
	modelObjects["main"].SetAnimation("No_Cont");


	//modelObjects["main"].SetPosition();
	//sphereDatas["main"].resize(3);


	actionTimer.SetResetFlag(false);



	actionTimer.SetMaxTime(60 * 3);
	jumpAttackTimer.SetMaxTime(60 * 0.1f);
	jumpAttackTimer.SetResetFlag(false);

	// ���ݒ�
	/*modelObjects["main"].SetAnimation("Attack_Jump.001");
	currentAttack = CurrentAttack::JUMP;*/


	hp.SetData(100, "Boss", "HP", 1, 300);

}

std::shared_ptr<MelLib::GameObject> Boss::GetNewPtr()
{
	return std::make_shared<Boss>();
}

void Boss::Initialize()
{
	FallStart(0.0f);

	// �����蔻��̍쐬
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(GetPosition() + MelLib::Vector3(0, 25.0f, 0), GetPosition() + MelLib::Vector3(0,-3.0f, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());


	// ��
	thisState = ThisState::BATTLE;
}

void Boss::Update()
{
	modelObjects["main"].Update();
	modelObjects["main"].SetAnimationPlayFlag(true);

	if (currentState == Boss::CurrentState::NONE) Rotate();

	SelectAction();
	AttackUpdate();
	CalcMovePhysics();
}

void Boss::Draw()
{
	AllDraw();
}

void Boss::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	//// 0�ɂȂ�������ꏈ��
	if (hp.GetValue() <= 0)
	{
		/*state = ThisState::DEAD;

		modelObjects["main"].SetAnimation("Dead");
		modelObjects["main"].SetAnimationFrameStart();
		modelObjects["main"].SetAnimationEndStopFlag(true);*/

		eraseManager = true;

		return;
	}


	std::string n = typeid(object).name();
	if (typeid(object) == typeid(PlayerSlush) || typeid(object) == typeid(JumpAttack) /* && !isMuteki*/)
	{
		MelLib::Vector3 toCameraVec = (MelLib::Camera::Get()->GetCameraPosition() - GetPosition()).Normalize();
		MelLib::Vector3 effectAddPos = MelLib::Vector3(0, 6, 0) + toCameraVec * 2.1f;
		MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<SlushHitEffect>
			(GetPosition() + effectAddPos));

		// �v���C���[���猻�݂̍U���̍U���͂��擾���A�̗͂����炷
		hp.SetValue(hp.GetValue() - pPlayer->GetCurrentAttackPower());

	}


	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//�����グ�����I��
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);

	/*	if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
		{
			currentThisAttackEffect = AttackEffect::NONE;

			state = ThisState::GET_UP;
			modelObjects["main"].SetAnimation("BeBlownAway_2");
			modelObjects["main"].SetAnimationFrameStart();
		}*/
	}
}
