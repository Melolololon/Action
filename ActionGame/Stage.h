#pragma once
#include <GameObject.h>

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

