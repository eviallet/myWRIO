#include "Wifi.h"

using namespace myRIO;



/**
* Create a Wifi handler.
* This detach a thread so it can read and write synchronously without blocking other tasks.
* @param func function to call when receiving data
*/
Wifi::Wifi(std::function<void(long setpoint)> func) : connected(false), _socket(0), _func(func) {
	_socketThread = std::thread([&]() {
			openServer();
		}
	);
	_socketThread.detach();
}

/**
* Create a wifi server.
* Ressources used to understand C socket programming : 
* https://www.geeksforgeeks.org/socket-programming-cc/
* http://manpages.ubuntu.com/manpages/cosmic/en/man2/accept.2.html
*/
void Wifi::openServer() {
	int inBytes;
	long val;
	char buffer[2];
	
	// opening a server : AF_INET (IPv4), SOCK_STREAM (TCP)
	int err, opt = 1;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd==0) { perror("socket"); return; }
	
	err = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &opt, sizeof(opt));
	if(err) { perror("setsockopt"); return; }

	// defining server address and port
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(50000);

	// binding the server address
	err = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(err<0) { perror("bind"); return; }

	// listening for incoming connections : blocking
	err = listen(server_fd, 10);
	if(err<0) { perror("listen"); return; }
	
	do {
		val = 0;
		
		struct sockaddr client_addr = {0};
		socklen_t client_addr_size = sizeof(client_addr);
		std::cout << "Wifi - Waiting for connection" << std::endl;
		
		// wait for a client connection
		_socket = accept(server_fd, &client_addr, &client_addr_size);
		if(_socket==-1) { perror("accept"); return; }

		std::cout << "Wifi - Connected" << std::endl;
		connected = true;

		while(val != 30000) {
			ioctl(_socket, FIONREAD, &inBytes);

			if(inBytes>0) {
				read(_socket , buffer, 4);
				val = buffer[3]<<24 | buffer[2]<<16 | buffer[1]<<8 | buffer[0];
				if(val != 30000)
					_func(val);
			}

			//Time::wait_ms(5);
		}
		err = close(_socket);
		if(err<0) { perror("close"); return; }
		std::cout << "Wifi - Socket closed";
		
		connected = false;
		
	} while(true);
}

/**
* Send data to the client
* @param angle the angle to send
*/
void Wifi::updateAngle(short angle) {
	if(connected)
		writeShort(angle);
}

/**
* Returns the state of the socket
* @return true if wifi is connected
*/
bool Wifi::isConnected() {
	return connected;
}

/**
* Write a char to the socket
* @param c a char to write
*/
void Wifi::writeChar(char c) {
	write(_socket, reinterpret_cast<const char*>(&c), sizeof(char));
}

/**
* Write a short to the socket
* @param s a short to write
*/
void Wifi::writeShort(short s) {
	write(_socket, reinterpret_cast<const char*>(&s), sizeof(short));
}

/**
* Write a long to the socket
* @param l a long to write
*/
void Wifi::writeLong(long l) {
	write(_socket, reinterpret_cast<const char*>(&l), sizeof(long));
}

/**
* Close the socket if we are still connected
*/
Wifi::~Wifi() {
	if(connected) {
		writeShort(30000);
		close(_socket);
	}
}
