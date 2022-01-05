#pragma once
#include<GameObject.h>
#include<Sprite3D.h>

// �X�v���C�g����ׂ�Ύa���G�t�F�N�g���ۂ��Ȃ�?
// ���̊p�x�Ȃǂɉ����ăX�v���C�g�̊p�x���Z�b�g���āA���Ԃɉ����ē����ɂ��Ă���

// �a���G�t�F�N�g
class SlushEffect :public MelLib::GameObject
{
private:
	MelLib::Sprite3D sprite;
	float subAlpha = 0.0f;

public:
	static void LoadResources();

	SlushEffect(const MelLib::Vector3& pos,const MelLib::Vector3& slushVector,unsigned int frame);

	void Update()override;
	void Draw()override;
	
};

