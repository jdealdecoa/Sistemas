#include "World.h"
#include <iostream>

Player* World::GetPlayer()
{
    return player;
}

void World::SetCurrentMap(Vector2 delta)
{
    currentMap = worldMap[delta.X][delta.Y];
}

World::World(Vector2 worldSize, Vector2 mapSize) : currentMapPosition(0, 0) {

    player = new Player(Vector2(mapSize.X / 2, mapSize.Y / 2), DisplayType::PLAYER);

    // Crear un mapamundi de tamaño `worldSize.X` x `worldSize.Y`
    for (int x = 0; x < worldSize.X; ++x) {
        std::vector<Mapa> row;
        for (int y = 0; y < worldSize.Y; ++y) {
            Mapa map;
            map.Initialize(mapSize, Vector2(10, 5), Vector2(x,y), worldSize); // Inicializar cada mapa
            row.push_back(map);
        }
        worldMap.push_back(row);
    }

    SetCurrentMap(Vector2(worldSize.X / 2, worldSize.Y / 2));


    SpawnEnemy();
}


Mapa& World::GetCurrentMap() {
    return currentMap;
}

bool World::MoveToMap(Vector2 delta) {
    Vector2 newMapPosition = currentMapPosition + delta;

    // Verificar si el nuevo mapa está dentro de los límites
    if (newMapPosition.X >= 0 && newMapPosition.X < worldMap.size() &&
        newMapPosition.Y >= 0 && newMapPosition.Y < worldMap[0].size()) {
        currentMapPosition = newMapPosition;

        return true;
    }
    return false;
}

void World::SpawnEnemy()
{
    Enemy* enemy = new Enemy(Vector2(40, 40), 10000, DisplayType::ENEMY);
    enemy->Start(&worldMap[1][1], player);
    enemies.push_back(enemy);
}

void World::PlayerInputThread(Player* player, World& world, bool& running, std::mutex& runningMutex)
{
    MoveCooldown moveCooldown(500); // Cooldown de 500 ms
    InputSystem inputSystem;
    Vector2 newPosition = player->position;

    CC::Lock();
    CC::SetPosition(player->position.X + world.GetCurrentMap().GetMapOffset().X, player->position.Y + world.GetCurrentMap().GetMapOffset().Y);
    player->Draw(Vector2(0, 0));
    CC::Unlock();


    inputSystem.AddListener(K_W, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.Y -= 1;
            if (newPosition.Y < 0) {
                if (world.MoveToMap(Vector2(0, -1))) {
                    newPosition.Y = world.GetCurrentMap().GetNodeMap()->GetSize().Y - 1;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X, world.GetCurrentMap().worldPos.Y - 1));
                }
            }
        }
        });

    inputSystem.AddListener(K_S, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.Y += 1;
            if (newPosition.Y >= world.GetCurrentMap().GetNodeMap()->GetSize().Y) {
                if (world.MoveToMap(Vector2(0, 1))) {
                    newPosition.Y = 0;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X, world.GetCurrentMap().worldPos.Y + 1));
                }
            }
        }
        });

    inputSystem.AddListener(K_A, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.X -= 1;
            if (newPosition.X < 0) {
                if (world.MoveToMap(Vector2(-1, 0))) {
                    newPosition.X = world.GetCurrentMap().GetNodeMap()->GetSize().X - 1;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X - 1, world.GetCurrentMap().worldPos.Y));
                }
            }
        }
        });

    inputSystem.AddListener(K_D, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.X += 1;
            if (newPosition.X >= world.GetCurrentMap().GetNodeMap()->GetSize().X) {
                if (world.MoveToMap(Vector2(1, 0))) {
                    newPosition.X = 0;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X + 1, world.GetCurrentMap().worldPos.Y));
                }
            }
        }
        });

    inputSystem.AddListener(K_ESCAPE, [&]() {
        runningMutex.lock();
        running = false; // Detener el programa
        runningMutex.unlock();
        });

    inputSystem.StartListen();

    while (true) {

        if (player->position != newPosition)
        {
            if (world.GetCurrentMap().IsValidMove(newPosition)) {

                CC::Lock();
                CC::SetPosition(newPosition.X + world.GetCurrentMap().GetMapOffset().X, newPosition.Y + world.GetCurrentMap().GetMapOffset().Y);
                player->Draw(Vector2(0, 0));
                CC::Unlock();

                world.GetCurrentMap().GetNodeMap()->SafePickNode(player->position, [&](Node* node) {
                    CC::Lock();
                    CC::SetPosition(player->position.X + world.GetCurrentMap().GetMapOffset().X, player->position.Y + world.GetCurrentMap().GetMapOffset().Y);
                    node->DrawContent(Vector2(0, 0));
                    CC::Unlock();
                    });

                player->position = newPosition;
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

    inputSystem.StopListen();
}

