#include "SlushEffect.h"
#include"Game.h"
void SlushEffect::LoadResources()
{
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePathType::TEXTURE);
	MelLib::Texture::Load(TEXTURE_PATH + "Player/SlushEffect.png", "slushEffect");
}

SlushEffect::SlushEffect(const MelLib::Vector3& pos, const MelLib::Vector3& slushVector,const unsigned int frame)
{
	SetPosition(pos);

	sprite.Create(MelLib::Texture::Get("slushEffect"));
	sprite.SetPosition(pos);
	
	// frameに応じて範囲を調整
	static const unsigned int X_AREA = 5;
	unsigned int areaStart = 0;
	if (frame >= 10)areaStart = X_AREA * 10;
	else areaStart = X_AREA * frame;

	sprite.SetDrawLeftUpPosition(MelLib::Vector2(areaStart, 0));
	sprite.SetDrawRigthDownPosition(MelLib::Vector2(areaStart + X_AREA, sprite.GetTexture()->GetTextureSize().y));

	// 刀のベクトルを元に角度を設定

}

void SlushEffect::Update()
{
	subAlpha += 10.0f;
	if (subAlpha >= 100.0f)eraseManager = true;
}

void SlushEffect::Draw()
{
	sprite.Draw();
}
