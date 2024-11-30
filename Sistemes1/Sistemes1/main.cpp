#include <json/json.h>
#include <fstream>
#include <iostream>
#include "3Nodes/Node.h"
#include "World.h"

void SaveWorld(World& world) {
	Json::Value saveData = world.Code();

	std::ofstream file("savegame.json");
	file << saveData;
	file.close();
    CC::Lock();
    CC::SetPosition(world.GetCurrentMap().GetMapOffset().X, world.GetCurrentMap().GetNodeMap()->GetSize().Y + world.GetCurrentMap().GetMapOffset().Y + 1);
    std::cout << "Game saved successfully!\t" << std::endl;
    CC::Unlock();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    CC::Lock();
    CC::SetPosition(world.GetCurrentMap().GetMapOffset().X, world.GetCurrentMap().GetNodeMap()->GetSize().Y + world.GetCurrentMap().GetMapOffset().Y + 1);
    std::cout << "                          \t" << std::endl;
    CC::Unlock(); 
}

void AutoSave(World& world, int intervalSeconds, bool& running) {
	while (running) {
		std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
		SaveWorld(world);
	}
}

void LoadWorld(World& world) {
	std::ifstream file("savegame.json");
    if (!file.fail()) 
    {
        Json::Value saveData;
        file >> saveData;
        file.close();

        world.Decode(saveData);
        CC::Lock();
        CC::SetPosition(world.GetCurrentMap().GetMapOffset().X, world.GetCurrentMap().GetNodeMap()->GetSize().Y + world.GetCurrentMap().GetMapOffset().Y + 1);
        std::cout << "Game loaded successfully!\t" << std::endl;
        CC::Unlock();
    }
}

int main() {
    World world(Vector2(3, 3), Vector2(10, 10)); // Mapamundi 3x3 con mapas 20x10
    LoadWorld(world);
    bool running = true; 
    Mapa currentmap = world.GetCurrentMap();
    world.GetCurrentMap().Draw(world.GetCurrentMap().GetNodeMap());
    std::mutex runningMutex; 

    std::thread inputThread(World::PlayerInputThread, world.GetPlayer(), std::ref(world), std::ref(running), std::ref(runningMutex));
	std::thread saveThread(AutoSave, std::ref(world), 10, std::ref(running));


    while (true) {
        CC::Lock();
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 1);
        std::cout << "Coins  :  " << world.GetPlayer()->GetCoins() << "\t";
        CC::Unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        CC::Lock();
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 3);
        std::cout << "HP     :  " << world.GetPlayer()->GetHealth() << "\t";
        CC::Unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        CC::Lock();
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 5);
        std::cout << "Lives  :  " << world.GetPlayer()->GetLives() << "\t";
        CC::Unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        CC::Lock();
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 7);
        std::cout << "Potions:  " << world.GetPlayer()->GetPotionAmount() << "\t";
        CC::Unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break; 
        }
        runningMutex.unlock();

        if (currentmap.worldPos.X != world.GetCurrentMap().worldPos.X || currentmap.worldPos.Y != world.GetCurrentMap().worldPos.Y)
        {
            world.GetCurrentMap().Draw(world.GetCurrentMap().GetNodeMap());
            currentmap = world.GetCurrentMap();
        }
    }

    inputThread.join();
	saveThread.join();
	
	while (running) {
        
	}
}
