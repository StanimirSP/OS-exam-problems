#include <fstream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <cstddef>
using namespace std;

int main(int argc, char** argv)
{
	long n = argc > 1? strtol(argv[1], NULL, 10): 50;
	ofstream of("file.bin", ios::binary);
	mt19937 gen(time(nullptr));
	uniform_int_distribution <unsigned long> dist(0, numeric_limits<unsigned long>::max());
	for(uint32_t i=0; i<n; i++)
	{
		uint32_t t = dist(gen);
		of.write(reinterpret_cast<const char*>(&t), sizeof t);
	}
}
