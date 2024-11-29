#pragma once
#include "3Nodes/Vector2.h"
#include "3Nodes/NodeMap.h"
#include "Player.h"
#include <thread>
#include <mutex>

class Enemy : public INodeContent {
private:
    Vector2 position;          // Posici�n actual del enemigo
    std::thread movementThread; // Hilo para manejar el movimiento del enemigo
    bool running;               // Controla si el enemigo sigue activo
    bool threadRunning;         // Indica si el hilo est� ejecut�ndose
    int moveIntervalMs;         // Tiempo entre movimientos en milisegundos
    std::mutex movementMutex;
    std::mutex runningMutex;

    void MovementLoop(NodeMap& map, Player* player);

public:
    Enemy(Vector2 startPos, int intervalMs);
    ~Enemy();

    void Start(NodeMap& map, Player* player); // Inicia el hilo del enemigo
    void Stop();                              // Detiene el movimiento
    Vector2 GetPosition();
    void Attack();

    // Implementaci�n de INodeContent
  //  bool Interact(Player& player) override; // L�gica al interactuar con el jugador
 // L�gica para dibujar al enemigo
    void Draw(const Vector2 offset) override {
        CC::SetColor(CC::YELLOW, CC::BLACK);
        std::cout << GetDisplayType(DisplayType::ENEMY);
        CC::SetColor(CC::WHITE, CC::BLACK);
    }
};