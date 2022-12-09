#pragma once
#include<GameObject.h>

#include"Player.h"
#include"NewEnemy.h"

// ���ꓯ�m�ŋ��������悤�ɂ���Ώ������Ȃ��Ă����̂ł�
class EnemySpaunPoint :public MelLib::GameObject
{
	// �Œ዗��
	static const float MIN_DISTANCE;

	static std::vector<EnemySpaunPoint*>spaunPoints;
	
	// ������
	static const unsigned int ENEMY_MAX_NUM;

	// �����ɓG�̔z�����ĊǗ�
	std::vector<std::shared_ptr<NewEnemy>>enemys;
	unsigned int enemyNum = ENEMY_MAX_NUM;

	static Player* pPlayer;

	static const float MOVE_SPEED;

	
private:
	bool CalcDistance();

	void Move();
	void Rot();
	void Attack();

	void CteateEnemy(std::shared_ptr<NewEnemy>& pEnemy, const type_info& type);
public:
	EnemySpaunPoint(const type_info& type);
	void Update()override;
	void Draw()override;

	static void SetPlayer(Player* p) { pPlayer = p; }
};
