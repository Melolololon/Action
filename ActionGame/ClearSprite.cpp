#include "ClearSprite.h"

ClearSprite::ClearSprite()
{
	sprites["main"].Create(MelLib::Color(255,255,0,255));
	sprites["main"].SetScale(MelLib::Vector2(1000,1000));
}

void ClearSprite::Update()
{

}

void ClearSprite::Draw()
{
	sprites["main"].Draw();
}
