#pragma once
#include<GameObject.h>
class NewEnemy:public MelLib::GameObject
{
public:
	NewEnemy(const std::string& name);
	void Update()override;
	void Draw()override;
};

