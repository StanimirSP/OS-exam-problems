#include <fstream>
#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cstdint>

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <outputFile> [<bytesConut>]\n";
		return 1;
	}
	long n = argc > 2? std::strtol(argv[2], NULL, 0): 64;
	std::ofstream of(argv[1], std::ios::binary);
	if(!of)
	{
		std::cerr << "Could not open " << argv[1] << '\n';
		return 2;
	}
	std::mt19937 gen(std::time(nullptr));
	std::uniform_int_distribution <> dist(0, 255);
	for(long i=0; i<n; i++)
	{
		unsigned char t = dist(gen);
		of.write(reinterpret_cast<const char*>(&t), 1);
	}
}
