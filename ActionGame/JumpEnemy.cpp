#include "JumpEnemy.h"

#include<GameObjectManager.h>
#include"EditMode.h"
#include"Pause.h"
#include"EnemyAttack.h"

void JumpEnemy::AttackRot()
{
	// �U�����̉�]���J�n����t���[��
	static const unsigned int ROT_START_FRAME = 1;

	// �U�����ɉ�]�t���[���ɂȂ������]�J�n
	if (isAttack&& modelObjects["main"].GetAnimationFrame() == ROT_START_FRAME)
	{
		modelObjects["main"].SetAnimationPlayFlag(true);

		static const float ROT_ANGLE_Y = 25;
		float setAngle = modelObjects["main"].GetAngle().y + ROT_ANGLE_Y;

		if(setAngle >= 360)setAngle = 0;
		modelObjects["main"].SetAngle(MelLib::Vector3(0, setAngle, 0));

		if(setAngle == ROT_ANGLE_Y)
		{
			// �U������̏o���ʒu
			static const MelLib::Vector3 ATTACK_POS[3] =
			{
				MelLib::Vector3(),
				MelLib::Vector3(),
				MelLib::Vector3()
			};

			// ���̖{�������U������̒ǉ�
			for (int i = 0; i < 3; i++) 
			{
				MelLib::GameObjectManager::GetInstance()->AddObject(std::make_shared<EnemyAttack>
					(
						3,
						ATTACK_POS[i],
						3.0f,
						60 * 0.2,
						modelObjects["main"],
						MelLib::Vector3(0, 0, 0),
						MelLib::Vector3(0, 0, 0),
						3
						)
				);
			}
		}

		// 360��]������t���[���i�߂�
		if(setAngle == 0.0f)
		{

			// �A�j���[�V�����̃t���[����i�߁A��]��if���ɓ���Ȃ��悤�ɂ���
			modelObjects["main"].SetAnimationFrame(ROT_START_FRAME + 1);
		}

	}


	

}

void JumpEnemy::CheckJumpStart()
{
	

	// �W�����v�J�n�t���[��
	static const unsigned int JUMP_START_FRAME = 0;

	bool jumpStartTiming =
		modelObjects["main"].GetCurrentAnimationName() == "Jump"
		&& modelObjects["main"].GetAnimationFrame() == JUMP_START_FRAME
		&& !modelObjects["main"].GetAnimationReversePlayBack();
	
	if(jumpStartTiming)
	{
		FallStart(20.0f);
		modelObjects["main"].SetAnimationEndStopFlag(true);
	}
}

void JumpEnemy::HitGroundUpdate()
{

	// ���n�����u�ԋt�Đ��J�n
	if(modelObjects["main"].GetAnimationEndFlag()
		&& !modelObjects["main"].GetAnimationReversePlayBack())
	{
		modelObjects["main"].SetAnimationReversePlayBack(true);
	}

	// �t�Đ����I��������ʏ�ʂ�Đ�
	if(modelObjects["main"].GetAnimationEndFlag()
		&& modelObjects["main"].GetAnimationReversePlayBack())
	{
		modelObjects["main"].SetAnimationReversePlayBack(false);
	}

}

void JumpEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/JumpEnemy/JumpEnemy.fbx", true, "jumpEnemy");
}

JumpEnemy::JumpEnemy(const MelLib::Vector3 pos)
	:Enemy(pos, 2, 2.0f, "jumpEnemy")
{
	//modelObjects["main"]

	//sphereData.resize(1);
	//sphereData[0].SetPosition(pos); 
	//sphereData[0].SetRadius(5.0f);

	segment3DData.resize(1);
	MelLib::Value2<MelLib::Vector3>segmentPos(GetPosition());
	segmentPos.v1 += MelLib::Vector3(0, 10, 0);
	segmentPos.v2 += MelLib::Vector3(0, -4, 0);
	segment3DData[0].SetPosition(segmentPos);

	FallStart(0.0f);

	modelObjects["main"].SetAnimation("Jump");
	modelObjects["main"].SetAnimationFrameEnd();
}


void JumpEnemy::Update()
{
	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	modelObjects["main"].Update();

	// �W�����v�J�n���邩�m�F
	if (!GetIsFall())CheckJumpStart();

	// �W�����v���ĂȂ��Ƃ��̏���
	if (!GetIsFall())HitGroundUpdate();

	// ����ł���return
	if (isDead)
	{
		Dead();
		return;
	}

	// ���G�̎��Ԑi�߂�
	CheckMutekiEnd();

	// �d������������return
	if (isStun)
	{
		// �d���֌W�̏���
		Stun();
		return;
	}

	// �����ɍU���������L�q
	if (CheckPlayerDistance(25.0f) && !isAttack)
	{
		AttackStart();
	}
	else if (!isAttack && GetIsFall())
	{
		CalcPlayerRoute();

	}
	if (isAttack)
	{
		//modelObjects["main"].SetAnimation("Attack");

		
		if (modelObjects["main"].GetAnimationEndFlag())
		{
			isAttack = false;
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

}
