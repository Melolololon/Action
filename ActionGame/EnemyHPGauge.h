#pragma once
#include<Sprite3D.h>


class EnemyHPGauge
{
private:
	MelLib::Sprite3D hpSprite;
	const int& REF_HP;
	const int MAX_HP;
public:
	EnemyHPGauge(const int& hp);
	void SetPosition(const MelLib::Vector3& pos);
	void Update();
	void Draw();

};

