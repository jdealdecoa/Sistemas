#pragma once
#include "3Nodes/INodeContent.h"
#include "Utils/ConsoleControl.h"

class Portal : public INodeContent {
public:
    Portal() = default;
    ~Portal() = default;

    /// Sobrescribe el m�todo Draw para dibujar el portal
    void Draw(Vector2 offset) override {
        CC::SetColor(CC::CYAN, CC::BLACK); // Color cian sobre negro
        std::cout << "O"; // Representaci�n de un portal
        CC::SetColor(CC::WHITE, CC::BLACK);
    }
};