#pragma once
#pragma once
#include "Map.h"
#include "3Nodes/Vector2.h"
#include <vector>

class World {
private:
    std::vector<std::vector<Map>> worldMap; // Matriz de mapas
    Vector2 currentMapPosition; // Posición actual del mapa en el mapamundi

public:
    World(Vector2 worldSize, Vector2 mapSize); // Constructor para crear un mapamundi
    Map& GetCurrentMap(); // Devuelve el mapa actual
    bool MoveToMap(Vector2 delta); // Cambia al mapa adyacente (si existe)
};
