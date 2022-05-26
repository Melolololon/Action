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
		const GameObject&  object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;
};

