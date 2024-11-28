#include "World.h"
#include <iostream>

World::World(Vector2 worldSize, Vector2 mapSize) : currentMapPosition(0, 0) {
    // Crear un mapamundi de tamaño `worldSize.X` x `worldSize.Y`
    for (int x = 0; x < worldSize.X; ++x) {
        std::vector<Map> row;
        for (int y = 0; y < worldSize.Y; ++y) {
            Map map;
            map.Initialize(mapSize, Vector2(0, 0)); // Inicializar cada mapa
            row.push_back(map);
        }
        worldMap.push_back(row);
    }
}
Map& World::GetCurrentMap() {
    return worldMap[currentMapPosition.X][currentMapPosition.Y];
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
