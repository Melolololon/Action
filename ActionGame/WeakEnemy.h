#pragma once
#include <GameObject.h>

// Žã‚¢“G
class WeakEnemy :
    public MelLib::GameObject
{
private:

	void Attack();
public:


	WeakEnemy(const MelLib::Vector3& pos);
	void Update()override;
	void Draw()override;

};

