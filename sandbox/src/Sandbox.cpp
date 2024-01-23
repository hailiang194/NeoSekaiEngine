#include "SekaiEngine.h"


class Sandbox: public SekaiEngine::Application
{
public:
    Sandbox()
        :Application()
    {

    }

    Sandbox(const Sandbox& sandbox)
        :Application(sandbox)
    {

    }

    ~Sandbox()
    {

    }
};

SekaiEngine::Application* SekaiEngine::CreateApplication()
{
    return new Sandbox();
}