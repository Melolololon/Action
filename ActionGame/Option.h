#pragma once

// �V�[���؂�ւ����ɕ\�����邽�߁AScene���p�����ĂȂ�
class Option 
{
private:
	// �I�v�V��������
	struct OptionMenu
	{
		static const int CAMERA_SPEED = 0;
		static const int NUM_MAX = 3;

	};
	float cameraRotSpeed = 2.0f;
private:
	int currentSelect = 0;


private:
	void LoadResources();
	Option() {}
	~Option() {}
public:
	Option(Option& o) = delete;
	Option& operator= (Option& o) = delete;
	static Option* GetInstance();

	void Initialize();
	void Update();
	void Draw();
};

