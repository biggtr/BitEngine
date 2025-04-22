#include "Sandbox.h"



BitEngine::Application* BitEngine::CreateApplication()
{
    return new Sandbox();
}
