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

	const std::string CLASS_NAME;
private:
	bool CalcDistance();

	void Move();
	void Rot();
	void Attack();

	void CteateEnemy(std::shared_ptr<NewEnemy>& pEnemy);
public:
	EnemySpaunPoint(const std::string& className);
	void Update()override;
	void Draw()override;
	std::shared_ptr<GameObject> GetNewPtr()override;

	static void SetPlayer(Player* p) { pPlayer = p; }
};

