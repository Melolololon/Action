#pragma once


// ���\�[�X�ǂݍ��݃N���X
class LoadResources final
{
private:
	LoadResources(LoadResources& l) = delete;
	LoadResources& operator=(LoadResources& l) = delete;

	LoadResources(){}
	~LoadResources(){}

	void LoadTexture();
	void LoadModel();
	void LoadBGM();
	void LoadSE();

public:
	static LoadResources* GetInstance();

	// ��X�V�[���w�肵�Đ؂�ւ�����悤�ɂ����炢������
	void Load();
};

