#pragma once
#include"Player.h"

#include"NewEnemy.h"

// これ同士で距離を取るようにすれば処理少なくていいのでは
class EnemySpaunPoint :public MelLib::GameObject
{
	// 最低距離
	static const float MIN_DISTANCE;

	static std::vector<EnemySpaunPoint*>spaunPoints;
	
	// 生成数
	static const unsigned int ENEMY_MAX_NUM;

	// ここに敵の配列作って管理
	std::vector<std::shared_ptr<NewEnemy>>enemys;
	unsigned int enemyNum = ENEMY_MAX_NUM;

	static Player* pPlayer;

	float moveSpeed = 0.0f;

	const std::string CLASS_NAME;
private:
	bool CalcDistance();

	void Move();
	void Rot();
	void Attack();

	void CteateEnemy(std::shared_ptr<NewEnemy>& pEnemy);
public:
	EnemySpaunPoint(const std::string& className);
	void Initialize()override;
	void Update()override;
	void Draw()override;
	std::shared_ptr<GameObject> GetNewPtr()override;

	static void SetPlayer(Player* p) { pPlayer = p; }

	static void ClearEnemySpauns() { spaunPoints.clear(); }
};

