#include "Log.h"

using namespace std;

Log::Log(std::string filename, char sep) : sep(sep) {
	file.open("/home/"+filename);
}

void Log::println(long time, double sp, double csp) {
	file << time << sep << sp << sep << csp << std::endl;
}

void Log::println(long time, double sp, double csp, double spd) {
	file << time << sep << sp << sep << csp << sep << spd << std::endl;
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
