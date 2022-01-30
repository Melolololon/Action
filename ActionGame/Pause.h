#pragma once
#include<Easing.h>
#include<Sprite2D.h>

class Pause
{
private:
	bool loadResource = false;

	// �|�[�Y�����ǂ���
	bool isPause = false;

	// �|�[�Y���I�������(�I���������s����)
	bool pauseEnd = false;

	//���ݑI�΂�Ă��鍀��
	int currentPauseSelect = 0;
	//�{�^����������đI�΂ꂽ����
	int pushPauseSelect = 0;
	// �|�[�Y���̍���
	struct PauseMenu
	{
		static const int PAUSE_END = 0;
		static const int RESTART = 1;
		static const int OPTION = 2;
		static const int CHECK_OPERATION = 3;
		static const int RETURN_TITLE = 4;
		static const int NUM_MAX = 4;
	};
	// �|�[�Y���j���[�̃X�v���C�g�̑傫��
	std::unordered_map<int, MelLib::Sprite2D>menuStringSprites;

	// �I���������ڂ̑傫��
	const MelLib::Vector2 SELECT_SCALE = 1;
	// ��I�����̍��ڂ̑傫��
	const MelLib::Vector2 UN_SELECTED_SCALE = SELECT_SCALE - 0.3f;


	// �|�[�Y�֌W�̃X�v���C�g�̃A���t�@�l
	MelLib::Easing<float>pauseSubAlpha;
	MelLib::Easing<float>pauseBackSubAlpha;

	// ����Object2D�ɂ��Ă���������
	// �|�[�Y���̔w�i
	MelLib::Sprite2D pauseBackSpr;
	MelLib::Sprite2D pauseStringSpr;

	MelLib::Sprite2D operationSpr;

	// �Q�[���I���t���O��true�ɂȂ������ǂ���
	bool returnTitleFlag = false;
	// ���X�^�[�g���邩�ǂ���
	bool isReStart = false;
	// ���������\�����邩�ǂ���
	bool drawOperation = false;

private:
	Pause(){}
	~Pause(){}

	void ActionPartPauseUpdate();
	void ActionPartPauseDraw();

	void CreateSprite();
public:
	Pause(Pause& p) = delete;
	Pause& operator=(Pause& p) = delete;
	static Pause* GetInstance();

	void Initialize();
	void Update();
	void Draw();

	bool GetIsPause() { return isPause; }
	bool GetIsEnd() { return returnTitleFlag; }
	bool GetIsReStart() { return isReStart; }
};

