#include <json/json.h>
#include <fstream>
#include <iostream>

//#include "tutorialThreads.h"
#include <iostream>
#include "2inputSystem/InputSystem.h"
#include "Utils/ConsoleControl.h"

#include "Map.h"
#include "MoveCoolDown.h"


// Variables globales
bool running = true; 
std::mutex runningMutex; 
 

void PlayerInputThread(Vector2& playerPosition, Map& gameMap) {
    MoveCooldown moveCooldown(500); 

    InputSystem inputSystem;

    inputSystem.AddListener(K_W, [&]() {
        if (moveCooldown.TryMove()) { 
            Vector2 newPosition = playerPosition;
            newPosition.Y -= 1;
            if (gameMap.IsValidMove(newPosition)) {
                playerPosition = newPosition;
            }
        }
        });

    inputSystem.AddListener(K_S, [&]() {
        if (moveCooldown.TryMove()) {
            Vector2 newPosition = playerPosition;
            newPosition.Y += 1;
            if (gameMap.IsValidMove(newPosition)) {
                playerPosition = newPosition;
            }
        }
        });

    inputSystem.AddListener(K_A, [&]() {
        if (moveCooldown.TryMove()) {
            Vector2 newPosition = playerPosition;
            newPosition.X -= 1;
            if (gameMap.IsValidMove(newPosition)) {
                playerPosition = newPosition;
            }
        }
        });

    inputSystem.AddListener(K_D, [&]() {
        if (moveCooldown.TryMove()) {
            Vector2 newPosition = playerPosition;
            newPosition.X += 1;
            if (gameMap.IsValidMove(newPosition)) {
                playerPosition = newPosition;
            }
        }
        });

    inputSystem.AddListener(K_ESCAPE, [&]() {
        runningMutex.lock();
        running = false;
        runningMutex.unlock();
        });

    inputSystem.StartListen();

    while (true) {
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break;
        }
        runningMutex.unlock();
        gameMap.Draw(gameMap.GetNodeMap(), playerPosition, {});
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    inputSystem.StopListen();
}

int main() {
	
    Map gameMap;
    Vector2 mapSize(20, 10);
    Vector2 offset(0, 0);
    gameMap.Initialize(mapSize, offset);

    Vector2 playerPosition(1, 1);

    std::thread inputThread(PlayerInputThread, std::ref(playerPosition), std::ref(gameMap));

    while (true) {
        runningMutex.lock();
        if (!running) {
            runningMutex.unlock();
            break;
        }
        runningMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    inputThread.join();




	
	while (true) {

	}
}
class Player {
public: 

	int life = 0;
	std::string name = "Test player";
	unsigned int coins = 0;

	Player() {

	}
	~Player() {

	}

	//De un json kei la variable que me interesan y las asigno a esta clase
	void Decode(Json::Value json){
		life = json["life"].asInt(); 
		name = json["name"].asString();
		coins = json["coins"].asUInt();
	}
	//A partir de esta clase creo un json rellenando con las variables de la clase
	Json::Value Enconde() {
		Json::Value json;
		
		json["life"] = life;
		json["name"] = name;
		json["coins"] = coins;

		return json;
	}

	static Player* FromJSON(Json::Value json) {
		Player* player = new Player();
		player->Decode(json);
		return player;
	}
};


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