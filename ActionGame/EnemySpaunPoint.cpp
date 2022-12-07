#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"

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
	// 敵を一斉に動かす
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
	// 順番決めるか近さで決めるか
	// 近さで決める
	// 一度に攻撃するキャラ数に制限をかける

}

EnemySpaunPoint::EnemySpaunPoint():GameObject("EnemySpaunPoint")
{
	enemys.resize(ENEMY_MAX_NUM);
	
	// 敵の生成と追加
	for (auto& enemy: enemys)
	{
		enemy = std::shared_ptr<WeakEnemy>();
		MelLib::GameObjectManager::GetInstance()->AddObject(enemy);
	}

	// 自身を追加
	spaunPoints.push_back(this);


}

void EnemySpaunPoint::Update()
{
	// 敵0人で削除
	if (enemyNum <= 0)eraseManager = true;

	// プレイヤーと確認
	float dis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// 近かったら攻撃
	if (dis <= MIN_DISTANCE) Attack();

	// 距離を確認
	for (auto& spaunPoint : spaunPoints) 
	{
		if (spaunPoint == this)continue;

		float dis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

		// 近かったら動かない
		if (dis <= MIN_DISTANCE) return;
	}	
	
	// 移動
	Move();
}

void EnemySpaunPoint::Draw()
{
}
