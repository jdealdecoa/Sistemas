#include <json/json.h>
#include <fstream>
#include <iostream>

#include "4json/Bannana.h"
#include "4json/Manzana.h"

////#include "tutorialThreads.h"
//#include <iostream>
//#include "2inputSystem/InputSystem.h"
//#include "Utils/ConsoleControl.h"
//
//int main() {
//	//ThreadTutorialTest();
//	InputSystem* is = new InputSystem();
//
//	InputSystem::KeyBinding* kb1 = is->AddListener(K_1, [](){
//		CC::Lock();
//		std::cout << "Pressed 1" << std::endl;
//		CC::Unlock();
//	});
//
//	is->AddListener(K_0, [is, kb1]() {
//		is->RemoveAndDeleteListener(kb1);
//	});
//
//	is->StartListen();
//	while (true) {
//
//	}
//}
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
//

int main() {

	ICodable::SaveDecodeProces<Bannana>();
	ICodable::SaveDecodeProces<Manzana>();
	
	std::vector<Fruta*> frutas
	{
		new Bannana(),
		new Manzana(),
		new Bannana()
	};

	Json::Value jsonArray = Json::Value(Json::arrayValue);

	for(Fruta* fruta : frutas) 
	{
		jsonArray.append(fruta->Code());
	}

	std::ofstream jsonWriteFile = std::ofstream("FrutasTest.json", std::ifstream::binary);

	if (!jsonWriteFile.fail()) 
	{
		jsonWriteFile << jsonArray;
		jsonWriteFile.close();
	}

	std::cout << "Finsihed Write" << std::endl;

	std::ifstream jsonReadFile = std::ifstream("FrutasTest.json", std::ifstream::binary);
	std::vector<Fruta*> readFrutas;

	if (!jsonReadFile.fail()) 
	{
		Json::Value readedJson;

		jsonReadFile >> readedJson;

		for (Json::Value value : readedJson) 
		{
			Fruta* f = ICodable::FromJson<Fruta>(value);
			readFrutas.push_back(f);
		}
	}

	std::cout << "Finsihed Read" << std::endl;

	while (true) {

	}
}