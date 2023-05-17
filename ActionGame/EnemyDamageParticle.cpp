#include "EnemyDamageParticle.h"

#include<Random.h>

const std::string EnemyDamageParticle::PARTICLE_NAME = "EnemyDamageParticle";

EnemyDamageParticle::EnemyDamageParticle(const MelLib::Vector3& pos, const MelLib::Vector3& vec)
	:GameObject(PARTICLE_NAME)
	, MOVE_VECTOR(vec)
{
	//spr.Create(MelLib::Texture::Get(PARTICLE_NAME));
	spr.Create(MelLib::Color(255,255,255,255));
	spr.SetPosition(pos);

	speed = MelLib::Random::GetRandomFloatNumberRangeSelect(2, 2.5, 2);
	

	deadTimer.SetMaxTime(60 * 1);
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
	spr.SetPosition(spr.GetPosition() + MOVE_VECTOR * speed);
	if (deadTimer.GetMaxOverFlag()) { eraseManager = true; }

}

void EnemyDamageParticle::Draw()
{
	spr.Draw();
}

void EnemyDamageParticle::LoadResources()
{
	//MelLib::Texture::Load("", PARTICLE_NAME);
}

