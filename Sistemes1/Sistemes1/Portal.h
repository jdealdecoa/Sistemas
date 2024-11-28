#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"

class Portal : public INodeContent {
public:
    Portal() = default;
    ~Portal() = default;

    void Draw(Vector2 offset) override {
        CC::SetColor(CC::CYAN, CC::BLACK); 
        std::cout << "O"; 
        CC::SetColor(CC::WHITE, CC::BLACK);
    }
};