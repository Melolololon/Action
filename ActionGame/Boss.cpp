#include "Boss.h"

#include"Stage.h"
#include"LibMath.h"

#include"EnemyAttack.h"
#include"BossAttack.h"
#include"CapsuleEnemyAttack.h"

#include"JumpAttack.h"

#include"SlushHitEffect.h"

#include<GameObjectManager.h>

#include"BossAliveChecker.h"
#include"EnemyDamageParticle.h"


#include<Random.h>

Player* Boss::pPlayer;
Boss* Boss::pBoss;

void Boss::Move()
{
	static const float MOVE_SPEED = 1.7f;
	
	// �߂��Ɉړ�
	MoveToPlayer(MOVE_SPEED);
}

void Boss::SelectAction()
{

	// �U������������X�L�b�v
	if (currentState != Boss::CurrentState::NONE)return;
	else actionTimer.SetStopFlag(false);


	MelLib::Vector3 myPos = GetPosition();
	MelLib::Vector3 playerPos = pPlayer->GetPosition();

	float playerDir = MelLib::LibMath::CalcDistance2D(MelLib::Vector2(myPos.x, myPos.z), MelLib::Vector2(playerPos.x, playerPos.z));

	static const float MIN_DIR = 30.0f;
	
	// ����������ړ�
	if (playerDir >= MIN_DIR)
	{
		Move();
	}

	if (actionTimer.GetMaxOverFlag()) 
	{
		//auto castEnum = [&](const Boss::CurrentState state)
		//{
		//	return static_cast<std::underlying_type_t<Boss::CurrentState>>(state);
		//};

		// ��̃����_���Ɨ������r���čU�����Z�b�g����
		// 0��NONE�Ȃ̂�1���Z����
		unsigned int attackNumber = MelLib::Random::GetRandomNumber(3) + 1;

		if (playerDir < MIN_DIR /* && playerDir >= 6.0f*/) // �����ł͂Ȃ��Ȃ�U�� 
		{
			// �����ɍU�����򏈗�������

			currentState = Boss::CurrentState::ROLL_ATTACK;
			modelObjects["main"].SetAnimation("Attack_Roll");
		}
		else
		{
			// �����ɍU�����򏈗�������
			// �������U����2����Ȃ̂�10���Z��
			attackNumber += 10;

			// ��
			unsigned int ranNum = MelLib::Random::GetRandomNumber(2);
			//unsigned int ranNum = 1;

			if (ranNum == 0) 
			{
				currentState = Boss::CurrentState::JUMP_ATTACK;
				modelObjects["main"].SetAnimation("Attack_Jump.001");
			}
			else 
			{
				currentState = Boss::CurrentState::DASH_ATTACK;
				modelObjects["main"].SetAnimation("Attack_Dash");
			}
			

		}

		modelObjects["main"].SetAnimationFrame(0);
		actionTimer.SetStopFlag(true);
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
	case Boss::CurrentState::DASH_ATTACK:
		DashAttackUpdate();
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
	// �U���I������
	if (modelObjects["main"].GetAnimationEndFlag())
	{
		currentState = CurrentState::NONE;
		modelObjects["main"].SetAnimation("No_Cont");

		actionTimer.SetStopFlag(false);
		actionTimer.ResetTimeZero();

		attackControlTimer.SetStopFlag(true);
		attackControlTimer.ResetTimeZero();
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
		AddCupsuleAttack(57,
			EnemyAttack::AttackType::BE_BLOWN_AWAY);
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

void Boss::DashAttackUpdate()
{
	const unsigned int FRAME = modelObjects["main"].GetAnimationFrame();

	if (FRAME == 9)
	{
		attackControlTimer.SetMaxTime(60 * 0.8);
		attackControlTimer.SetStartFlag(true);
		modelObjects["main"].SetAnimationPlayFlag(false);
	}
	else if (FRAME > 9) 
	{

		// �A�j���[�V�������^�C�~���O�ŌŒ肵���܂ܐڋ�
		if (FRAME == 30)
		{
			modelObjects["main"].SetAnimationPlayFlag(false);
		}

		if (MelLib::Input::KeyTrigger(DIK_Q)) 
		{
			int fhweoifwu = 0;
		}

		// �߂Â�����U�� y�͍l�����Ȃ�
		MelLib::Vector3 y0Pos = GetPosition();
		y0Pos.y = 0;
		float gDis = MelLib::LibMath::CalcDistance3D(dashAttackEndPos, y0Pos);
		
		MelLib::Vector3 y0PPos = pPlayer->GetPosition();
		y0PPos.y = 0;
		float pDis = MelLib::LibMath::CalcDistance3D(y0PPos, y0Pos);

		if (gDis <= 7.0f)
		{
			modelObjects["main"].SetAnimationPlayFlag(true);


			dashAttackDash = false;
		}
		else
		{
			// 70�ȏ�̎��͕ێ�
			if (pDis >= 70.0f && !dashAttackGoalSet)
			{
				// �v���C���[���W��ۑ�
		        dashAttackEndPos = pPlayer->GetPosition();
				// �ێ����͉�]�L��
				Rotate();
			}
			else 
			{
				dashAttackGoalSet = true;
			}
			dashAttackEndPos.y = 0;
			MelLib::Vector3 toPlayer = (dashAttackEndPos - GetPosition()).Normalize();
			
			// �ړ�
			AddPosition(toPlayer * 10.0f);
			dashAttackDash = true;

			// �ړ����̂݃p�[�e�B�N�����o��
			sandParEmitter.Update();
		}

		// ����ǉ�
		if (FRAME == 37)
		{
			// �w�肵���t���[���ōU�����������悤�ɂ���
			AddCupsuleAttack(40,
				EnemyAttack::AttackType::BE_BLOWN_AWAY);
			dashAttackGoalSet = false;
		}
	}


	if(attackControlTimer.GetMaxOverFlag())
	{
		modelObjects["main"].SetAnimationPlayFlag(true);

		attackControlTimer.ResetTimeZero();
		attackControlTimer.SetStopFlag(true);
	}

	AttackEnd();
}

void Boss::AddCupsuleAttack(const unsigned int deleteFrame,
	EnemyAttack::AttackType attackType)
{
	std::shared_ptr<CapsuleEnemyAttack>attack;


	// ����̍��W
	MelLib::Value2<MelLib::Vector3>p =
		MelLib::Value2<MelLib::Vector3>(MelLib::Vector3(-9, 10, 5), MelLib::Vector3(-4, 10, 5));

	attack = std::make_shared<CapsuleEnemyAttack>
		(
			10,
			p,
			3.0f,
			modelObjects["main"],
			0,
			0,
			1,
			"Bone_R.003",
			"Body.001",
			deleteFrame,
			attackType
		);

	MelLib::GameObjectManager::GetInstance()->AddObject(attack);
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
	MelLib::ModelData::Load("Resources/Model/Boss/Boss.fbx", false,"boss");
}

Boss::Boss()
	:GameObject("boss")
	, sandParEmitter(sandEffect,10,60 * 0.5f,GetPosition(),"SandPar")
{
	BossAliveChecker::GetInstance()->AddBoss(this);

	modelObjects["main"].Create(MelLib::ModelData::Get("boss"), "boss", nullptr);
	modelObjects["main"].SetAnimation("_T");


	//modelObjects["main"].SetPosition();
	//sphereDatas["main"].resize(3);


	actionTimer.SetResetFlag(false);
	actionTimer.SetMaxTime(60 * 1.5f);

	jumpAttackTimer.SetResetFlag(false);
	jumpAttackTimer.SetMaxTime(60 * 0.1f);

	// ���ݒ�
	/*modelObjects["main"].SetAnimation("Attack_Jump.001");
	currentAttack = CurrentAttack::JUMP;*/


	hp.SetData(100, GetObjectName(), "HP", 1, 100);


	tags.push_back("Enemy");
	tags.push_back("Boss");

	pBoss = this;



}

std::shared_ptr<MelLib::GameObject> Boss::GetNewPtr()
{
	return std::make_shared<Boss>();
}

void Boss::Initialize()
{
	modelObjects["main"].SetAnimation("No_Cont");
	
	// �����蔻��̍쐬
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(GetPosition() + MelLib::Vector3(0, 25.0f, 0), GetPosition() + MelLib::Vector3(0,0.0f, 0)));

	segment3DDatas["main"].resize(2);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());
	
	segment3DDatas["main"][1] = capsuleDatas["main"][0].GetSegment3DData();

	MelLib::Value2<MelLib::Vector3>segPos = capsuleDatas["main"][0].GetSegment3DData().GetPosition();
	segPos.v1 += MelLib::Vector3(0.3f, 0, 0);
	segPos.v2 += MelLib::Vector3(0.3f, 0, 0);
	segment3DDatas["main"][1].SetPosition(segPos);



	mutekiTimer.SetMaxTime(60 * 0.2);

	thisState = ThisState::OTHER;

	hpGauge = std::make_unique<EnemyHPGauge>(hp.GetRefValue());


	
}

void Boss::Update()
{
	FallStart(0.0f);
	CalcMovePhysics();

	// �v���C���[�Ƃ̋������v�Z
	float toPDis = MelLib::LibMath::CalcDistance3D(pPlayer->GetPosition(), GetPosition());

	if (toPDis <= 200.0f)thisState = ThisState::BATTLE;

	
	if (thisState == ThisState::OTHER)return;

	modelObjects["main"].Update();
	modelObjects["main"].SetAnimationPlayFlag(true);

	// �U�����ĂȂ�������ړ��Ɖ�]
	if (currentState == Boss::CurrentState::NONE)
	{
		modelObjects["main"].SetAnimation("Work");
		Rotate();
	}

	SelectAction();
	AttackUpdate();

	// ��
	if (hp.GetValue() <= 0) 
	{
		eraseManager = true;
	}

	if (mutekiTimer.GetMaxOverFlag()) 
	{
		mutekiTimer.ResetTimeZero();
		mutekiTimer.SetStopFlag(true);
		isMuteki = false;
	}

	hpGauge->SetPosition(GetPosition() + MelLib::Vector3(0, 25, 0));
	hpGauge->Update();
}

void Boss::Draw()
{
	AllDraw();
	
	if (dashAttackDash) 
	{
		sandParEmitter.Draw();
	}

	if(hpGauge)hpGauge->Draw();

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
	
	if (!isMuteki) 
	{
		if (typeid(object) == typeid(PlayerSlush))
		{
			const int PARTICLE_NUM = 4;
			for (int i = 0; i < PARTICLE_NUM; i++)
			{
				MelLib::Vector3 vec = MelLib::Vector3(
					MelLib::Random::GetRandomFloatNumberRangeSelect(-1, 1, 1),
					1,
					MelLib::Random::GetRandomFloatNumberRangeSelect(-1, 1, 1)).Normalize();
				vec.y = 2.2f;

				MelLib::GameObjectManager::GetInstance()->AddObject
				(
					std::make_shared<EnemyDamageParticle>(GetPosition(), vec)
				);
			}
		}

		

		if (typeid(object) == typeid(PlayerSlush) || typeid(object) == typeid(JumpAttack) /* && !isMuteki*/)
		{
			MelLib::Vector3 toCameraVec = (MelLib::Camera::Get()->GetCameraPosition() - GetPosition()).Normalize();
			MelLib::Vector3 effectAddPos = MelLib::Vector3(0, 6, 0) + toCameraVec * 2.1f;
			MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<SlushHitEffect>
				(GetPosition() + effectAddPos));

			// �v���C���[���猻�݂̍U���̍U���͂��擾���A�̗͂����炷
			hp.SetValue(hp.GetValue() - pPlayer->GetCurrentAttackPower());

			isMuteki = true;
			mutekiTimer.SetStopFlag(false);
		}
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
