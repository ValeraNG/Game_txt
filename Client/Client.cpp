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

#pragma comment(lib, "WS2_32.lib")

const int WIDTH = 100;
const int HEIGHT = 30;

class Window
{
public:
	static Window* getInstance(size_t width = 0, size_t	height = 0)
	{
		static Window instance(width, height);
		return &instance;
	}

	static void display()
	{
		getInstance()->display_l();
	}
	static void clear(char c = ' ')
	{
		getInstance()->clear_l(c);
	}
	static void draw(std::vector<char>& data)
	{
		for (size_t i = 0; i < data.size(); ++i)
		{
			getInstance()->screen[i] = data[i];
		}
	}

	~Window()
	{
		delete[] screen;
	}

private:
	Window(const size_t width, const size_t height) :width(width), height(height)
	{
		_SMALL_RECT Rect = _SMALL_RECT(0, 0, width - 1, height - 1);
		HANDLE		Handle = GetStdHandle(STD_OUTPUT_HANDLE);
		_COORD		Coord = _COORD(width, height);
		SetConsoleScreenBufferSize(Handle, Coord);
		SetConsoleWindowInfo(Handle, TRUE, &Rect);

		hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		screen = new wchar_t[width * height];
		dwBytesWritten = NULL;
		SetConsoleActiveScreenBuffer(hConsole);

		clear_l();
	}

	void display_l()
	{
		WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);
	}
	void clear_l(char c = ' ')
	{
		for (size_t i = 0; i < width * height; ++i) screen[i] = c;
	}

private:
	DWORD  dwBytesWritten;
	HANDLE hConsole;

public:
	wchar_t* screen{ nullptr };

	const size_t height;
	const size_t width;
};



class Client
{
public:
	Client()
	{
		if (WSAStartup(MAKEWORD(2, 2), &wData) == 0)
		{
			printf("WSA Startup succes\n");
		}

		SOCKADDR_IN addr;
		addr.sin_addr.s_addr = inet_addr("127.0.0.1");//"127.0.0.1");// "192.168.200.178");//"192.168.56.1");//"127.0.0.1");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(3000);

		serverSocket = socket(AF_INET, SOCK_STREAM, NULL);

		if (connect(serverSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			printf("No connect!\n");
		}

		std::thread([this]() { send_messages(serverSocket); }).detach();
	}

	void receive_messages()
	{
		int msg_size = WIDTH * HEIGHT;
		//recv(serverSocket, (char*)&msg_size, sizeof(int), NULL);////////////////////
		std::vector<char> screen(msg_size);
		recv(
			serverSocket,
			(std::remove_const_t<const char>*)screen.data(),
			msg_size,
			NULL
		);

		Window::clear();
		Window::draw(screen);
	}

	void send_messages(SOCKET serverSocket)
	{
		while (true)
		{
			std::string massage;

			if (GetAsyncKeyState((unsigned short)'A') & 0x8000) massage.push_back('A');
			if (GetAsyncKeyState((unsigned short)'D') & 0x8000) massage.push_back('D');
			if (GetAsyncKeyState((unsigned short)'W') & 0x8000) massage.push_back('W');
			if (GetAsyncKeyState((unsigned short)'S') & 0x8000) massage.push_back('S');
			if (GetAsyncKeyState((unsigned short)' ') & 0x8000) massage.push_back(' ');
			if (GetAsyncKeyState((unsigned short)'R') & 0x8000) massage.push_back('R');
			if (GetAsyncKeyState((unsigned short)'Q') & 0x8000) massage.push_back('Q');

			if (!massage.empty())
			{
				int msg_size = massage.size();
				send(serverSocket, (char*)&msg_size, sizeof(int), NULL);
				send(serverSocket, massage.c_str() , msg_size   , NULL);
			}
		}
	}

private:
	SOCKET  serverSocket;
	WSAData	wData;
};



int main()
{
	Window::getInstance(100, 30);

	Client client;

	while (true)
	{
		client.receive_messages();
		Window::display();
	}
}