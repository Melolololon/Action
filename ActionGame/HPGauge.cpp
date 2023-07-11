#include "HPGauge.h"

#include"Game.h"

const Player* HPGauge::pPlayer;

void HPGauge::LoadResources()
{
	std::string texturePath = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
	bool res = MelLib::Texture::Load(texturePath + "Player/hpGauge.png","hpGauge");
	res = MelLib::Texture::Load(texturePath + "Player/hpFrame.png","hpFrame");
}

HPGauge::HPGauge()
{
	sprites["gauge"].Create(MelLib::Texture::Get("hpGauge"));
	sprites["gauge"].SetPosition(MelLib::Vector2(30,30));
	sprites["frame"].Create(MelLib::Texture::Get("hpFrame"));
	sprites["frame"].SetPosition(MelLib::Vector2(30, 30));
}

void HPGauge::Update()
{

	//float textureSizeY = sprites["gauge"].GetTexture()->GetTextureSize().y;
	float rigthPos = static_cast<float>(pPlayer->GetHP()) / static_cast<float>(pPlayer->GetHPMax());
	sprites["gauge"].SetDrawRigthDownPosition(MelLib::Vector2(rigthPos, 1));
}

void HPGauge::Draw()
{
	sprites["gauge"].Draw();
	sprites["frame"].Draw();
}

