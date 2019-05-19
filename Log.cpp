#include "Log.h"

using namespace std;
using namespace myRIO;

/**
 * A simple constructor for a Log.
 * Path of the resulting file: /home/filename
 * @param filename The filename to give. You can pass an extension if you want.
 * @param sep The separator between each data on a line
 */
Log::Log(std::string filename, char sep) : sep(sep) {
	file.open("/home/"+filename);
}

void Log::println(long time, double sp) {
	file << time << sep << sp << std::endl;
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
