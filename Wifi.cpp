#include "Wifi.h"

using namespace myRIO;

// https://www.geeksforgeeks.org/socket-programming-cc/
// http://manpages.ubuntu.com/manpages/cosmic/en/man2/accept.2.html
Wifi::Wifi(std::function<void(short setpoint)> func) : connected(false), _socket(0), _func(func) {
	_socketThread = std::thread([&]() {
			openServer();
		}
	);
	_socketThread.detach();
}

void Wifi::openServer() {
	int inBytes;
	short val;
	char buffer[2];
	
	int err, opt = 1;
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd==0) { perror("socket"); return; }
	
	err = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_KEEPALIVE, &opt, sizeof(opt));
	if(err) { perror("setsockopt"); return; }

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(50000);

	err = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(err<0) { perror("bind"); return; }

	err = listen(server_fd, 10);
	if(err<0) { perror("listen"); return; }
	
	do {
		val = 0;

		struct sockaddr client_addr = {0};
		socklen_t client_addr_size = sizeof(client_addr);
		std::cout << "Wifi - Waiting for connection" << std::endl;
		_socket = accept(server_fd, &client_addr, &client_addr_size);
		if(_socket==-1) { perror("accept"); return; }

		std::cout << "Wifi - Connected" << std::endl;
		connected = true;

		while(val != 30000) {
			ioctl(_socket, FIONREAD, &inBytes);

			if(inBytes>0) {
				read(_socket , buffer, 2);
				val = buffer[1]<<8 | buffer[0];
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

void Wifi::updateAngle(short angle) {
	if(connected)
		writeShort(angle);
}

void Wifi::updateEncoder(uint8_t setpoint, uint32_t enc) {
	if(connected) {
		writeChar('#');
		writeShort(setpoint);
		writeLong(enc);
	}
}

bool Wifi::isConnected() {
	return connected;
}

void Wifi::writeChar(char c) {
	write(_socket, reinterpret_cast<const char*>(&c), sizeof(char));
}

void Wifi::writeShort(short s) {
	write(_socket, reinterpret_cast<const char*>(&s), sizeof(short));
}

void Wifi::writeLong(long l) {
	write(_socket, reinterpret_cast<const char*>(&l), sizeof(long));
}

Wifi::~Wifi() {
	if(connected) {
		writeShort(30000);
		close(_socket);
	}
}
