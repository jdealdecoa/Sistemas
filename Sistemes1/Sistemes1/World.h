#pragma once
#pragma once
#include "Mapa.h"
#include "3Nodes/Vector2.h"
#include <vector>
#include "Player.h"
#include "Enemy.h"

class World {
private:
    Player* player;
    std::vector<std::vector<Mapa>> worldMap; 
    Mapa currentMap;
    Vector2 currentMapPosition; 
    std::vector<Enemy*> enemies;

public:

    Player* GetPlayer();
    void SetCurrentMap(Vector2 delta);
    World(Vector2 worldSize, Vector2 mapSize); 
    Mapa& GetCurrentMap(); 
    bool MoveToMap(Vector2 delta); 
    void SpawnEnemy();
};
