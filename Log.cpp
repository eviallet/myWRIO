#include "Log.h"

using namespace std;

Log::Log(std::string filename, char sep) : sep(sep) {
	file.open("/home/"+filename);
}

void Log::println(long time, int dat1, long dat2) {
	file << time << sep << dat1 << sep << dat2 << std::endl;
}

void Log::close() {
	file.close();
}

Log::~Log() {
	if(file.is_open())
		file.close();
}
