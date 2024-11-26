#pragma once
#include <json/json.h>
#include <string>

#include <functional>
#include <map>

class ICodable
{
public:

	typedef std::function<ICodable* ()> SubclassDecode;
	typedef std::map<std::string, SubclassDecode> DecodeMap;

private:

	static DecodeMap* GetDecodeMap();
	static void SaveDecodeProces(std::string className, SubclassDecode decodeProcess);

public:

	static std::string GetDecodeKey();

	template<typename T, typename = typename std::enable_if<std::is_base_of<ICodable, T>::value>::type>
	static void SaveDecodeProces()
	{
		SaveDecodeProces(typeid(T).name(), []() {return new T(); });
	}


	template<typename T, typename = typename std::enable_if<std::is_base_of<ICodable, T>::value>::type>
	void CodeSubClassType(Json::Value json)
	{
		json[GetDecodeKey()] = typeid(T).name();
	}

	virtual Json::Value Code() = 0;
	virtual void Decode(Json::Value json) = 0;

	template<typename T, typename = typename std::enable_if<std::is_base_of<ICodable, T>::value>::type>
	static T* FromJson(Json::Value json) 
	{
		std::string className = json[GetDecodeKey()].asSting();
		
		SubclassDecode decodeProcess = (*GetDecodeMap())[className];

		ICodable* codable = decodeProcess();

		T* codableCasted = dynamic_cast<T*>(codable);

		codable->Decode(json);
	}

};

