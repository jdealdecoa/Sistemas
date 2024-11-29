#include "Enemy.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "Portal.h"
#include "Wall.h"

Enemy::Enemy(Vector2 startPos, int intervalMs, DisplayType type) :INodeContent(type), position(startPos), moveIntervalMs(intervalMs), running(true), threadRunning(false) {};


void Enemy::Start(NodeMap* map, Player* player) {
    runningMutex.lock();
    running = true;
    runningMutex.unlock();

    CC::Lock();
    CC::SetPosition(position.X, position.Y);
    Draw(Vector2(0, 0));
    CC::Unlock();

    movementThread = std::thread(&Enemy::MovementLoop, this, std::ref(map), player);
    threadRunning = true; // Marcar el hilo como activo
}

Vector2 Enemy::GetPosition() {
    movementMutex.lock();
    Vector2 currentPosition = position;
    movementMutex.unlock();
    return currentPosition;
}

void Enemy::MovementLoop(NodeMap* map, Player* player) {
    //while (true) {

    //    runningMutex.lock();
    //    if (!running) {
    //        runningMutex.unlock();
    //        break; // Salir si se detiene el enemigo
    //    }
    //    runningMutex.unlock();

    //    std::this_thread::sleep_for(std::chrono::milliseconds(moveIntervalMs));

    //    movementMutex.lock();
    //    Vector2 direction(0, 0);

    //    // Calcular dirección hacia el jugador
    //    if (player != nullptr) {
    //        Vector2 playerPos = player->position;
    //        if (position.X < playerPos.X) direction.X = 1;
    //        else if (position.X > playerPos.X) direction.X = -1;

    //        if (position.Y < playerPos.Y) direction.Y = 1;
    //        else if (position.Y > playerPos.Y) direction.Y = -1;
    //    }

    //    Vector2 nextPosition = position + direction;
    //    CC::Lock();
    //    CC::SetPosition(nextPosition.X, nextPosition.Y);
    //    Draw(Vector2(0, 0));
    //    CC::Unlock();

    //    Node* targetNode = nullptr;
    //    map->SafePickNode(nextPosition, [&](Node* node) {
    //        targetNode = node;
    //        if (targetNode && targetNode->GetContent()) {
    //            if (dynamic_cast<Portal*>(targetNode->GetContent()) == nullptr &&
    //                dynamic_cast<Wall*>(targetNode->GetContent()) == nullptr) {
    //                position = nextPosition; // Mover al enemigo
    //            }
    //        }
    //        });

    //    // Verificar si el nodo es válido y no es un portal ni una pared


    //    // Verificar si atacamos al jugador
    //    //if (position == player->position) {
    //    //    Attack();
    //    //}

    //    movementMutex.unlock();
    //}
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