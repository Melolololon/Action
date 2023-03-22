#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"

// �G�̎��
#include"WeakEnemy.h"

const float EnemySpaunPoint::MIN_DISTANCE = 30.0f;
const unsigned int EnemySpaunPoint::ENEMY_MAX_NUM = 5;

const float EnemySpaunPoint::MOVE_SPEED = 0.25f;

std::vector<EnemySpaunPoint*>EnemySpaunPoint::spaunPoints;
Player* EnemySpaunPoint::pPlayer;


bool EnemySpaunPoint::CalcDistance()
{
	return false;
}

void EnemySpaunPoint::Move()
{
	MelLib::Vector3 playerVector = pPlayer->CalcPlayerVector(GetPosition());
	// �G����Ăɓ�����
	for (auto& enemy : enemys)
	{
		enemy->AddPosition(playerVector * MOVE_SPEED);
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

void EnemySpaunPoint::CteateEnemy(std::shared_ptr<NewEnemy>& pEnemy, const type_info& type)
{
	if (type == typeid(WeakEnemy)) pEnemy = std::shared_ptr<WeakEnemy>();
}

EnemySpaunPoint::EnemySpaunPoint(const type_info& type):GameObject("EnemySpaunPoint")
{
	enemys.resize(ENEMY_MAX_NUM);
	
	// �G�̐����ƒǉ�
	for (auto& enemy: enemys)
	{
		// className�ŉ��𐶐����邩�ύX����
		CteateEnemy(enemy, type);
		MelLib::GameObjectManager::GetInstance()->AddObject(enemy);
	}

	// ���g��ǉ�
	spaunPoints.push_back(this);

	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "WeakEnemy");
	modelObjects["main"].SetPosition(GetPosition());
}

void EnemySpaunPoint::Update()
{
	// �G0�l�ō폜
	if (enemyNum <= 0)eraseManager = true;

	// �v���C���[�Ɗm�F
	float dis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// �߂�������U��
	if (dis <= MIN_DISTANCE) Attack();

	// �������m�F
	for (auto& spaunPoint : spaunPoints) 
	{
		if (spaunPoint == this)continue;

		float dis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

		// �߂������瓮���Ȃ�
		if (dis <= MIN_DISTANCE) return;
	}	
	
	// �ړ�
	Move();
}

void EnemySpaunPoint::Draw()
{
	AllDraw();
}
