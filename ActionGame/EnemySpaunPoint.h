#pragma once
#include<GameObject.h>

#include"Player.h"
#include"WeakEnemy.h"

// ���ꓯ�m�ŋ��������悤�ɂ���Ώ������Ȃ��Ă����̂ł�
class EnemySpaunPoint :public MelLib::GameObject
{
	// �Œ዗��
	static const float MIN_DISTANCE;

	static std::vector<EnemySpaunPoint*>spaunPoints;
	
	// ������
	static const unsigned int ENEMY_MAX_NUM;

	// �����ɓG�̔z�����ĊǗ�
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

