#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"
#include<Random.h>

// �G�̎��
#include"WeakEnemy.h"

#include"Stage.h"

const float EnemySpaunPoint::MIN_DISTANCE = 60.0f;
const float EnemySpaunPoint::ATTACK_MIN_DISTANCE = 8.0f;
const unsigned int EnemySpaunPoint::ENEMY_MAX_NUM = 1;


std::vector<EnemySpaunPoint*>EnemySpaunPoint::spaunPoints;
Player* EnemySpaunPoint::pPlayer;


bool EnemySpaunPoint::CalcDistance()
{
	return false;
}

void EnemySpaunPoint::Move()
{
	MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(GetPosition());
	MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * moveSpeed;
	//AddPosition(moveVector);

	minDisEnemyDis = FLT_MAX;
	
	MelLib::Vector3 pPos = pPlayer->GetPosition();
	pPos.y = 0;
	for (int i = 0; i < enemys.size(); i++)
	{
		MelLib::Vector3 pos = enemys[i]->GetPosition();
		pos.y = 0;
		float dis = MelLib::LibMath::CalcDistance3D(pos, pPos);

		// �ړ����ĂȂ���͑ΏۊO
		if (minDisEnemyDis > dis
			&& enemys[i]->GetThisState() == NewEnemy::ThisState::MOVE)
		{
			minDisEnemyDis = dis;
			minDisEnemyNum = i;
		}
	}

	// �����̌v�Z1��ł���

	// ������񂾂肵�ĂȂ�������G�𓮂���
	for (auto& enemy : enemys)
	{
		if (enemy->GetCurrentThisAttackEffect() == AttackEffect::NONE)
		{
			MelLib::Vector3 pos = enemy->GetPosition();
			pos.y = 0;
			float dis = MelLib::LibMath::CalcDistance3D(pos, pPos);
			
			// �߂�������ړ����Ȃ�
			if (dis <= MIN_DISTANCE && enemy != enemys[minDisEnemyNum])continue;
			else if (dis <= ATTACK_MIN_DISTANCE)continue;// ��ԋ߂���͂߂�����ڋ߂���
		
			// �ړ����ȊO�������疳��
			if (enemy->GetThisState() != NewEnemy::ThisState::MOVE)continue;

			playerNormalizeVector = pPlayer->CalcPlayerVector(enemy->GetPosition());
			moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * moveSpeed;
			enemy->AddPosition(moveVector);
		}
	}

	// ��ԓG������ȊO���̃t���O���������Ĕ��f����悤�ɂ���
	CalcMovePhysics();
}

void EnemySpaunPoint::Rot()
{
}

void EnemySpaunPoint::Attack()
{

	// ���Ԍ��߂邩�߂��Ō��߂邩
	// �߂��Ō��߂�
	// ��x�ɍU������L�������ɐ�����������

	enemys[minDisEnemyNum]->StartAttack();
	enemys[minDisEnemyNum]->SetStartAttackAnimation();
	isAttack = true;

}

void EnemySpaunPoint::CheckEndAttack()
{
	if (!enemys[attackEnemyNum]->GetIsAttack())isAttack = false;
}

void EnemySpaunPoint::CteateEnemy()
{
	enemys.resize(ENEMY_MAX_NUM);
	
	if (CLASS_NAME == typeid(WeakEnemy).name())
	{

		// �����h�~
		FallStart(0.0f);

		moveSpeed = WeakEnemy::GetMoveSpeed();

		for (auto& enemy : enemys)
		{
			MelLib::Vector3 enemyPos(
				MelLib::Random::GetRandomFloatNumberRangeSelect(-10.0f, 10.0f, 1),
				0,
				MelLib::Random::GetRandomFloatNumberRangeSelect(-10.0f, 10.0f, 1)
			);
			enemyPos += GetPosition();

			enemy = std::make_shared<WeakEnemy>(enemyPos);

			MelLib::GameObjectManager::GetInstance()->AddObject(enemy);

		}

	}

}

EnemySpaunPoint::EnemySpaunPoint(const std::string& className)
	:GameObject("EnemySpaunPoint")
	, CLASS_NAME(className)
{

	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "WeakEnemy");
	modelObjects["main"].SetPosition(GetPosition());
	SetScale(5.0f);

	segment3DDatas["main"].resize(1);
	MelLib::Vector3 pos = GetPosition();
	MelLib::Value2<MelLib::Vector3> segmentPos =
		MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0));
	segment3DDatas["main"][0].SetPosition(segmentPos);
}

void EnemySpaunPoint::Initialize()
{
	// ���g��ǉ�
	spaunPoints.push_back(this);


	// �G�̐����ƒǉ�
	CteateEnemy();
}

void EnemySpaunPoint::Update()
{
	// �ʂɃv���C���[�Ɍ������悤�ɂ���
	// ��ԋ߂�������߂Â��B����ȊO�͈�苗���őҋ@

	CheckEndAttack();

	// �G0�l�ō폜
	//if (enemyNum <= 0)eraseManager = true;

	Move();
	// �v���C���[�Ɗm�F
	playerDis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// �߂�������U��
	if (!isAttack && minDisEnemyDis <= ATTACK_MIN_DISTANCE) Attack();

	//bool move = true;

	//// �v���C���[�Ƌ߂�������ړ����Ȃ�
	//if (playerDis <= MIN_DISTANCE)move = false;

	//// �������m�F
	//for (auto& spaunPoint : spaunPoints) 
	//{
	//	if (spaunPoint == this)continue;

	//	float spaunDis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

	//	// �߂������瓮���Ȃ�
	//	if (spaunDis <= MIN_DISTANCE) move = false;
	//}	
	

	//// �ړ�
	//if (!isAttack && move)
	//{
	//	//Move();
	//}
	//else 
	//{
	//	/* �����G���������O�œ�����
	//	for (auto& enemy : enemys)
	//	{
	//		if (enemy->GetIsAttack())continue;

	//		if (MelLib::LibMath::CalcDistance3D(enemy->GetPosition(), pPlayer->GetPosition()) >= MIN_DISTANCE)
	//		{
	//			MelLib::Vector3 playerNormalizeVector = pPlayer->CalcPlayerVector(enemy->GetPosition());
	//			MelLib::Vector3 moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * moveSpeed;

	//			enemy->AddPosition(moveVector);
	//		}
	//	}*/
	//}
}

void EnemySpaunPoint::Draw()
{
	modelObjects["main"].SetMulColor(MelLib::Color(0, 255, 0, 255));
	AllDraw();
}

void EnemySpaunPoint::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//�����グ�����I��
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);
	}
}

std::shared_ptr<MelLib::GameObject> EnemySpaunPoint::GetNewPtr()
{
	return std::make_shared<EnemySpaunPoint>(CLASS_NAME);
}
