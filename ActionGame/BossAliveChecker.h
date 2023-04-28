#pragma once
#include"Boss.h"
class BossAliveChecker final
{
private:
	BossAliveChecker(){}
	~BossAliveChecker(){}

private:
	Boss* pBoss = nullptr;

public:
	BossAliveChecker(BossAliveChecker& b) = delete;
	BossAliveChecker& operator= (BossAliveChecker& b) = delete;
	static BossAliveChecker* GetInstance();

	void AddBoss(Boss* boss) { pBoss = boss; }
	bool GetBossDeadFlag() { return pBoss->GetEraseManager(); }
};

