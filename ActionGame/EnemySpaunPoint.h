#pragma once
#include<GameObject.h>

#include"Player.h"
#include"WeakEnemy.h"

// ‚±‚ê“¯m‚Å‹——£‚ğæ‚é‚æ‚¤‚É‚·‚ê‚Îˆ—­‚È‚­‚Ä‚¢‚¢‚Ì‚Å‚Í
class EnemySpaunPoint :public MelLib::GameObject
{
	// Å’á‹——£
	static const float MIN_DISTANCE;

	static std::vector<EnemySpaunPoint*>spaunPoints;
	
	// ¶¬”
	static const unsigned int ENEMY_MAX_NUM;

	// ‚±‚±‚É“G‚Ì”z—ñì‚Á‚ÄŠÇ—
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

