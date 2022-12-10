#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"

// 敵の種類
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

	//// 敵を一斉に動かす
	//for (auto& enemy : enemys)
	//{
	//	enemy->AddPosition(playerVector * MOVE_SPEED);
	//}
}

void EnemySpaunPoint::Rot()
{
}

void EnemySpaunPoint::Attack()
{
	// 順番決めるか近さで決めるか
	// 近さで決める
	// 一度に攻撃するキャラ数に制限をかける

}

void EnemySpaunPoint::CteateEnemy(std::shared_ptr<NewEnemy>& pEnemy)
{
	if (CLASS_NAME == typeid(WeakEnemy).name())
	{
		//pEnemy = std::shared_ptr<WeakEnemy>();

		moveSpeed = WeakEnemy::GetMoveSpeed();
	}
}

EnemySpaunPoint::EnemySpaunPoint(const std::string& className)
	:GameObject("EnemySpaunPoint")
	, CLASS_NAME(className)
{
	enemys.resize(ENEMY_MAX_NUM);
	//
	// 敵の生成と追加
	for (auto& enemy: enemys)
	{
		// classNameで何を生成するか変更する
		CteateEnemy(enemy);
		//MelLib::GameObjectManager::GetInstance()->AddObject(enemy);
	}


	modelObjects["main"].Create(MelLib::ModelData::Get(MelLib::ShapeType3D::BOX), "WeakEnemy");
	modelObjects["main"].SetPosition(GetPosition());
}

void EnemySpaunPoint::Initialize()
{
	// 自身を追加
	spaunPoints.push_back(this);
}

void EnemySpaunPoint::Update()
{
	// 敵0人で削除
	//if (enemyNum <= 0)eraseManager = true;

	// プレイヤーと確認
	float playerDis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// 近かったら攻撃
	if (playerDis <= MIN_DISTANCE) Attack();

	// 距離を確認
	for (auto& spaunPoint : spaunPoints) 
	{
		if (spaunPoint == this)continue;

		float spaunDis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

		// 近かったら動かない
		if (spaunDis <= MIN_DISTANCE) return;
	}	
	
	// 移動
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
