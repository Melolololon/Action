#pragma once
class EnemyDeadCounter final
{
private:
	EnemyDeadCounter(){}
	~EnemyDeadCounter(){}

	unsigned int enemyDeadCounter = 0;

public:
	EnemyDeadCounter(EnemyDeadCounter& e) = delete;
	EnemyDeadCounter& operator=(EnemyDeadCounter& e) = delete;
	static EnemyDeadCounter* GetInstance();

	void CountUp() { enemyDeadCounter++; }
	void Reset() { enemyDeadCounter = 0; }
	unsigned int GetCount() { return enemyDeadCounter; }
};

