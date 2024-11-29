#include <json/json.h>
#include <fstream>
#include <iostream>

//#include "tutorialThreads.h"
#include <iostream>
#include "2inputSystem/InputSystem.h"
#include "Utils/ConsoleControl.h"

#include "3Nodes/Node.h"
#include "Mapa.h"
#include "MoveCoolDown.h"
#include "World.h"
#include "Empty.h"
#include "Wall.h"
#include "Portal.h"

// Variables globales
bool running = true; 
std::mutex runningMutex; 
 

void PlayerInputThread(Player* player, World& world, bool& running, std::mutex& runningMutex) {
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
            if (newPosition.Y < 0) {
                if (world.MoveToMap(Vector2(0, -1))) {
                    newPosition.Y = world.GetCurrentMap().GetNodeMap()->GetSize().Y - 1;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X, world.GetCurrentMap().worldPos.Y -1));
                }
            }
        }
        });

    inputSystem.AddListener(K_S, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.Y += 1;
            if (newPosition.Y >= world.GetCurrentMap().GetNodeMap()->GetSize().Y) {
                if (world.MoveToMap(Vector2(0, 1))) {
                    newPosition.Y = 0;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X, world.GetCurrentMap().worldPos.Y + 1));
                }
            }
        }
        });

    inputSystem.AddListener(K_A, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.X -= 1;
            if (newPosition.X < 0) {
                if (world.MoveToMap(Vector2(-1, 0))) {
                    newPosition.X = world.GetCurrentMap().GetNodeMap()->GetSize().X - 1;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X - 1, world.GetCurrentMap().worldPos.Y));
                }
            }
        }
        });

    inputSystem.AddListener(K_D, [&]() {
        if (moveCooldown.TryMove()) {
            newPosition.X += 1;
            if (newPosition.X >= world.GetCurrentMap().GetNodeMap()->GetSize().X) {
                if (world.MoveToMap(Vector2(1, 0))) {
                    newPosition.X = 0;
                    world.SetCurrentMap(Vector2(world.GetCurrentMap().worldPos.X + 1, world.GetCurrentMap().worldPos.Y ));
                }
            }
        }
        });

    inputSystem.AddListener(K_ESCAPE, [&]() {
        runningMutex.lock();
        running = false; // Detener el programa
        runningMutex.unlock();
        });

    inputSystem.StartListen();

    while (true) {
        CC::Lock();
        CC::SetPosition(30, 1);
        std::cout << player->position.X << " : " << player->position.Y;
        CC::Unlock();
        
        if (player->position != newPosition)
        {
            if (world.GetCurrentMap().IsValidMove(newPosition)) {

                CC::Lock();
                CC::SetPosition(newPosition.X + world.GetCurrentMap().GetMapOffset().X, newPosition.Y + world.GetCurrentMap().GetMapOffset().Y);
                player->Draw(Vector2(0, 0));
                CC::Unlock();

                world.GetCurrentMap().GetNodeMap()->SafePickNode(player->position, [&](Node* node) {
                    CC::Lock();
                    CC::SetPosition(player->position.X + world.GetCurrentMap().GetMapOffset().X, player->position.Y + world.GetCurrentMap().GetMapOffset().Y);
                    node->DrawContent(Vector2(0, 0));
                    CC::Unlock();
                });

                player->position = newPosition;
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

int main() {
    World world(Vector2(3, 3), Vector2(20, 10)); // Mapamundi 3x3 con mapas 20x10
    bool running = true; // Control del bucle principal
    Mapa currentmap = world.GetCurrentMap();
    world.GetCurrentMap().Draw(world.GetCurrentMap().GetNodeMap());
    std::mutex runningMutex; // Mutex para sincronizar el acceso a `running`

    std::thread inputThread(PlayerInputThread, world.GetPlayer(), std::ref(world), std::ref(running), std::ref(runningMutex));


    while (true) {
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