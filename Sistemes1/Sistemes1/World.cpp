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

    player = new Player(Vector2(mapSize.X / 2, mapSize.Y / 2));

    // Crear un mapamundi de tamaño `worldSize.X` x `worldSize.Y`
    for (int x = 0; x < worldSize.X; ++x) {
        std::vector<Mapa> row;
        for (int y = 0; y < worldSize.Y; ++y) {
            Mapa map;
            map.Initialize(mapSize , player, Vector2(10, 5), Vector2(x,y), worldSize); // Inicializar cada mapa
            row.push_back(map);
        }
        worldMap.push_back(row);
    }
    SetCurrentMap(Vector2(worldSize.X/2, worldSize.Y/2));
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
