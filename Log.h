#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <fstream>

class Log {
public:
	Log(std::string filename, char sep = ' ');
	~Log();
	void println(long time, double angle);
	void println(long time, double angle, double pidOutput);
	void println(long time, double sp, double csp, double spd);
	void println(long time, double sp, double csp, long enc, long filtered);
	void close();
private:
	std::ofstream file;
	char sep;
};

#endif
