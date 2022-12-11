#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"
#include<Random.h>

// 敵の種類
#include"WeakEnemy.h"

#include"Stage.h"

const float EnemySpaunPoint::MIN_DISTANCE = 60.0f;
const unsigned int EnemySpaunPoint::ENEMY_MAX_NUM = 5;


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

	AddPosition(moveVector);

	// 敵を一斉に動かす
	for (auto& enemy : enemys)
	{
		enemy->AddPosition(moveVector);
	}

	// 飛ぶ敵かそれ以外かのフラグを持たせて判断するようにする
	CalcMovePhysics();
}

void EnemySpaunPoint::Rot()
{
}

void EnemySpaunPoint::Attack()
{
	// 順番決めるか近さで決めるか
	// 近さで決める
	// 一度に攻撃するキャラ数に制限をかける

	float minDis = FLT_MAX;
	for (int i = 0; i < enemys.size();i++)
	{
		MelLib::Vector3 pos = enemys[i]->GetPosition();
		float dis = MelLib::LibMath::CalcDistance3D(pos, pPlayer->GetPosition());

		if (minDis > dis)
		{
			minDis = dis;
			attackEnemyNum = i;
		}
	}

	enemys[attackEnemyNum]->StartAttack();
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

		// 浮き防止
		FallStart(0.0f);

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

	segment3DDatas["main"].resize(1);
	MelLib::Vector3 pos = GetPosition();
	MelLib::Value2<MelLib::Vector3> segmentPos =
		MelLib::Value2<MelLib::Vector3>(pos + MelLib::Vector3(0, 25.0f, 0), pos + MelLib::Vector3(0, -10.0f, 0));
	segment3DDatas["main"][0].SetPosition(segmentPos);
}

void EnemySpaunPoint::Initialize()
{
	// 自身を追加
	spaunPoints.push_back(this);


	// 敵の生成と追加
	CteateEnemy();
}

void EnemySpaunPoint::Update()
{
	CheckEndAttack();

	// 敵0人で削除
	//if (enemyNum <= 0)eraseManager = true;

	// プレイヤーと確認
	playerDis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// 近かったら攻撃
	if (playerDis <= MIN_DISTANCE && !isAttack) Attack();

	// プレイヤーと近かったら移動しない
	if (playerDis <= MIN_DISTANCE)return;

	// 距離を確認
	for (auto& spaunPoint : spaunPoints) 
	{
		if (spaunPoint == this)continue;

		float spaunDis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

		// 近かったら動かない
		if (spaunDis <= MIN_DISTANCE) return;
	}	
	
	// 移動
	if (!isAttack) 
	{
		Move();
	}
}

void EnemySpaunPoint::Draw()
{
	AllDraw();
}

void EnemySpaunPoint::Hit(const GameObject& object, const MelLib::ShapeType3D collisionType, const std::string& shapeName, const MelLib::ShapeType3D hitObjColType, const std::string& hitShapeName)
{
	if (typeid(object) == typeid(Stage)
		&& collisionType == MelLib::ShapeType3D::SEGMENT)
	{

		MelLib::Vector3 addPos;

		//投げ上げ処理終了
		FallEnd();

		addPos.y += GetSegmentCalcResult().triangleHitPos.y - segment3DDatas["main"][0].GetPosition().v2.y;

		AddPosition(addPos);
	}
}

std::shared_ptr<MelLib::GameObject> EnemySpaunPoint::GetNewPtr()
{
	return std::make_shared<EnemySpaunPoint>(CLASS_NAME);
}
