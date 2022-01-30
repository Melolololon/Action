#include "JumpEnemy.h"

#include<GameObjectManager.h>
#include"EditMode.h"
#include"Pause.h"
#include"EnemyAttack.h"

void JumpEnemy::Attack()
{
	// ����ړ�
	MelLib::Vector3 attackPos = GetPosition();
	attackPos.y -= 10;
	enemyAttack->SetPosition(attackPos);

	float moveVectorY = GetPosition().y - prePos.y;
	static const float ATTACK_ADD_VECTOR_Y = -1;

	// �ړ��ʂ�1�ȏ�ɂȂ�����U��������}�l�[�W���[�ɒǉ�
	if(moveVectorY < ATTACK_ADD_VECTOR_Y
		&& GetIsFall()
		&& !attackAddFrame)
	{
		MelLib::GameObjectManager::GetInstance()->AddObject(enemyAttack);
		attackAddFrame = true;
	}
}

void JumpEnemy::AttackRot()
{
	// �U�����̉�]���J�n����t���[��
	static const unsigned int ROT_START_FRAME = 55 - 1;

	// �U�����ɉ�]�t���[���ɂȂ������]�J�n
	if (isAttack&& modelObjects["main"].GetAnimationFrame() == ROT_START_FRAME)
	{
		modelObjects["main"].SetAnimationPlayFlag(false);

		static const float ROT_ANGLE_Y = 25;
		float setAngle = modelObjects["main"].GetAngle().y + ROT_ANGLE_Y;

		if(setAngle >= 360)setAngle = 0;
		modelObjects["main"].SetAngle(MelLib::Vector3(0, setAngle, 0));

		if(setAngle == ROT_ANGLE_Y)
		{
			//// �U������̏o���ʒu
			//static const MelLib::Vector3 ATTACK_POS[3] =
			//{
			//	MelLib::Vector3(),
			//	MelLib::Vector3(),
			//	MelLib::Vector3()
			//};

			//// ���̖{�������U������̒ǉ�
			//for (int i = 0; i < 3; i++) 
			//{
			//	MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
			//		(
			//			3,
			//			ATTACK_POS[i],
			//			3.0f,
			//			60 * 0.2,
			//			modelObjects["main"],
			//			MelLib::Vector3(0, 0, 0),
			//			MelLib::Vector3(0, 0, 0),
			//			3
			//			)
			//	);
			//}
		}

		// 360��]������t���[���i�߂�
		if(setAngle == 0.0f)
		{

			// �A�j���[�V�����̃t���[����i�߁A��]��if���ɓ���Ȃ��悤�ɂ���
			modelObjects["main"].SetAnimationFrame(ROT_START_FRAME + 1);

			// �Đ��ĊJ
			modelObjects["main"].SetAnimationPlayFlag(true);
		}

	}


	

}

void JumpEnemy::CheckJumpStart()
{

	bool jumpStartTiming = 
		modelObjects["main"].GetAnimationFrame() >= JUMP_START_FRAME
		&& !modelObjects["main"].GetAnimationReversePlayBack();
	
	// �A�j���[�V�������W�����v���A�^�b�N�̏ꍇ�̂݃W�����v
	// ���ꏑ���Ȃ��ƁA����A�j���[�V�������ȂǂɃW�����v�����Ⴄ
	jumpStartTiming = jumpStartTiming
		&& (modelObjects["main"].GetCurrentAnimationName() == "Jump"
			|| modelObjects["main"].GetCurrentAnimationName() == "Attack");

	// �W�����v
	if (jumpStartTiming)
	{
		FallStart(3.0f);
	}
}

void JumpEnemy::HitGroundUpdate()
{
	// ����Ă���return
	if (isDead)return;
	

	// ���n�����u�ԋt�Đ��J�n
	if (preIsFall
		&& !GetIsFall())
	{
		modelObjects["main"].SetAnimationReversePlayBack(true);
		modelObjects["main"].SetAnimationPlayFlag(true);

		// �W�����v�h�~�̂��߁A�A�j���[�V�����̃t���[����߂� 
		modelObjects["main"].SetAnimationFrame(JUMP_START_FRAME - 1);

		attackAddFrame = false;
		enemyAttack->TrueEraseManager();
	}

	// �t�Đ����I��������ʏ�ʂ�Đ�
	if (modelObjects["main"].GetAnimationReversePlayBack()
		&& modelObjects["main"].GetAnimationEndFlag())
	{
		modelObjects["main"].SetAnimationReversePlayBack(false);
	}

}

void JumpEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/JumpEnemy/JumpEnemy.fbx", true, "jumpEnemy");
}

JumpEnemy::JumpEnemy(const MelLib::Vector3 pos, int wallNum)
	:Enemy(pos, 2, 2.0f, wallNum, "jumpEnemy")
{

	//modelObjects["main"]

	// ����Z�b�g
	sphereData.resize(1);
	sphereData[0].SetPosition(pos + MelLib::Vector3(0,20,0)); 
	sphereData[0].SetRadius(10.0f);

	// �n�`�p����Z�b�g
	segment3DData.resize(1);
	MelLib::Value2<MelLib::Vector3>segmentPos(GetPosition());
	segmentPos.v1 += MelLib::Vector3(0, 10, 0);
	segmentPos.v2 += MelLib::Vector3(0, 0, 0);
	segment3DData[0].SetPosition(segmentPos);

	// �ǉ��������ĂĂ���������悤�ɂ��邽�߂̌Ăяo��
	FallStart(0.0f);

	// �A�j���[�V�����Z�b�g
	modelObjects["main"].SetAnimation("Jump");
	modelObjects["main"].SetAnimationFrame(JUMP_START_FRAME);
	modelObjects["main"].SetAnimationPlayFlag(true);
	modelObjects["main"].SetAnimationEndStopFlag(true);

	// �U������쐬
	enemyAttack = std::make_shared<NormalEnemyAttack>(2, sphereData[0].GetRadius());




}


void JumpEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;


	prePos = GetPosition();

	// ����ł���return
	if (isDead)
	{
		Dead(); 

		// ���̔��菈���𖳎�����悤��
		collisionFlag.sphere = false;

		modelObjects["main"].SetAnimationPlayFlag(true);
		modelObjects["main"].Update();
		return;
	}

	modelObjects["main"].Update();

	if (!GetIsFall())
	{
		HitGroundUpdate();
	}


	//// �W�����v���ĂȂ��Ƃ��̏���
	if (!GetIsFall())
	{
		// �W�����v�J�n���邩�m�F
		CheckJumpStart();

	}
	else
	{
		// �ʏ�W�����v���̓A�j���[�V������~
		if(!isAttack) modelObjects["main"].SetAnimationPlayFlag(false);
	}


	// ���G�̎��Ԑi�߂�
	CheckMutekiEnd();

	// �d������������return
	if (isStun)
	{
		// �d���֌W�̏���
		Stun();

		// �d���I���Ő؂�ւ�
		if(!isStun)
		{
			modelObjects["main"].SetAnimation("Jump");
		}

		return;
	}


	// �����ɍU���������L�q
	if (CheckPlayerDistance(25.0f) && !isAttack && !GetIsFall())
	{
		// �U�����@�ύX�̂��߃R�����g�A�E�g
		//AttackStart();
	}
	else if (!isAttack && GetIsFall())
	{
		if (!CalcPlayerRoute())modelObjects["main"].SetAnimation("No_Move");
		else modelObjects["main"].SetAnimation("Jump");
	}
	else if(!isAttack && !GetIsFall())
	{
		if(CheckPlayerDistance(MOVE_DISTANCE))
		{
			modelObjects["main"].SetAnimation("Jump");
		}
		else
		{
			modelObjects["main"].SetAnimation("No_Move");
		}
	}


	if (isAttack)
	{
		//modelObjects["main"].SetAnimation("Attack");
		AttackRot();
		
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			isAttack = false;
			modelObjects["main"].SetAnimation("Jump");
		}
	}

	CheckAttackEnd();

	// ��]
	RotModel();

	// ���ԂɂȂ�����U��
	if (attackTimer.GetNowTime() == ATTACK_START_TIME)
	{
		attackTimer.ResetTimeZero();
		attackTimer.SetStopFlag(true);
	}

	CalcMovePhysics();


	// �U������
	Attack();

	preIsFall = GetIsFall();
}
