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
	
	enum class ThisState 
	{
		BATTLE,
		OTHER
	};
	ThisState thisState = ThisState::OTHER;

	MelLib::Vector3 jumpAttackDir; 
	int jumpAttackCount = 0;
	static const int JUMP_ATTACK_MAX_COUNT = 12;
	MelLib::FrameTimer jumpAttackTimer;

	enum class CurrentState 
	{
		NONE,
		NORMAL_1,
		JUMP_ATTACK,
	};
	CurrentState currentState = CurrentState::NONE;

	MelLib::GuiInt hp;

private:

	void Move();

	void SelectAction();
	void AttackUpdate();

	void AttackEnd();

	void NormalAttackUpdate();
	// 攻撃一覧
	void JumpAttackUpdate();

	void Rotate();
public:


	static void LoadResources();
	static void SetPPlayer(Player* p) { pPlayer = p; }

	Boss();

	std::shared_ptr<MelLib::GameObject> GetNewPtr()override;
	void Initialize()override;
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

