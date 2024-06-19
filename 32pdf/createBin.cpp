#include <fstream>
#include <cstdint>
using namespace std;

int main()
{
	ofstream of("f2.bin", ios::binary);
	const uint32_t start = 100000, cnt = 50000;
	for(uint32_t i=start; i<start+cnt; i++)
	of.write(reinterpret_cast<const char*>(&i), sizeof i);
}
