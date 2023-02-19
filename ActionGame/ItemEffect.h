#pragma once
#include<GameObject.h>

#include"GameItem.h"

class ItemEffect :public MelLib::GameObject
{
private:

public:
	static void LoadResources();

	ItemEffect(const MelLib::Vector3& pos);

	void Update()override;
	void Draw()override;

	void Erase();
};

