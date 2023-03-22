#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"
#include<Random.h>

// �G�̎��
#include"WeakEnemy.h"

const float EnemySpaunPoint::MIN_DISTANCE = 30.0f;
const unsigned int EnemySpaunPoint::ENEMY_MAX_NUM = 5;


std::vector<EnemySpaunPoint*>EnemySpaunPoint::spaunPoints;
Player* EnemySpaunPoint::pPlayer;


bool EnemySpaunPoint::CalcDistance()
{
	return false;
}

void EnemySpaunPoint::Move()
{
	MelLib::Vector3 playerVector = pPlayer->CalcPlayerVector(GetPosition());

	AddPosition(playerVector * moveSpeed);

	// �G����Ăɓ�����
	for (auto& enemy : enemys)
	{
		enemy->AddPosition(playerVector * moveSpeed);
	}
}

void EnemySpaunPoint::Rot()
{
}

void EnemySpaunPoint::Attack()
{
	// ���Ԍ��߂邩�߂��Ō��߂邩
	// �߂��Ō��߂�
	// ��x�ɍU������L�������ɐ�����������

}

void EnemySpaunPoint::CteateEnemy()
{
	enemys.resize(ENEMY_MAX_NUM);
	
	if (CLASS_NAME == typeid(WeakEnemy).name())
	{
		for (auto& enemy : enemys)
		{
			MelLib::Vector3 enemyPos(
				MelLib::Random::GetRandomFloatNumberRangeSelect(-10.0f, 10.0f, 2),
				0,
				MelLib::Random::GetRandomFloatNumberRangeSelect(-10.0f, 10.0f, 2)
			);


			enemy = std::make_shared<WeakEnemy>(enemyPos);
			moveSpeed = WeakEnemy::GetMoveSpeed();

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
	// �G0�l�ō폜
	//if (enemyNum <= 0)eraseManager = true;

	// �v���C���[�Ɗm�F
	float playerDis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// �߂�������U��
	if (playerDis <= MIN_DISTANCE) Attack();

	// �������m�F
	for (auto& spaunPoint : spaunPoints) 
	{
		if (spaunPoint == this)continue;

		float spaunDis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

		// �߂������瓮���Ȃ�
		if (spaunDis <= MIN_DISTANCE) return;
	}	
	
	// �ړ�
	Move();
}

void EnemySpaunPoint::Draw()
{
	AllDraw();
}

std::shared_ptr<MelLib::GameObject> EnemySpaunPoint::GetNewPtr()
{
	return std::make_shared<EnemySpaunPoint>(CLASS_NAME);
}
