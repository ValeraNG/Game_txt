#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>

//		containers
//----------------------//
#include <vector>
#include <list>
#include <map>
//----------------------//


const int WIDTH  = 100;
const int HEIGHT = 30;


inline int random(int a, int b) {
	using namespace std;
	static mt19937 gen(random_device{}());
	uniform_int_distribution dist(a, b);
	return dist(gen);
}

//float get_elapsedTime() {
//	static auto tp1 = std::chrono::system_clock::now();
//	static auto tp2 = std::chrono::system_clock::now();
//	tp2 = std::chrono::system_clock::now();
//	std::chrono::duration<float> elapsedTime = tp2 - tp1;
//	tp1 = tp2;
//	return elapsedTime.count();
//}
//
//inline float elapsedTime = NULL;




class PlayTime
{
public:
	static void update() {
		static auto tp1 = std::chrono::system_clock::now();
		static auto tp2 = std::chrono::system_clock::now();
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed_time = tp2 - tp1;
		tp1 = tp2;
		elapsedTime = elapsed_time.count();
	}

	inline static float elapsedTime = NULL;
};





//			auxiliary structs
//--------------------------------------//
struct Size
{
	int width  = NULL;
	int height = NULL;
};

struct React
{
	int x = NULL;
	int y = NULL;
	int w = NULL;
	int h = NULL;
};

template<typename T>
struct vector2T
{
	T x = (T)NULL;
	T y = (T)NULL;
};
//--------------------------------------//





//				traffic
//--------------------------------------//
enum Direction
{
	STOP  =  0,
	UP	  = -1,
	DOWN  =  1,
	LEFT  = -1,
	RIGHT =  1
};

class Speed
{
public:
	Speed() {}
	Speed(float speed_x, float speed_y) :
		speed{ speed_x, speed_y } {}
	Speed(float speed_x, float speed_y, Direction dx, Direction dy) :
		speed{ speed_x, speed_y }, dir{ dx, dy } {}

	auto get() const
	{
		float speed_x = speed.x * static_cast<float>(dir.x);
		float speed_y = speed.y * static_cast<float>(dir.y);
		return vector2T<float>(speed_x, speed_y);
	}

	vector2T<float>     speed;
	vector2T<Direction> dir;
};



struct Posithion
{
	Posithion() {}
	Posithion(float x, float y) : x(x), y(y) {}
	Posithion(const Posithion& other, int bias_x = 0, int bias_y = 0)
	{
		x = other.x + bias_x;
		y = other.y + bias_y;
	}

	auto get_int()
	{
		return vector2T<int>(x, y);
	}

	void move(const Speed& speed, float elapsedTime)
	{
		auto [speed_x, speed_y] = speed.get();
		move(speed_x * elapsedTime, speed_y * elapsedTime);
	}
	void move(float dx, float dy)
	{
		x += dx;
		y += dy;
	}

	Posithion operator + (const Posithion& other) const
	{
		return Posithion(other, x, y);
	}

	float x = NULL;
	float y = NULL;
};
//--------------------------------------//





class Life
{
public:
	Life() {}
	Life(int size) : life(size) {}

	void death()
	{
		life = NULL;
	}
	bool check_death()
	{
		return (life <= NULL ? true : false);
	}
	void damage(float size)
	{
		life -= size;
	}
	int value()
	{
		return life;
	}

private:
	float life = 1;
};





class Identifier {
	inline static size_t Count = 0;
public:
	Identifier(std::string name, std::string group) :
		id(Count), name(name), group(group) 
	{
		++Count;
	}	
	Identifier(std::string group) :
		id(Count), name("Name: " + std::to_string(id)), group(group)
	{
		++Count;
	}
	Identifier() : 
		id(Count), name("Name: " + std::to_string(id)), group("Group: " + std::to_string(id))
	{
		++Count;
	}

	bool operator == (const Identifier& other)
	{
		return name == other.name && group == other.group && id == other.id;
	}

	size_t get_id()
	{
		return id;
	}

	std::string name;
	std::string group;
private:
	size_t id;
};




class Image
{
public:
	inline static std::string PATH{ "Images.txt" };
	inline static std::map<std::string, Image> images;

	static void load_local(const std::string path = PATH)
	{
		std::ifstream in(path);

		while (!in.eof())
		{
			std::string name;
			std::getline(in, name);
			images[name] = Image(in);
		}

		//WIDTH
		//------------------------------------//
		images["laser"] = Image(100, 1, "~>");
	}

	Image() {}
	//						laser
	//--------------------------------------------------//
	Image(size_t width, size_t height, std::string subsequence)
	{
		buffer.resize(width * height);
		for (size_t i = 0; i < buffer.size(); ++i)
		{
			int size = subsequence.size();
			buffer[i] = subsequence[i % size];
		}
	}
	//--------------------------------------------------//

	auto size()
	{
		return Size
		{
			static_cast<int>(width),
			static_cast<int>(height)
		};
	}
	char& operator [] (size_t i)
	{
		return buffer[i];
	}

private:
	Image(std::ifstream& in)
	{
		using namespace std;

		string		 line;
		list<string> bufferTemp;
		size_t		 maxLength = NULL;

		while (!in.eof())
		{
			getline(in, line);
			if (line == "\0") break;
			bufferTemp.push_back(line);
			maxLength = max(maxLength, line.size());
		}

		width  = maxLength;
		height = bufferTemp.size();
		buffer.resize(width * height);

		for (auto it = begin(buffer); auto & line : bufferTemp)
		{
			line += string(maxLength - line.size(), ' ');
			std::copy(line.begin(), line.end(), it);
			std::advance(it, width);
		}
	}

private:
	size_t			  width = NULL;
	size_t			  height = NULL;
	std::vector<char> buffer;
};