#include "EnemyDamageParticle.h"

#include<Random.h>

const std::string EnemyDamageParticle::PARTICLE_NAME = "EnemyDamageParticle";

EnemyDamageParticle::EnemyDamageParticle(const MelLib::Vector3& pos, const MelLib::Vector3& vec)
	:GameObject(PARTICLE_NAME)
	, moveVector(vec)
{
	spr.Create(MelLib::Texture::Get(PARTICLE_NAME));
	//spr.Create(MelLib::Color(255,255,255,255));
	spr.SetPosition(pos);
	spr.SetBillboardFlag(true, true, true);

	moveSpeed = MelLib::Random::GetRandomFloatNumberRangeSelect(0.2f, 1.5f, 1);

	deadTimer.SetMaxTime(60.0f * 0.4f);
	deadTimer.SetStopFlag(false);
}

EnemyDamageParticle::~EnemyDamageParticle()
{
}

void EnemyDamageParticle::Initialize()
{
}

void EnemyDamageParticle::Update()
{
	float moveSpeed = MelLib::Random::GetRandomFloatNumberRangeSelect(-3, 3, 2);
	spr.SetPosition(spr.GetPosition() + moveVector * moveSpeed);
}

void EnemyDamageParticle::Draw()
{
	spr.Draw();
}

void EnemyDamageParticle::LoadResources()
{
	MelLib::Texture::Load("Resources/Texture/Effect/EnemyDarkEffect.png", PARTICLE_NAME);
}

