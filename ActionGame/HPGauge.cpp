#include "HPGauge.h"

#include"Game.h"

const Player* HPGauge::pPlayer;

void HPGauge::LoadResources()
{
	std::string texturePath = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
	bool res = MelLib::Texture::Load(texturePath + "Player/hpGauge.png","hpGauge");
}

HPGauge::HPGauge()
{
	sprites["gauge"].Create(MelLib::Texture::Get("hpGauge"));
	sprites["gauge"].SetPosition(MelLib::Vector2(30,30));
	
}

void HPGauge::Update()
{

	//float textureSizeY = sprites["gauge"].GetTexture()->GetTextureSize().y;
	sprites["gauge"].SetDrawRigthDownPosition(MelLib::Vector2(pPlayer->GetHP() / pPlayer->GetHPMax(), 1));
}

void HPGauge::Draw()
{
	sprites["gauge"].Draw();
}

