#pragma once
#include<GameObject.h>

#include"Player.h"
#include"WeakEnemy.h"

// これ同士で距離を取るようにすれば処理少なくていいのでは
class EnemySpaunPoint :public MelLib::GameObject
{
	// 最低距離
	static const float MIN_DISTANCE;

	static std::vector<EnemySpaunPoint*>spaunPoints;
	
	// 生成数
	static const unsigned int ENEMY_MAX_NUM;

	// ここに敵の配列作って管理
	std::vector<std::shared_ptr<WeakEnemy>>enemys;
	unsigned int enemyNum = ENEMY_MAX_NUM;

	static Player* pPlayer;

	static const float MOVE_SPEED;

	
private:
	bool CalcDistance();

	void Move();
	void Rot();
	void Attack();
public:
	EnemySpaunPoint();
	void Update()override;
	void Draw()override;

	static void SetPlayer(Player* p) { pPlayer = p; }
};

