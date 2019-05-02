#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>

class Log {
public:
	Log(std::string filename, char sep = ' ');
	~Log();
	void println(long time, int dat1, long dat2);
	void close();
private:
	std::ofstream file;
	char sep;
};

#endif
