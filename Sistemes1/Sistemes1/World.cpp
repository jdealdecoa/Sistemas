#include "World.h"
#include <iostream>
#include "Portal.h"
#include "Chest.h"
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
            newPosition = player->position;
            newPosition.Y -= 1;
        }
        });

    inputSystem.AddListener(K_S, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition = player->position;
            newPosition.Y += 1;
        }
        });

    inputSystem.AddListener(K_A, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition = player->position;
            newPosition.X -= 1;
        }
        });

    inputSystem.AddListener(K_D, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition = player->position;
            newPosition.X += 1;
        }
        });

    inputSystem.AddListener(K_1, [&]() {
        player->UsePotion();
        });

    inputSystem.StartListen();

    while (true) { 

        if (player->position != newPosition)
        {
                world.ActAcordinglyToNodeContent(newPosition);
                CC::Lock();
                CC::SetPosition(newPosition.X + world.GetCurrentMap().GetMapOffset().X, newPosition.Y + world.GetCurrentMap().GetMapOffset().Y);
                player->Draw(Vector2(0, 0));
                CC::Unlock();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break;
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
                    newPos.Y = GetCurrentMap().GetNodeMap()->GetSize().Y - 2;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X, GetCurrentMap().worldPos.Y - 1));
                }
                else if (portalDirection == Vector2(0, 1)) 
                {
                    newPos.Y = 1;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X, GetCurrentMap().worldPos.Y + 1));
                }
                else if (portalDirection == Vector2(-1, 0))
                {
                    newPos.X = GetCurrentMap().GetNodeMap()->GetSize().X - 2;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X - 1, GetCurrentMap().worldPos.Y));
                }
                else if (portalDirection == Vector2(1, 0))
                {
                    newPos.X = 1;
                    SetCurrentMap(Vector2(GetCurrentMap().worldPos.X + 1, GetCurrentMap().worldPos.Y));
                }

               GetCurrentMap().GetNodeMap()->SafePickNode(player->position, [&](Node* node) {
                    CC::Lock();
                    CC::SetPosition(player->position.X + GetCurrentMap().GetMapOffset().X, player->position.Y + GetCurrentMap().GetMapOffset().Y);
                    node->DrawContent(Vector2(0, 0));
                    CC::Unlock();
                    });
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                player->position = newPos;
            }
        }
        else if (node->GetContent()->nodeDisplay == DisplayType::CHEST) 
        {
            Chest* chest = dynamic_cast<Chest*>(node->GetContent());
            chest->ReciveDamage(node);
            CC::Lock();
            CC::SetPosition(newPos.X + GetCurrentMap().GetMapOffset().X, newPos.Y + GetCurrentMap().GetMapOffset().Y);
            node->DrawContent(Vector2(0, 0));
            CC::Unlock();

            newPos = player->position;
        }
        else if (node->GetContent()->nodeDisplay == DisplayType::LOOT)
        {
            Loot* loot = dynamic_cast<Loot*>(node->GetContent());
            loot->Act(player,node);
            CC::Lock();
            CC::SetPosition(player->position.X + GetCurrentMap().GetMapOffset().X, player->position.Y + GetCurrentMap().GetMapOffset().Y);
            node->DrawContent(Vector2(0, 0));
            CC::Unlock();

            player->position = newPos;
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

	currentMapPosition = Vector2(json["currentMapPosition"]["x"].asInt(), json["currentMapPosition"]["y"].asInt());


	if (!player) {
		player = new Player(Vector2(0, 0), DisplayType::PLAYER);
	}
	player->Decode(json["player"]);


	Json::Value worldMapJson = json["worldMap"];
	for (int x = 0; x < worldMap.size(); ++x) {
		for (int y = 0; y < worldMap[x].size(); ++y) {
			worldMap[x][y].Decode(worldMapJson[x][y]);
		}
	}


	currentMap.Decode(json["currentMap"]);


	//enemies.clear();
	//Json::Value enemiesJson = json["enemies"];
	//for (const auto& enemyJson : enemiesJson) {
	//	Enemy* enemy = new Enemy(Vector2(0, 0), 10000, DisplayType::ENEMY);
	//	enemy->Decode(enemyJson);
	//	enemies.push_back(enemy);
	//}
}
