#include "SlushEffect.h"
#include"Game.h"

#include<LibMath.h>

void SlushEffect::LoadResources()
{
	const std::string TEXTURE_PATH = Game::GetInstance()->GetPath(Game::ResourcePath::TEXTURE);
	MelLib::Texture::Load(TEXTURE_PATH + "Player/SlushEffect.png", "slushEffect");
}

SlushEffect::SlushEffect(const MelLib::Vector3& pos, const MelLib::Vector3& slushVector, const MelLib::Vector3& moveDirection,const unsigned int frame, const MelLib::Vector3& direction)
{
	SetPosition(pos);

	// frame�ɉ����Ĕ͈͂𒲐�
	static const unsigned int X_AREA = 5;
	unsigned int areaStart = 0;
	if (frame >= 10)areaStart = X_AREA * 10;
	else areaStart = X_AREA * frame;

	
	// ���̃x�N�g�������Ɋp�x��ݒ�
	// �v���C���[�̌��������f������

	static const MelLib::Vector3 START_ANGLE[2] = { MelLib::Vector3(7,0,0),MelLib::Vector3(-7,0,0) };
	
	// �ړ��� �v���C���[�̌����ɉ����ĉ�]������
	MelLib::Vector3 movePosition[2] = { MelLib::Vector3(0,0,-2),MelLib::Vector3(0,0,2) };


	// �܂��̓v���C���[�̌����ɉ����ĉ�]
	MelLib::Vector3 angle;
	//angle.y = -MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), true);

	// ���̃x�N�g����XYZ���ƂɓK��
	// X�����Ɍ����ق�Z�����A
	// Z�����Ɍ����ق�Y���̉�]�p�x�����Z
	angle.z += MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(slushVector.x, slushVector.y), true) - 90;
	angle.y += MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(slushVector.z, slushVector.y), true) - 90;

	// X�����ɐU��ꍇ�A��]
	angle.y += MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(moveDirection.x, moveDirection.y), true);

	// Z�����ɐU��ꍇ�A��]

	for (int i = 0; i < _countof(sprite); i++)
	{
		// �ړ��ʂ���]������
		movePosition[i] = 
			MelLib::LibMath::RotateVector3(movePosition[i], MelLib::Vector3(0, 1, 0), MelLib::LibMath::Vector2ToAngle(MelLib::Vector2(direction.x, direction.z), true));

		sprite[i].Create(MelLib::Texture::Get("slushEffect"));
		sprite[i].SetPosition(pos + movePosition[i]);
	
		sprite[i].SetDrawLeftUpPosition(MelLib::Vector2(areaStart, 0));
		sprite[i].SetDrawRigthDownPosition(MelLib::Vector2(areaStart + X_AREA, sprite[i].GetTexture()->GetTextureSize().y));

		sprite[i].SetAngle(angle + START_ANGLE[i]);
		
		sprite[i].SetScale(MelLib::Vector2(10,30));

	}

	// �X�v���C�g���ƁAZ�������������Ƃ��Ɍ����Ȃ�
	// ���ƌ��̐i�s�������l�����ĉ�]�����Ȃ��Ƃ����Ȃ�
	// �������������ĂĂ��A�㉺�ƍ��E�ň���Ă���
	// �g�[���X�����������f��(�~����؂����ĂԂ������̂���)������āA���ԑ����悤�ɂ��āA�\���͈̓e�N�X�`���œ����x�w�肷��΂��������ɂȂ�?
	// ���ō��񂾂�����A�؂������g�[���X�p�ӂ��ăe�N�X�`���ƃ}�e���A�������p�ӂ��ăt���[���ɉ����Đ؂�ւ��Ă��悳����
	// ���̊p�x�ɂ���Ă̓X�v���C�g�ł����Ȃ��\�������邩��A�X�v���C�g�_���������玎�������ł�������������Ȃ�
	// �Q�[���̓�������ĎQ�l�ɂ����ق�����������

	// �X�v���C�g2�����ɂ��Ԃ���悤�ɔ��̎��ɕ��ׂ���g�[���X���Ԃ������Ɠ����悤�ɂȂ�̂ł�?

	// ���̐i�s�����ƌ��̌����ǂ������K�v

	// ���̖ʂ̌����擾�����ق�������?

	// ��������ۗ�
}

void SlushEffect::Update()
{
	subAlpha += 10.0f;
	if (subAlpha >= 100.0f)eraseManager = true;

	for (int i = 0; i < _countof(sprite); i++) 
	{
		sprite[i].SetSubColor(MelLib::Color(0, 0, 0, subAlpha));
	}
}

void SlushEffect::Draw()
{
	for (int i = 0; i < _countof(sprite); i++)
	{
		sprite[i].Draw();
	}
}
