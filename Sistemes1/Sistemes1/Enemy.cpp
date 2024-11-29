#include "Enemy.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "Portal.h"
#include "Wall.h"
#include "3Nodes/NodeMap.h"

Enemy::Enemy(Vector2 startPos, int intervalMs)
    : position(startPos), moveIntervalMs(intervalMs), running(true), threadRunning(false) {}

Enemy::~Enemy() {
    Stop();
}

void Enemy::Start(NodeMap& map, Player* player) {
    runningMutex.lock();
    running = true;
    runningMutex.unlock();

    movementThread = std::thread(&Enemy::MovementLoop, this, std::ref(map), player);
    threadRunning = true; // Marcar el hilo como activo
}

void Enemy::Stop() {
    runningMutex.lock();
    running = false;
    runningMutex.unlock();

    if (threadRunning) { // Solo intentamos unir si sabemos que el hilo está corriendo
        movementThread.join();
        threadRunning = false;
    }
}

Vector2 Enemy::GetPosition() {
    movementMutex.lock();
    Vector2 currentPosition = position;
    movementMutex.unlock();
    return currentPosition;
}

void Enemy::MovementLoop(NodeMap& map, Player* player) {
    while (true) {
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break; // Salir si se detiene el enemigo
        }
        runningMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(moveIntervalMs));

        movementMutex.lock();
        Vector2 direction(0, 0);

        // Calcular dirección hacia el jugador
        if (player) {
            Vector2 playerPos = player->position;
            if (position.X < playerPos.X) direction.X = 1;
            else if (position.X > playerPos.X) direction.X = -1;

            if (position.Y < playerPos.Y) direction.Y = 1;
            else if (position.Y > playerPos.Y) direction.Y = -1;
        }

        Vector2 nextPosition = position + direction;

        Node* targetNode = nullptr;
        map.SafePickNode(nextPosition, [&](Node* node) {
            targetNode = node;
            });

        // Verificar si el nodo es válido y no es un portal ni una pared
        if (targetNode && targetNode->GetContent()) {
            if (dynamic_cast<Portal*>(targetNode->GetContent()) == nullptr &&
                dynamic_cast<Wall*>(targetNode->GetContent()) == nullptr) {
                position = nextPosition; // Mover al enemigo
            }
        }

        // Verificar si atacamos al jugador
        if (position == player->position) {
            Attack();
        }

        movementMutex.unlock();
    }
}

void Enemy::Attack() {
    std::cout << "Enemy at " << position.X << ", " << position.Y << " attacks the player!" << std::endl;
    // Aquí podrías implementar lógica para reducir la vida del jugador o iniciar combate
}

// Métodos de INodeContent
//bool Enemy::Interact(Player& player) {
//    std::cout << "Enemy interacts with player!" << std::endl;
//    return false; 
//}