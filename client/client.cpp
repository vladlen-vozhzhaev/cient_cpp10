#pragma comment(lib, "ws2_32.lib") // Этот код используется для автоматичкской связи с библиотекой ws2_32.lib
#define _WINSOCK_DEPRECATED_NO_WARNINGS // Отключение предупреждений
#include <winsock2.h> // Включение файла winsock2.h для работы с функциями Winsock
#include <iostream>
#include <string>
SOCKET Connection;

void recvMessage() {
	int msg_size; // Размер сообщения
	char* msg;
	while (true){
		recv(Connection, (char*)&msg_size, sizeof(int), NULL); // Получаем размер сообщения
		msg = new char[msg_size + 1]; // Выделяем память под сообщение
		msg[msg_size] = '\0'; // Устанавливаем завершающий нуль
		recv(Connection, msg, msg_size, NULL); // Получаем сообщение
		std::cout << msg << std::endl;
		delete[] msg; // Освобождение памяти
	}
}

int main() {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) { // Инициализируем Winsock
		std::cout << "WSA ERROR" << std::endl;
	}
	else {
		std::cout << "Client started!" << std::endl;
	}

	SOCKADDR_IN addr; // Адрес
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP_адрес к которому подключаетесь
	addr.sin_port = htons(9123); // Порт к которому вы подключаетесь
	addr.sin_family = AF_INET; // Семейство сети, в данном случае IPv4

	Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
		std::cout << "ERROR: failed connect to server\n";
		return 1;
	}
	std::cout << "Connected!\n";
	int msg_size; // Размер сообщения
	recv(Connection, (char*)&msg_size, sizeof(int), NULL); // Получаем размер сообщения
	char* msg = new char[msg_size + 1]; // Выделяем память под сообщение
	msg[msg_size] = '\0'; // Устанавливаем завершающий нуль
	recv(Connection, msg, msg_size, NULL); // Получаем сообщение "Hello world"
	std::cout << msg << std::endl;
	delete[] msg; // Освобождение памяти
	std::string userMessage;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recvMessage, NULL, NULL, NULL);
	while (true){
		std::getline(std::cin, userMessage); // Ожидаем ввод сообщения от пользователя
		msg_size = userMessage.size();
		send(Connection, (char*)&msg_size, sizeof(int), NULL);
		send(Connection, userMessage.c_str(), msg_size, NULL);
	}
}