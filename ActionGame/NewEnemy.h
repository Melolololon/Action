#pragma once
#include<GameObject.h>
#include"Player.h"
#include"AttackEffect.h"

class NewEnemy:public MelLib::GameObject
{
protected:
	/// <summary>
	/// ‚Á”ò‚Ô“®‚«
	/// </summary>
	void BeBlownAwayMove();
protected:
	bool isAttack = false;
	static Player* pPlayer;

	AttackEffect currentThisAttackEffect = AttackEffect::NONE;

	// ƒvƒŒƒCƒ„[‚Ö‚ÌˆÚ“®
	//void ToPlayerMove();
	//bool CheckMove();


	static const float MOVE_SPEED;

	// Å’á‹——£
	static const float MIN_DISTANCE;



public:
	static void SetPPlayer(Player* p) { pPlayer = p; }

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
	virtual void SetStartAttackAnimation();

	bool GetIsAttack()const { return isAttack; }

	AttackEffect GetCurrentThisAttackEffect()const { return currentThisAttackEffect; }


};

