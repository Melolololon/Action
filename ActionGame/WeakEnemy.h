#pragma once
#include "NewEnemy.h"

// �ア�G
class WeakEnemy :
    public NewEnemy
{
private:

	void Attack();
public:


	WeakEnemy(const MelLib::Vector3& pos);
	void Update()override;
	void Draw()override;

};

