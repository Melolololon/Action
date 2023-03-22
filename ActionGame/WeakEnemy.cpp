#include "WeakEnemy.h"
#include"Pause.h"
#include"Stage.h"

#include"CapsuleEnemyAttack.h"

#include"PlayerSlush.h"
#include"JumpAttack.h"


#include<GameObjectManager.h>

#include<LibMath.h>

// 35F�ōU������

void WeakEnemy::SetStartAttackAnimation()
{
	modelObjects["main"].SetAnimation("Attack");
	modelObjects["main"].SetAnimationFrameStart();
	state = ThisState::ATTACK;
}

void WeakEnemy::Attack()
{
	/*timer++;

	if (timer >= 60 * 1.5f)
	{
		isAttack = false;
		timer = 0;
	}*/

	// �U������ǉ�
	if (modelObjects["main"].GetAnimationFrame() == 35)
	{
		AddAttackCollisionDetection();
	}

	if (modelObjects["main"].GetAnimationEndFlag()) 
	{
		isAttack = false;
		state = ThisState::MOVE;
		modelObjects["main"].SetAnimation("E_Dash");
	}
}

void WeakEnemy::AddAttackCollisionDetection()
{
	std::shared_ptr<CapsuleEnemyAttack>attack;

	// ����̃Z�b�g���@
	// 1.T�|�[�Y�ɂ���
	// 2.�l�p�����f����p�ӂ��A�����蔻��̈ʒu�ɃZ�b�g���A���̍��W����������
	// 3.2�ڂ̈���(�����0�̕���)�������������W�ɂ���
	// 4.2���s�����Ƃ��̃��f���̍��W�Ȃǂ�������5�`7�ɃZ�b�g����
	// 
	// 0�𔻒�̏������W�ɏ���������
	// 5�̕����ɁA�����蔻��̍��W�����Z�b�g�������̍��W�A�p�x�A�傫�������

	// ����̍��W
	MelLib::Value2<MelLib::Vector3>p = 
		MelLib::Value2<MelLib::Vector3>(MelLib::Vector3(-4.9, 9.7, 6.5), MelLib::Vector3(-4.9, 9.7, 8));

	attack = std::make_shared<CapsuleEnemyAttack>
		(
			30,
			p,
			2.0f,
			modelObjects["main"],
			0,
			0,
			1,
			"Bone_R.003",
			"Body.001"
		);

	MelLib::GameObjectManager::GetInstance()->AddObject(attack);

}

void WeakEnemy::Rotate()
{
	// �U�����͉�]���Ȃ�

	//direction = (pPlayer->GetPosition() - GetPosition()).Normalize();

	//// ��]
	//float angle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	//modelObjects["main"].SetAngle(MelLib::Vector3(28, angle, 0));

	// �i�s�����Ɍ����悤��
	MelLib::Vector3 direction = (pPlayer->GetPosition() - GetPosition()).Normalize();

	// ��]
	float directionAngle = MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), false) - 270;
	float preAngle = modelObjects["main"].GetAngle().y;

	// �v���C���[�̌����̊p�x�ƍ��̊p�x��5�x�ȓ���������return
	if (MelLib::LibMath::AngleDifference(directionAngle, preAngle, 5))return;

	SetAngle(MelLib::Vector3(0, directionAngle, 0));
	//modelObjects["main"].SetAngle(MelLib::Vector3(0, directionAngle, 0));
}

void WeakEnemy::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Enemy/WeakEnemy.fbx", true, "WeakEnemy");
}

WeakEnemy::WeakEnemy(const MelLib::Vector3& pos):NewEnemy("WeakEnemy")
{
	SetPosition(pos);

	//modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX),"WeakEnemy");
	modelObjects["main"].Create(MelLib::ModelData::Get("WeakEnemy"),"WeakEnemy");
	modelObjects["main"].SetPosition(pos);

	// �����蔻��̍쐬
	capsuleDatas["main"].resize(1);
	capsuleDatas["main"][0].SetRadius(4.5f);
	capsuleDatas["main"][0].GetRefSegment3DData().
		SetPosition(MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, 0, 0)));

	segment3DDatas["main"].resize(1);
	segment3DDatas["main"][0].SetPosition(capsuleDatas["main"][0].GetSegment3DData().GetPosition());

	// �v���C���[�݂����ȏ\���̐��������āA�ǂŃW�����v����悤�ɂ���


	//modelObjects["main"].SetScale(20);

	/*modelObjects["Hit01"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "H01");
	modelObjects["Hit01"].SetPosition(MelLib::Vector3(-4.9, 9.7, 6.5));

	modelObjects["Hit02"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "H02");
	modelObjects["Hit02"].SetPosition(MelLib::Vector3(-4.9, 9.7, 8));*/
}

void WeakEnemy::Initialize()
{
	FallStart(0.0f);
	modelObjects["main"].SetAnimationPlayFlag(true);
	modelObjects["main"].SetAnimation("E_Dash");
	state = ThisState::MOVE;
}

void WeakEnemy::Update()
{
	CalcMovePhysics();
	modelObjects["main"].Update();

	Rotate();

	if (isAttack)Attack();

	// ������ђ��������琁����ԓ��������
	if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
	{
		BeBlownAwayMove();
		state = ThisState::BE_BLOWN_AWAY;
	}

	if (state == ThisState::GET_UP
		&& modelObjects["main"].GetAnimationEndFlag()) 
	{
		modelObjects["main"].SetAnimation("E_Dash");
		state = ThisState::MOVE;
	}
}

void WeakEnemy::Draw()
{
	if (isAttack)modelObjects["main"].SetMulColor(MelLib::Color(0, 0, 255, 255));
	else modelObjects["main"].SetMulColor(MelLib::Color(255,255, 255, 255));
	AllDraw();
}

void WeakEnemy::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	//if (Pause::GetInstance()->GetIsPause())return;

	//if (isDead)return;

	std::string n = typeid(object).name();
	if (typeid(object) == typeid(PlayerSlush) || typeid(object) == typeid(JumpAttack) /* && !isMuteki*/)
	{

		// �v���C���[���猻�݂̍U���̍U���͂��擾���A�̗͂����炷
		//hp -= pPlayer->GetCurrentAttackPower();

		// ���G��
		//isMuteki = true;
		//mutekiTimer.SetStopFlag(false);

		// �d������
		//isStun = true;
		//modelObjects["main"].SetAnimation("Stun");

		//modelObjects["main"].SetAnimationFrameStart();
		//modelObjects["main"].SetAnimationEndStopFlag(true);
		//modelObjects["main"].SetAnimationReversePlayBack(false);

		// �U�������I��
		isAttack = false;
		//attackTimer.ResetTimeZero();
		//attackTimer.SetStopFlag(true);


		// 0�ɂȂ�������ꏈ��
		/*if (hp <= 0)
		{
			isDead = true;

			modelObjects["main"].SetAnimation("Dead");
		}*/

		// ������΂�����
		// ���1�t���[���u����NONE�ɐ؂�ւ���悤�ɂ���
		if (pPlayer->GetPlayerAttackEffect() == AttackEffect::BE_BLOWN_AWAY || typeid(object) == typeid(JumpAttack))
		{
			FallStart(0.6f);
			currentThisAttackEffect = AttackEffect::BE_BLOWN_AWAY;
			state = ThisState::BE_BLOWN_AWAY;
			modelObjects["main"].SetAnimation("BeBlownAway");
			modelObjects["main"].SetAnimationFrameStart();
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

		if (currentThisAttackEffect == AttackEffect::BE_BLOWN_AWAY)
		{
			currentThisAttackEffect = AttackEffect::NONE;

			state = ThisState::GET_UP;
			modelObjects["main"].SetAnimation("BeBlownAway_2");
			modelObjects["main"].SetAnimationFrameStart();
		}
	}
}
