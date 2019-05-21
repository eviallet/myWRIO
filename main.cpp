#include "MyRIO.h"
#include <math.h>

using namespace myRIO;
using namespace std;

int main() {
	if(!myRIO_init()) {cout << "Error initializing myRIO"; return -1;}


	PWM p(PWMA0, 10e3, 40);
	Time::wait_s(10);


	return 0;
}

