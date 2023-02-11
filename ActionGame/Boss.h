#pragma once
#include"GameObject.h"

#include<FrameTimer.h>

#include"Player.h"

class Boss:public MelLib::GameObject
{
private:
	static Player* pPlayer;

	// 移動や攻撃を制御するタイマー。例　遠くにいる時、このタイマーの時間に応じて移動するか遠距離攻撃するかを決める
	MelLib::FrameTimer actionTimer;
public:


	static void LoadResources();
	static void SetPlayer();

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


	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

	void Move();
};

