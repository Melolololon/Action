#include "Wall.h"
#include "Game.h"

// static�ϐ��ŃJ�E���g���āA����̈ʒu�ɕǏo��?

unsigned int Wall::wallSetCount;

void Wall::LoadResources()
{
	// �e�N�X�`���ǂݍ���
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);

	MelLib::Texture::Load(TEXTURE_PATH + "Wall.png","wall");
}

Wall::Wall(const MelLib::Vector3& pos, const MelLib::Vector3& angle, const MelLib::Vector2& scale)
	:GameObject("Wall")
{
	SetPosition(pos);

	wallSpr.Create(MelLib::Texture::Get("wall"));

	
	wallSpr.SetAngle(angle);
	wallSpr.SetScale(scale);
	wallSpr.SetPosition(pos);

	boardDatas["main"].resize(1);
	boardDatas["main"][0].SetAngle(angle);
	boardDatas["main"][0].SetSize(scale);
	boardDatas["main"][0].SetPosition(pos);

	
	
}

void Wall::Update()
{
	// �摜�̃X�N���[��
	static const float AREA_SCROLL_SPEED = 0.01f;
	drawAreaLeftUp.y -= AREA_SCROLL_SPEED;
	drawAreaRightDown.y -= AREA_SCROLL_SPEED;
	wallSpr.SetDrawLeftUpPosition(drawAreaLeftUp);
	wallSpr.SetDrawRigthDownPosition(drawAreaRightDown);

}

void Wall::Draw()
{
	wallSpr.Draw();
}
