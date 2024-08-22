
#include "Animator.h"

Animator CreateAnimator()
{
    Animator a;
    a.currFrame = 0;
    a.frameCounter = 0;
    a.recData = std::array<math::rec, 2>();

    return a;
}