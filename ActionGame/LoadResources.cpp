#include "LoadResources.h"


#include"Player.h"
#include"SlushEffect.h"
#include"HPGauge.h"
#include"Wall.h"
#include"Boss.h"
#include"BossAttack.h"
#include"Water.h"
#include"EnemyDamageParticle.h"
#include"Stage.h"
#include"Wall.h"

#include"ItemEffect.h"
#include"RecoveryItem.h"
#include"RecoveryEffect.h"
#include"SandEffect.h"

#include"EnemySpaunPoint.h"
#include"WeakEnemy.h"
#include"EnemyDarknessEffect.h"

#include"ClearSprite.h"
#include"GameOverSprite.h"

#include"Pause.h"

#include"Tutorial.h"



void LoadResources::LoadTexture()
{
	HPGauge::LoadResources();
	ItemEffect::LoadResources();
	RecoveryEffect::LoadResources();
	ClearSprite::LoadResources();
	GameOverSprite::LoadResources();
	SandEffect::LoadResources();

	for (int i = 0; i < 4; i++)
	{
		std::string numStr = std::to_string(i);
		bool res = MelLib::Texture::Load("Resources/Texture/SlushEffect/e_" + numStr + ".png", "slushEffect_" + numStr);
	}

	MelLib::Texture::Load("Resources/Model/Stage/Field_01.png", "Grass");
	MelLib::Texture::Load("Resources/Model/Stage/TitleFierd_Tex.png", "Rock");
	EnemyDarknessEffect::LoadResources();
	bool r = MelLib::Texture::Load("Resources/Model/Stage/Stage_Mask_1.png", "StageMask_1");
}

void LoadResources::LoadModel()
{
	Player::LoadResources();
	Stage::LoadResources(1);
	Water::LoadResources();
	SlushEffect::LoadResources();
	Wall::LoadResources();
	//Tutorial::LoadResources();

	WeakEnemy::LoadResources();
	Boss::LoadResources();
	BossAttack::LoadResources();
	RecoveryItem::LoadResources();
	EnemyDamageParticle::LoadResources();
}

void LoadResources::LoadBGM()
{
}

void LoadResources::LoadSE()
{
}

LoadResources* LoadResources::GetInstance()
{
    static LoadResources l;
    return &l;
}

void LoadResources::Load()
{
	LoadTexture();
	LoadModel();
	LoadBGM();
	LoadSE();
}
