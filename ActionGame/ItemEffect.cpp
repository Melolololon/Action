#include "ItemEffect.h"

void ItemEffect::LoadResources()
{
	MelLib::ModelData::Load("Resources/Model/Item/ItemEffect.obj", true, "ItemEffect");
}

ItemEffect::ItemEffect(const MelLib::Vector3& pos)
	:GameObject("ItemEffect")
{
	SetPosition(pos);
	modelObjects["main"].SetPosition(pos);
	SetScale(5.0f);
	modelObjects["main"].Create(MelLib::ModelData::Get("ItemEffect"),"ItemEffect");
}

void ItemEffect::Update()
{
	modelObjects["main"].SetAddUV(modelObjects["main"].GetAddUV() + MelLib::Vector2(0, 0.01f));
}

void ItemEffect::Draw()
{
	AllDraw();
}

void ItemEffect::Erase()
{
	eraseManager = true;
}
