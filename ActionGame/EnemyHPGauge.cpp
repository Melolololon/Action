#include "EnemyHPGauge.h"

EnemyHPGauge::EnemyHPGauge(const int& hp)
	:REF_HP(hp)
	, MAX_HP(hp)
{
	hpSprite.Create(MelLib::Texture::Get("hpGauge"));
	hpSprite.SetBillboardFlag(true, true, true);

}

void EnemyHPGauge::SetPosition(const MelLib::Vector3& pos)
{
	hpSprite.SetPosition(pos);
}

void EnemyHPGauge::Update()
{
	MelLib::Vector2 scale = MelLib::Vector2(10,2);

	float wariai = static_cast<float>(REF_HP) / static_cast<float>(MAX_HP);
	scale.x *= wariai;
	hpSprite.SetScale(scale);
}

void EnemyHPGauge::Draw()
{
	if (REF_HP >= 1)hpSprite.Draw();
	
}
