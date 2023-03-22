#pragma once
#include <GameObject.h>

// �u�����_�[�ŏ��ƕǕ�������̖ʓ|�����ǁA�܂Ƃ߂ēǂݍ��ނƖ��ʂȓ����蔻�肪������
// �@���ŏ��ƕǂ����肷�鏈�����邩��A������g���ĕǂ̔���������΂���?����ŁA�ǂ̔���͍��܂Œʂ�ʂŗp��

// �X�e�[�W
class Stage :
    public MelLib::GameObject
{
private:
    void SetStageData(unsigned int stageNum);

    // �X�e�[�W�ԍ�
    // 0�̓e�X�g�X�e�[�W
    const int STAGE_NUM;


public:
    static void LoadResources(unsigned int stageNum);

    Stage(unsigned int stageNum);
    void Initialize()override;
    void Draw()override;
    std::shared_ptr<GameObject> GetNewPtr()override;
};

