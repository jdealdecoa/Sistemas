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
	std::cout << "Game saved successfully!" << std::endl;

}

void AutoSave(World& world, int intervalSeconds, bool& running) {
	while (running) {
		std::this_thread::sleep_for(std::chrono::seconds(intervalSeconds));
		SaveWorld(world);
	}
}

void LoadWorld(World& world) {
	std::ifstream file("savegame.json");
	Json::Value saveData;
	file >> saveData;
	file.close();

	world.Decode(saveData);
	std::cout << "Game loaded successfully!" << std::endl;
}

int main() {
    World world(Vector2(3, 3), Vector2(20, 10)); // Mapamundi 3x3 con mapas 20x10
    bool running = true; // Control del bucle principal
    Mapa currentmap = world.GetCurrentMap();
    world.GetCurrentMap().Draw(world.GetCurrentMap().GetNodeMap());
    std::mutex runningMutex; // Mutex para sincronizar el acceso a `running`

    std::thread inputThread(World::PlayerInputThread, world.GetPlayer(), std::ref(world), std::ref(running), std::ref(runningMutex));
	std::thread saveThread(AutoSave, std::ref(world), 10, std::ref(running));


    while (true) {
        CC::Lock();
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 1);
        std::cout << "Coins  :  " << world.GetPlayer()->GetCoins() << "\t";
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 3);
        std::cout << "HP     :  " << world.GetPlayer()->GetHealth() << "\t";
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 5);
        std::cout << "Lives  :  " << world.GetPlayer()->GetLives() << "\t";
        CC::SetPosition(world.GetCurrentMap().GetNodeMap()->GetSize().X + world.GetCurrentMap().GetMapOffset().X + 6, world.GetCurrentMap().GetMapOffset().Y + 7);
        std::cout << "Potions:  " << world.GetPlayer()->GetPotionAmount() << "\t";
        CC::Unlock();
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break; // Salir si se detiene el programa
        }
        runningMutex.unlock();

        if (currentmap.worldPos.X != world.GetCurrentMap().worldPos.X || currentmap.worldPos.Y != world.GetCurrentMap().worldPos.Y)
        {
            world.GetCurrentMap().Draw(world.GetCurrentMap().GetNodeMap());
            currentmap = world.GetCurrentMap();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    inputThread.join();
	saveThread.join();
	
	while (running) {
        
	}
}
//class Player {
//public: 
//
//	int life = 0;
//	std::string name = "Test player";
//	unsigned int coins = 0;
//
//	Player() {
//
//	}
//	~Player() {
//
//	}
//
//	//De un json kei la variable que me interesan y las asigno a esta clase
//	void Decode(Json::Value json){
//		life = json["life"].asInt(); 
//		name = json["name"].asString();
//		coins = json["coins"].asUInt();
//	}
//	//A partir de esta clase creo un json rellenando con las variables de la clase
//	Json::Value Enconde() {
//		Json::Value json;
//		
//		json["life"] = life;
//		json["name"] = name;
//		json["coins"] = coins;
//
//		return json;
//	}
//
//	static Player* FromJSON(Json::Value json) {
//		Player* player = new Player();
//		player->Decode(json);
//		return player;
//	}
//};


//int main() {
//
//	ICodable::SaveDecodeProces<Bannana>();
//	ICodable::SaveDecodeProces<Manzana>();
//	
//	std::vector<Fruta*> frutas
//	{
//		new Bannana(),
//		new Manzana(),
//		new Bannana()
//	};
//
//	Json::Value jsonArray = Json::Value(Json::arrayValue);
//
//	for(Fruta* fruta : frutas) 
//	{
//		jsonArray.append(fruta->Code());
//	}
//
//	std::ofstream jsonWriteFile = std::ofstream("FrutasTest.json", std::ifstream::binary);
//
//	if (!jsonWriteFile.fail()) 
//	{
//		jsonWriteFile << jsonArray;
//		jsonWriteFile.close();
//	}
//
//	std::cout << "Finsihed Write" << std::endl;
//
//	std::ifstream jsonReadFile = std::ifstream("FrutasTest.json", std::ifstream::binary);
//	std::vector<Fruta*> readFrutas;
//
//	if (!jsonReadFile.fail()) 
//	{
//		Json::Value readedJson;
//
//		jsonReadFile >> readedJson;
//
//		for (Json::Value value : readedJson) 
//		{
//			Fruta* f = ICodable::FromJson<Fruta>(value);
//			readFrutas.push_back(f);
//		}
//	}
//
//	std::cout << "Finsihed Read" << std::endl;
//
//	while (true) {
//
//	}
//}