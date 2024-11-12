#pragma once
#include <chrono> //gestiona el tiempo 
#include <thread> //per utilitzar dos codis executant-se a la vegada 
#include <iostream>
#include <list>
#include <string>
#include <mutex> //encara qe executem dos codis a la vegada, evitem els errors dels	que no volem 

//ignora la linia, com si posessim comentari. Region es per agrupar el codi
#pragma once Funcions Declarations Header              

void PrintElapsedTime(std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end, std::string theadName);  //es como un for, va sumando 

void Count(unsigned long long maxCounter, std::string threadName);
void Example01();
void Example02();
void Example03();
void Example04();
void Example05();
void Example06();
void Example07();



#pragma endregion

#pragma region Globar Variables

std::chrono::system_clock::time_point startDate;

std::mutex consoleMutex;



#pragma endregion

void ThreadTutorialTest() {
	startDate = std::chrono::system_clock::now();

	int exampleUsed = 3;
	switch (exampleUsed) {
	case 0: 
		Example01();
		break;
	case 1:
		Example02();
		break;
	case 2:
		Example03();
		break;
	case 3:
		Example04();
		break;
	case 4:
		Example05();
		break;
	case 5:
		Example06();
		break;
	case 6:
		Example07();
		break;
	default:
		break;


	}

	std::cout << "Example 0" << exampleUsed << "finished" << std::endl;
	std::string text = "";
	std::getline(std::cin, text);
}
void PrintElapsedTime(std::chrono::system_clock::time_point start, std::chrono::system_clock::time_point end, std::string threadName) {
	std::chrono::duration<double> elapsedTime = end - start;
	//TODO NADA POR AHORA 
	std::cout << "Thread" << threadName << "- elapsed time:" << elapsedTime.count() << "seconds" << std::endl;
}
void Count(unsigned long long maxCounter, std::string threadName) {
	for (unsigned long long i = 0; i < maxCounter; i++) {

	}

	std::chrono::system_clock::time_point endDate = std::chrono::system_clock::now();
	PrintElapsedTime(startDate, endDate, threadName);
}

void Example01() {
	unsigned long long totalCount = 1000000000ull;


	std::thread* Counter1 = new std::thread(Count, totalCount, "1");
	std::thread* Counter2 = new std::thread(Count, totalCount, "2");
	std::thread* Counter3 = new std::thread(Count, totalCount, "3");
	// Solo se detiene si todos los threatsse estan ejecutando y al menos uno de ellos se esta ejecutando con un join


	Counter1->join();
	Counter2->join();
	Counter3->join();



	Count(totalCount, "0");
}


void Example02() 
{
	unsigned long long totalCount = 1000000000ull;


	std::thread* Counter1 = new std::thread(Count, totalCount, "1");
	Counter1->join();


	std::thread* Counter2 = new std::thread(Count, totalCount, "2");
	std::thread* Counter3 = new std::thread(Count, totalCount, "3");
	// Solo se detiene si todos los threatsse estan ejecutando y al menos uno de ellos se esta ejecutando con un join


	Counter2->join();
	Counter3->join();



	Count(totalCount, "0");
}

void Example03() {
	unsigned long long totalCount = 1000000000ull;


	std::thread* Counter1 = new std::thread(Count, totalCount, "1");
	std::thread* Counter2 = new std::thread(Count, totalCount, "2");
	std::thread* Counter3 = new std::thread(Count, totalCount, "3");
	// Solo se detiene si todos los threatsse estan ejecutando y al menos uno de ellos se esta ejecutando con un join


	Counter1->detach();
	Counter2->detach();
	Counter3->detach();  
	//cambiando el join por el detach, hace que el codigo no pare. 



	Count(totalCount, "0");
}
void Example04() {
	std::list<std::thread*>* threads = new std::list<std::thread*>();
	unsigned long long totalCount = 1000000000ull;
	for (int i = 0; i < 3; i++) {
		std::thread* counterThread = new std::thread(Count, totalCount, std::to_string(i + 1));
		threads->push_back(counterThread);

	}

	Count(totalCount, "0");

}

void Example05() {
	std::list<std::thread*>* threads = new std::list<std::thread*>();
	unsigned long long totalCount = 1000ull * 1000ull * 1000ull;

	for (int i = 0; i < 3; i++) {
		std::thread* counterThread = new std::thread(Count, totalCount, std::to_string(i + 1));
		threads->push_back(counterThread);
		counterThread->detach();
	}
	Count(totalCount, "0");
}

void Example06() {
	unsigned int maxThreads = std::thread::hardware_concurrency();
	std::cout << maxThreads << " concurrent threads are supported." << std::endl;

	int extraThreads = 12;
	std::list<std::thread*>* threads = new std::list<std::thread*>();
	unsigned long long totalCount = 1000ull * 1000ull * 1000ull;

	for (int i = 0; i < maxThreads + extraThreads; ++i) {
		std::thread* counterThread = new std::thread(Count, totalCount, std::to_string(i + 1));
		threads->push_back(counterThread);

	}

	for (std::thread* t : *threads) {
		t->join();
	}

	Count(totalCount, "0");
}

class ThreadTester {
public:
	ThreadTester(){}
	~ThreadTester(){}

	void Example07() {
		std::thread* testThread = new std::thread(ThreadTester::TestFunction,this, 14ull);
		testThread->join();
	}
private:
	void TestFunction(unsigned long long testNum) {
		consoleMutex.lock();
		std::cout << "Number is: " << testNum << std::endl;
		consoleMutex.unlock();

	}

};
void Example07() {
	ThreadTester* threadTester = new ThreadTester();
	threadTester->Example07();

	ThreadTester threadTester2 = ThreadTester();
	threadTester2.Example07();

}
