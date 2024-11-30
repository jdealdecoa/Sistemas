#include "Enemy.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "Portal.h"
#include "Wall.h"
#include "MoveCoolDown.h"

Enemy::Enemy(Vector2 startPos, int intervalMs, DisplayType type) :INodeContent(type), position(startPos), moveIntervalMs(intervalMs), running(true), threadRunning(false) {};


void Enemy::Start(Mapa* map, Player* player) {
    runningMutex.lock();
    running = true;
    runningMutex.unlock();

    //movementThread = std::thread(&Enemy::MovementLoop, this, std::ref(map), player);
    threadRunning = true; // Marcar el hilo como activo
}

Vector2 Enemy::GetPosition() {
    movementMutex.lock();
    Vector2 currentPosition = position;
    movementMutex.unlock();
    return currentPosition;
}

void Enemy::MovementLoop(Mapa* map, Player* player) {

    while (map == nullptr)
    {

    }
    MoveCooldown moveCooldown(moveIntervalMs); // Cooldown de 500 ms

    Vector2 newPosition = Vector2(0,0);

    CC::Lock();
    CC::SetPosition(position.X + map->GetMapOffset().X, position.Y + map->GetMapOffset().Y);
    Draw(Vector2(0, 0));
    CC::Unlock();


    //inputSystem.AddListener(K_W, [&]() {
    //    if (moveCooldown.TryMove()) {
    //        newPosition.Y -= 1;
    //    }
    //    });

    //inputSystem.AddListener(K_S, [&]() {
    //    if (moveCooldown.TryMove()) {
    //        newPosition.Y += 1;
    //    }
    //    });

    //inputSystem.AddListener(K_A, [&]() {
    //    if (moveCooldown.TryMove()) {
    //        newPosition.X -= 1;
    //    }
    //    });

    //inputSystem.AddListener(K_D, [&]() {
    //    if (moveCooldown.TryMove()) {
    //        newPosition.X += 1;
    //    }
    //    });

    //inputSystem.AddListener(K_ESCAPE, [&]() {
    //    runningMutex.lock();
    //    running = false; // Detener el programa
    //    runningMutex.unlock();
    //    });

    //inputSystem.StartListen();

    while (true) {

        if (player->position != newPosition)
        {
            if (map->IsValidMove(newPosition)) {

                CC::Lock();
                CC::SetPosition(newPosition.X + map->GetMapOffset().X, newPosition.Y + map->GetMapOffset().Y);
                Draw(Vector2(0, 0));
                CC::Unlock();

                map->GetNodeMap()->SafePickNode(position, [&](Node* node) {
                    CC::Lock();
                    CC::SetPosition(position.X + map->GetMapOffset().X, position.Y + map->GetMapOffset().Y);
                    node->DrawContent(Vector2(0, 0));
                    CC::Unlock();
                    });

                position = newPosition;
            }
        }


        //world.GetCurrentMap().Draw(world.GetCurrentMap().GetNodeMap());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break; // Salir si se detiene el programa
        }
        runningMutex.unlock();
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