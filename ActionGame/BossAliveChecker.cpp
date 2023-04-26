#include "BossAliveChecker.h"

BossAliveChecker* BossAliveChecker::GetInstance()
{
    static BossAliveChecker i;
    return &i;

}
