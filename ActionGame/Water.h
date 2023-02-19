#pragma once
#include <GameObject.h>
class Water :
    public MelLib::GameObject
{
private:
    
public:
    static void LoadResources();
    
    Water();
    void Initialize()override;
    void Update()override;
    void Draw()override;
    std::shared_ptr<MelLib::GameObject> GetNewPtr()override;
};

