#include "Log.h"

using namespace std;

Log::Log(std::string filename, char sep) : sep(sep) {
	file.open("/home/"+filename);
}

void Log::println(long time, double sp, double csp, long enc) {
	file << time << sep << sp << sep << csp << sep << enc << std::endl;
}

void Log::println(long time, double sp, double csp, long enc, long filtered) {
	file << time << sep << sp << sep << csp << sep << enc << sep << filtered << std::endl;
}

void Log::close() {
	file.close();
}

Log::~Log() {
	if(file.is_open())
		file.close();
}
