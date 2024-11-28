#pragma once
#pragma once
#include "Mapa.h"
#include "3Nodes/Vector2.h"
#include <vector>
#include "Player.h"

class World {
private:
    Player* player;
    std::vector<std::vector<Mapa>> worldMap; // Matriz de mapas
    Mapa currentMap;
    Vector2 currentMapPosition; // Posición actual del mapa en el mapamundi

public:
    Player* GetPlayer();
    void SetCurrentMap(Vector2 delta);
    World(Vector2 worldSize, Vector2 mapSize); // Constructor para crear un mapamundi
    Mapa& GetCurrentMap(); // Devuelve el mapa actual
    bool MoveToMap(Vector2 delta); // Cambia al mapa adyacente (si existe)
};
