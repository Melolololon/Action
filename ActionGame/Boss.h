#pragma once
#include"GameObject.h"
class Boss:public MelLib::GameObject
{

public:
	static void LoadResources();

	Boss(const MelLib::Vector3 pos);

	void Update()override;
	void Draw()override;
	void Hit
	(
		const GameObject* const  object,
		const MelLib::ShapeType3D& collisionType,
		const int arrayNum,
		const MelLib::ShapeType3D& hitObjColType,
		const int hitObjArrayNum
	)override;
};

