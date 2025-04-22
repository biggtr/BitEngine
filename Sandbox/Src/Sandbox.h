#pragma once

#include "Bit/Bit.h"
#include <iostream>

class Sandbox : public BitEngine::Application

{
public:
    Sandbox() = default;

protected:
    // void OnInit() override;
    //
    // void OnRender() override;
    //
    //
    // void OnUpdate(float deltaTime) override;

    
};

BitEngine::Application* BitEngine::CreateApplication();
