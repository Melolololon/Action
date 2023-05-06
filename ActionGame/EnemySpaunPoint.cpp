#include "EnemySpaunPoint.h"
#include"GameObjectManager.h"
#include"LibMath.h"
#include<Random.h>

// 敵の種類
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

		// 移動してないやつは対象外
		if (minDisEnemyDis > dis
			&& enemys[i]->GetThisState() == NewEnemy::ThisState::MOVE)
		{
			minDisEnemyDis = dis;
			minDisEnemyNum = i;
		}
	}

	// 距離の計算1回でいい

	// 吹っ飛んだりしてなかったら敵を動かす
	for (auto& enemy : enemys)
	{
		if (enemy->GetCurrentThisAttackEffect() == AttackEffect::NONE)
		{
			MelLib::Vector3 pos = enemy->GetPosition();
			pos.y = 0;
			float dis = MelLib::LibMath::CalcDistance3D(pos, pPos);
			
			// 近かったら移動しない
			if (dis <= MIN_DISTANCE && enemy != enemys[minDisEnemyNum])continue;
			else if (dis <= ATTACK_MIN_DISTANCE)continue;// 一番近いやつはめっちゃ接近する
		
			// 移動中以外だったら無視
			if (enemy->GetThisState() != NewEnemy::ThisState::MOVE)continue;

			playerNormalizeVector = pPlayer->CalcPlayerVector(enemy->GetPosition());
			moveVector = MelLib::Vector3(playerNormalizeVector.x, 0, playerNormalizeVector.z) * moveSpeed;
			enemy->AddPosition(moveVector);
		}
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

		// 浮き防止
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
	// 自身を追加
	spaunPoints.push_back(this);


	// 敵の生成と追加
	CteateEnemy();
}

void EnemySpaunPoint::Update()
{
	// 個別にプレイヤーに向かうようにする
	// 一番近いやつだけ近づく。それ以外は一定距離で待機

	CheckEndAttack();

	// 敵0人で削除
	//if (enemyNum <= 0)eraseManager = true;

	Move();
	// プレイヤーと確認
	playerDis = MelLib::LibMath::CalcDistance3D(GetPosition(), pPlayer->GetPosition());
	// 近かったら攻撃
	if (!isAttack && minDisEnemyDis <= ATTACK_MIN_DISTANCE) Attack();

	//bool move = true;

	//// プレイヤーと近かったら移動しない
	//if (playerDis <= MIN_DISTANCE)move = false;

	//// 距離を確認
	//for (auto& spaunPoint : spaunPoints) 
	//{
	//	if (spaunPoint == this)continue;

	//	float spaunDis = MelLib::LibMath::CalcDistance3D(GetPosition(), spaunPoint->GetPosition());

	//	// 近かったら動かない
	//	if (spaunDis <= MIN_DISTANCE) move = false;
	//}	
	

	//// 移動
	//if (!isAttack && move)
	//{
	//	//Move();
	//}
	//else 
	//{
	//	/* 遠い敵がいたら例外で動かす
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
