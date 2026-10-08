// This file must NOT compile, and that is the whole point of it.
//
// Every family takes its direction as a template argument, so the only way to reject a
// direction naming none of the three is to reject it at compile time. A passing gtest
// cannot express that: the program either builds or it does not. So this snippet is
// deliberately kept out of engine_test and compiled on its own by the pipeline, which
// expects the build to fail on the static_assert in each header.
//
// If this file ever compiles, a static_assert has been dropped or loosened. Do not "fix"
// it by editing the modes below.

#include "SekaiEngine/Animation/Transitions/Back.h"
#include "SekaiEngine/Animation/Transitions/Bounce.h"
#include "SekaiEngine/Animation/Transitions/Circ.h"
#include "SekaiEngine/Animation/Transitions/Cubic.h"
#include "SekaiEngine/Animation/Transitions/Expo.h"
#include "SekaiEngine/Animation/Transitions/Elastic.h"
#include "SekaiEngine/Animation/Transitions/Quad.h"
#include "SekaiEngine/Animation/Transitions/Quart.h"
#include "SekaiEngine/Animation/Transitions/Quint.h"
#include "SekaiEngine/Animation/Transitions/Sine.h"

using namespace SekaiEngine;
using namespace SekaiEngine::Animation;

// 7 is not Ease::In, Ease::Out or Ease::InOut. The cast is needed because Ease::Mode is a
// scoped enum, so a raw integer would be rejected for the wrong reason. An object rather
// than a bare type is what forces the class body, and so the static_assert, to be read.
template<Ease::Mode MODE>
struct InstantiateInThisDirection
{
    Quad<MODE> quad{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Cubic<MODE> cubic{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Quart<MODE> quart{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Quint<MODE> quint{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Sine<MODE> sine{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Expo<MODE> expo{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Circ<MODE> circ{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Back<MODE> back{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Elastic<MODE> elastic{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
    Bounce<MODE> bounce{0.0f, 1.0f, Timestep(1.0f), [](const float&){}};
};

InstantiateInThisDirection<static_cast<Ease::Mode>(7)> invalid;

int main() { return 0; }
