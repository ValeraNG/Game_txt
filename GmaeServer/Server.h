#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream> 
#include <cstdio> 
#include <cstring> 
#include <winsock2.h> 

#include <windows.h>
#include <vector>
#include <mutex>
#include <queue>
#include <list>
#include <algorithm>
#include <chrono>
#include <thread>

#pragma comment(lib, "WS2_32.lib")
#include "Primitives.h"

class Window
{
public:
	static const int  WIDTH = 100;
	static const int  HEIGHT = 30;

	static Window* getInstance()
	{
		static Window instance(WIDTH, HEIGHT);
		return &instance;
	}

	static void clear(char c = ' ')
	{
		getInstance()->clear_l(c);
	}
	static void draw(const auto& sprite, Posithion posithionGlobal = {})
	{
		getInstance()->draw_l(sprite, posithionGlobal);
	}

private:
	Window(const size_t width, const size_t height)
	{
		coordinates_Z.resize(width * height);
		buffer.resize(width * height);
	}

	void clear_l(char c = ' ')
	{
		//std::scoped_lock lk(mutex);
		for (size_t i = 0; i < WIDTH * HEIGHT; ++i)
		{
			coordinates_Z[i] = NULL;
			buffer[i] = c;
		}
	}
	void draw_l(const auto& sprite, auto& posithionGlobal)
	{
		auto [w, h] = sprite.size();
		auto [pos_x, pos_y] = sprite.posithion + posithionGlobal;

		//-----проверка на х-----//
		if (max(0, pos_x) >= min(pos_x + w, (int)WIDTH)) return;


		//std::scoped_lock lk(mutex);


		//------size_t->int------//
		for (int i = max(0, (int)pos_y); i < min((int)pos_y + h, (int)HEIGHT); ++i)
		{
			for (int j = max(0, (int)pos_x); j < min((int)pos_x + w, (int)WIDTH); ++j)
			{
				int  index = i * WIDTH + j;
				int  importance = sprite.importance;
				char fragment = sprite.get_fragment(j - (int)pos_x, i - (int)pos_y);

				if (fragment != ' ' && fragment != '`' && importance >= coordinates_Z[index])
				{
					coordinates_Z[index] = importance;
					buffer[index] = fragment;
				}
			}
		}
	}

	std::vector<int> coordinates_Z;
public:
	std::vector<char> buffer;

	std::mutex mutex;
};



struct Client
{
	Client() {}
	Client(SOCKET socket) :
		socket(socket), status("online") {}

	Client& operator = (const Client& other)
	{
		socket = other.socket;
		status = other.status;
		comand = other.comand;

		return *this;
	}

	SOCKET		socket;
	std::string comand;
	std::string	status;
	std::mutex	mutex;
};



class Server
{
public:
	void start()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wData) == 0)
		{
			printf("WSA Startup succes\n");
		}

		SOCKADDR_IN addr;
		addr.sin_addr.s_addr = inet_addr(addr_server.c_str());
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		serverSocket = socket(AF_INET, SOCK_STREAM, NULL);

		if (serverSocket == SOCKET_ERROR)
		{
			printf("Socket not created\n");
		}

		if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR)
		{
			printf("Socket succed binded\n");
		}

		if (listen(serverSocket, SOMAXCONN) != SOCKET_ERROR)
		{
			printf("Start listenin at port %u\n", ntohs(addr.sin_port));
		}

		std::thread([this]() { this->connect_handle();  }).detach();
		std::thread([this]() { this->send_all_client(); }).detach();
	}

	void close()
	{
		closesocket(serverSocket);
		WSACleanup();
		std::cout << "Server was stoped. You can close app\n";
	}

	void send_all_client()
	{
		while (true)
		{
			auto buffer = Window::getInstance()->buffer;
			Window::clear();

			int   msg_size = buffer.size();

			std::scoped_lock lk(mutex);

			auto it = remove_if(connections.begin(), connections.end(), [](auto& elem) {
				std::scoped_lock lk(elem.mutex);
			return elem.status == "offline";
				});
			connections.erase(it, connections.end());

			for (auto& client : connections)
			{
				std::scoped_lock lk(client.mutex);

				//send(client.socket, (char*)&msg_size, sizeof(int), NULL);
				send(client.socket, buffer.data(), msg_size, NULL);
			}

			std::chrono::milliseconds temp(10);
			std::this_thread::sleep_for(temp);
		}
	}

private:
	void connect_handle()
	{
		size_t index = 0;

		SOCKADDR_IN addr;
		int sizeOfAddr = sizeof(addr);

		while (true)
		{
			auto connection = accept(serverSocket, (SOCKADDR*)&addr, &sizeOfAddr);

			std::cout << "Client " << index << " connected!\n";

			std::scoped_lock lk(mutex);
			connections.emplace_back(connection);
			std::thread([this]() { client_events(connections.back()); }).detach();

			++index;
		}
	}
	void client_events(Client& user)
	{
		user.mutex.lock();
		auto socket = user.socket;
		user.mutex.unlock();

		while (true)
		{
			int msg_size = 0;
			recv(socket, (char*)&msg_size, sizeof(int), NULL);////////////////////
			std::string massage(msg_size, '\0');
			recv(
				socket,
				(std::remove_const_t<const char>*)massage.c_str(),
				msg_size,
				NULL
			);

			std::scoped_lock lk(user.mutex);
			user.comand = massage;

			if (massage == "exit")
			{
				user.status = "offline";
				break;
			}
		}
	}

	SOCKET  serverSocket;
	WSAData	wData;

public:
	std::string    addr_server = "127.0.0.1";//"192.168.56.1";   //"192.168.141.231"; // 127.0.0.1";
	unsigned short port = 3000;

	std::list<Client> connections;
	std::mutex		  mutex;
};