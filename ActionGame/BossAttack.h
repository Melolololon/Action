#pragma once
#include<GameObject.h>
#include<FrameTimer.h>

#include"Player.h"
class BossAttack:public MelLib::GameObject
{
private:
	MelLib::FrameTimer deadTimer;

	static Player* pPlayer;
public:
	static void SetPPlayer(Player* p) { pPlayer = p; }
	
	static void LoadResources();
	
	BossAttack(const MelLib::Vector3& pos);

	void Initialize()override;
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
};
