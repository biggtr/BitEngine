#include "Bit/Core/Application.h"
extern BitEngine::Application* BitEngine::CreateApplication();

int main(int argc, char** argv)
{

   
    auto app = BitEngine::CreateApplication();

    app->Run();
    delete app;
    return 0;

}
