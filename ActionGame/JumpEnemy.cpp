#include "JumpEnemy.h"

#include<GameObjectManager.h>
#include"EditMode.h"
#include"Pause.h"
#include"EnemyAttack.h"

void JumpEnemy::Attack()
{

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


	// �W�����v�J�n�t���[��(�ŏI�t���[��)�擾
	static const unsigned int JUMP_START_FRAME = 50 - 1;

	int f = modelObjects["main"].GetAnimationFrame();
	bool jumpStartTiming = modelObjects["main"].GetAnimationFrame() == JUMP_START_FRAME;
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

	
	// ���n�����u�ԋt�Đ��J�n
	if (modelObjects["main"].GetAnimationEndFlag()
		&& !modelObjects["main"].GetAnimationReversePlayBack())
	{
		modelObjects["main"].SetAnimationReversePlayBack(true);
		modelObjects["main"].SetAnimationPlayFlag(true);
	}

	// �t�Đ����I��������ʏ�ʂ�Đ�
	if (modelObjects["main"].GetAnimationEndFlag()
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

	sphereData.resize(1);
	sphereData[0].SetPosition(pos + MelLib::Vector3(0,20,0)); 
	sphereData[0].SetRadius(10.0f);

	segment3DData.resize(1);
	MelLib::Value2<MelLib::Vector3>segmentPos(GetPosition());
	segmentPos.v1 += MelLib::Vector3(0, 10, 0);
	segmentPos.v2 += MelLib::Vector3(0, 0, 0);
	segment3DData[0].SetPosition(segmentPos);


	FallStart(0.0f);

	modelObjects["main"].SetAnimation("Jump");
	modelObjects["main"].SetAnimationFrameEnd();
	modelObjects["main"].SetAnimationPlayFlag(true);
	modelObjects["main"].SetAnimationEndStopFlag(true);


}


void JumpEnemy::Update()
{

	if (EditMode::GetInstance()->GetIsEdit() || Pause::GetInstance()->GetIsPause())return;

	prePos = GetPosition();

	// ����ł���return
	if (isDead)
	{
		Dead(); 
		modelObjects["main"].SetAnimationPlayFlag(true);
		modelObjects["main"].Update();
		return;
	}
	
	// �W�����v����O�ɍĐ����t�ɂ��鏈�����s���悤�ɂ��A
	// ���n���ɃW�����v�����̊m�F�̑O��1�t���[�����߂��A�����ɃW�����v���Ȃ��悤�ɂ��邽�߂ɁA
	// ��UModelObject�̑O��HitGroundUpdate�̏��������Ă�

	if (!GetIsFall())
	{
		HitGroundUpdate();
	}

	modelObjects["main"].Update();

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

}
