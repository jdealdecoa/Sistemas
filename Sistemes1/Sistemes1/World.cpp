#include "World.h"
#include <iostream>
#include "Portal.h"

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


    //SpawnEnemy();
}


Mapa& World::GetCurrentMap() {
    return currentMap;
}

void World::SpawnEnemy()
{
    //Enemy* enemy = new Enemy(Vector2(40, 40), 10000, DisplayType::ENEMY);
    //enemy->Start(&worldMap[1][1], player);
    //enemies.push_back(enemy);
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
        }
        });

    inputSystem.AddListener(K_S, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.Y += 1;
        }
        });

    inputSystem.AddListener(K_A, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.X -= 1;
        }
        });

    inputSystem.AddListener(K_D, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.X += 1;
        }
        });

    inputSystem.AddListener(K_ESCAPE, [&]() {
        runningMutex.lock();
        running = false; // Detener el programa
        runningMutex.unlock();
        });

    inputSystem.AddListener(K_1, [&]() {
        player->UsePotion();
        });

    inputSystem.StartListen();

    while (true) {

        if (player->position != newPosition)
        {
            if (world.GetCurrentMap().IsValidMove(newPosition)) {

                world.ActAcordinglyToNodeContent(newPosition);
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

void World::ActAcordinglyToNodeContent(Vector2& newPos)
{
    GetCurrentMap().GetNodeMap()->SafePickNode(newPos, [&](Node* node) {
        if (node->GetContent()->nodeDisplay == DisplayType::EMPTY)
        {
            CC::Lock();
            CC::SetPosition(newPos.X + GetCurrentMap().GetMapOffset().X, newPos.Y + GetCurrentMap().GetMapOffset().Y);
            player->Draw(Vector2(0, 0));
            CC::Unlock();

            GetCurrentMap().GetNodeMap()->SafePickNode(player->position, [&](Node* node) {
                CC::Lock();
                CC::SetPosition(player->position.X + GetCurrentMap().GetMapOffset().X, player->position.Y + GetCurrentMap().GetMapOffset().Y);
                node->DrawContent(Vector2(0, 0));
                CC::Unlock();
                });

            player->position = newPos;
        }
        else if (node->GetContent()->nodeDisplay == DisplayType::WALL)
        {
            newPos = player->position;
        }
        else if (node->GetContent()->nodeDisplay == DisplayType::PORTAL) {
            Portal* portal = dynamic_cast<Portal*>(node->GetContent());
            if (portal) {
                Vector2 portalDirection = portal->direction;
                if (portalDirection == Vector2(0, -1))
                {
                    newPos.Y = GetCurrentMap().GetNodeMap()->GetSize().Y - 1;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X, GetCurrentMap().worldPos.Y - 1));
                }
                else if (portalDirection == Vector2(0, 1)) 
                {
                    newPos.Y = 0;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X, GetCurrentMap().worldPos.Y + 1));
                }
                else if (portalDirection == Vector2(-1, 0))
                {
                    newPos.X = GetCurrentMap().GetNodeMap()->GetSize().X - 1;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X - 1, GetCurrentMap().worldPos.Y));
                }
                else if (portalDirection == Vector2(1, 0))
                {
                    newPos.X = 0;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X + 1, GetCurrentMap().worldPos.Y));
                }
            }
        }
        });

}

Json::Value World::Code() {
	Json::Value json;

	// Guardar la posición del mapa actual
	json["currentMapPosition"]["x"] = currentMapPosition.X;
	json["currentMapPosition"]["y"] = currentMapPosition.Y;

	// Guardar el jugador
	if (player) {
		json["player"] = player->Code();
	}

	// Guardar todos los mapas del mundo
	Json::Value worldMapJson;
	for (int x = 0; x < worldMap.size(); ++x) {
		for (int y = 0; y < worldMap[x].size(); ++y) {
			worldMapJson[x][y] = worldMap[x][y].Code();
		}
	}
	json["worldMap"] = worldMapJson;

	// Guardar el mapa actual
	json["currentMap"] = currentMap.Code();

	// Guardar enemigos
	//Json::Value enemiesJson;
	//for (Enemy* enemy : enemies) {
	//	if (enemy) {
	//		enemiesJson.append(enemy.Code());
	//	}
	//}
	//json["enemies"] = enemiesJson;

	CodeSubClassType<World>(json); // Guardar tipo de clase
	return json;
}

void World::Decode(Json::Value json) {
	// Restaurar posición del mapa actual
	currentMapPosition = Vector2(json["currentMapPosition"]["x"].asInt(), json["currentMapPosition"]["y"].asInt());

	// Restaurar jugador
	if (!player) {
		player = new Player(Vector2(0, 0), DisplayType::PLAYER);
	}
	player->Decode(json["player"]);

	// Restaurar todos los mapas del mundo
	Json::Value worldMapJson = json["worldMap"];
	for (int x = 0; x < worldMap.size(); ++x) {
		for (int y = 0; y < worldMap[x].size(); ++y) {
			worldMap[x][y].Decode(worldMapJson[x][y]);
		}
	}

	// Restaurar mapa actual
	currentMap.Decode(json["currentMap"]);

	// Restaurar enemigos
	//enemies.clear();
	//Json::Value enemiesJson = json["enemies"];
	//for (const auto& enemyJson : enemiesJson) {
	//	Enemy* enemy = new Enemy(Vector2(0, 0), 10000, DisplayType::ENEMY);
	//	enemy->Decode(enemyJson);
	//	enemies.push_back(enemy);
	//}
}
