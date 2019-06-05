# myWRIO
## C++ API for National Instruments myRIO

This is a framework to use with myRIO. It allows the user to code on the myRIO without LabVIEW.
It is based on the official C API, but handles registers communications to provide a high level, arduino like programming.

[Documentation found here](https://github.com/eviallet/myWRIO-Docs)

The docs are generated with Doxygen. Each line of code is documented and examples are provided for each class.

* Blink a LED

```cpp
#include "MyRIO.h"
using namespace myRIO;
int main() {
    if(!myRIO_init()) {std::cout << "Error initializing myRIO"; return -1;}
  
    bool status = HIGH;
  
    while(1) {
        status=!status
        DIO::writeLed(LED1, status);
        Time::wait_ms(500);
    }
}
```

* Output a PWM signal

```cpp
#include "MyRIO.h"
using namespace myRIO;
using namespace std;
int main() {
    if(!myRIO_init()) {cout << "Error initializing myRIO"; return -1;}
    // Output a 10kHz, 3.3V PWM signal with a duty cycle of 25% for 10 seconds
    PWM channelC0(PWMC0, 10e3, 25);
    Time::wait_s(10);
}
```

* Generate a sine with AIO class (Analog Input Output)

```cpp
#include "MyRIO.h"
using namespace myRIO;
using namespace std;
int main() {
    if(!myRIO_init()) {cout << "Error initializing myRIO"; return -1;}
    double fq = 100;
    double vpp = 4;
    double outputFq = 100e3;
    Time timer = Time::stopwatch();
    timer.reset();
    while(1) {
        AIO::writePin(CO0, (vpp/2)*sin(2*3.141592653*fq*timer.elapsed_ns()*1e-9));
        // we cannot measure more than 2 seconds ; reset it before it's too late
        if(timer.elapsed_ns() >= 1e9L)
            timer.reset();
        // convert the frequency to time, then the time to µs
        Time::wait_us(1e6/outputFq);
    }
    return 0;
}
```

* 




* LEDs example

[![Youtube video 2](https://img.youtube.com/vi/AXE74Ngltvw/0.jpg)](https://youtu.be/AXE74Ngltvw)

* MotorPID example

[![Youtube video 2](https://img.youtube.com/vi/YSBKbA0pjjg/0.jpg)](https://youtu.be/YSBKbA0pjjg)

* Real time angle supervision (with external I2C gyroscope)

[![Youtube video 2](https://img.youtube.com/vi/nEx_2FulfrY/0.jpg)](https://youtu.be/nEx_2FulfrY)

* And a car

[![Youtube video 2](https://img.youtube.com/vi/CFziAZTmknE/0.jpg)](https://youtu.be/CFziAZTmknE)
