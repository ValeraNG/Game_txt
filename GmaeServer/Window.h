#pragma once
#include <stdio.h>
#include <windows.h>
#include "Primitives.h"



//class WindowServer
//{
//public:
//	static WindowServer* getInstance()
//	{
//		static WindowServer instance(WIDTH, HEIGHT);
//		return &instance;
//	}
//	static void draw(const auto& sprite, Posithion posithionGlobal = {})
//	{
//		getInstance()->draw_l(sprite, posithionGlobal);
//	}
//	static void clear(char c = ' ')
//	{
//		getInstance()->clear_l(c);
//	}
//	static void display()
//	{
//		getInstance()->display_l();
//	}
//
//	~WindowServer()
//	{
//		delete[] screen;
//	}
//
//private:
//	WindowServer(const size_t width, const size_t height) :width(width), height(height)
//	{
//		_SMALL_RECT Rect   = _SMALL_RECT(0, 0, width - 1, height - 1);
//		HANDLE		Handle = GetStdHandle(STD_OUTPUT_HANDLE);
//		_COORD		Coord  = _COORD(width, height);
//		SetConsoleScreenBufferSize(Handle, Coord);
//		SetConsoleWindowInfo(Handle, TRUE, &Rect);
//
//		hConsole       = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
//		screen         = new wchar_t[width * height];
//		dwBytesWritten = NULL;
//		importanceV.resize(width * height, NULL);
//		SetConsoleActiveScreenBuffer(hConsole);
//
//		clear_l();
//	}
//
//	//									change							p.s.: posTest
//	//-----------------------------------------------------------------------------//
//	void draw_l(const auto& sprite, auto& posithionGlobal)
//	{
//		auto [w, h]         = sprite.size();
//		auto [pos_x, pos_y] = sprite.posithion + posithionGlobal;
//
//		//-----проверка на х-----//
//		if (max(0, pos_x) >= min(pos_x + w, (int)width)) return;
//
//		//------size_t->int------//
//		for (int i = max(0, (int)pos_y); i < min((int)pos_y + h, (int)height); ++i)
//		{
//			for (int j = max(0, (int)pos_x); j < min((int)pos_x + w, (int)width); ++j)
//			{
//				int  index = i * width + j;
//				int  importance = sprite.importance;
//				char fragment   = sprite.get_fragment(j - (int)pos_x, i - (int)pos_y);
//
//				if (fragment != ' ' && fragment != '`' && importance >= importanceV[index])
//				{
//					importanceV[index] = importance;
//					screen	   [index] = fragment;
//				}
//			}
//		}
//	}
//	//-----------------------------------------------------------------------------//
//	void clear_l(char c = ' ')
//	{
//		for (size_t i = 0; i < width * height; ++i)
//		{
//			screen     [i] = c;
//			importanceV[i] = NULL;
//		}
//	}
//	void display_l()
//	{
//		WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);
//	}
//
//	wchar_t*		 screen{ nullptr };
//	DWORD			 dwBytesWritten;
//	std::vector<int> importanceV;
//	HANDLE			 hConsole;
//	const size_t	 height;
//	const size_t	 width;
//};