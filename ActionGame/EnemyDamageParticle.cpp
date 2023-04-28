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
	spr.SetPosition(spr.GetPosition() + MOVE_VECTOR * moveSpeed);
}

void EnemyDamageParticle::Draw()
{
	spr.Draw();
}

void EnemyDamageParticle::LoadResources()
{
	//MelLib::Texture::Load("", PARTICLE_NAME);
}

