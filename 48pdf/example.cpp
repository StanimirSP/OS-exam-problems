#include <random>
#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{
	std::mt19937 gen(time(nullptr));
	std::uniform_int_distribution<> d1(0,2), d2(1,9);
	int ret = d1(gen), sl = d2(gen);
	std::cout << "The program will sleep for " << sl << "s and will return " << ret << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(sl));
	return ret;
}

