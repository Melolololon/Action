#pragma once
#include<GameObject.h>
class NewEnemy:public MelLib::GameObject
{
protected:
	bool isAttack = false;
	
public:
	NewEnemy(const std::string& name);
	void Update()override;
	void Draw()override;

	void Hit
	(
		const GameObject& object,
		const MelLib::ShapeType3D collisionType,
		const std::string& shapeName,
		const MelLib::ShapeType3D hitObjColType,
		const std::string& hitShapeName
	)override;


	void StartAttack() { isAttack = true; }

	bool GetIsAttack()const { return isAttack; }
};

