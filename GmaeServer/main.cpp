#include "Server.h"
#include "Window.h"
#include "Characters.h"



class Controler
{
public:
	void set_control(GameObject* new_object_ptr)
	{
		object_ptr = new_object_ptr;
	}
	void evant_handler()
	{
		Direction dx = STOP;
		Direction dy = STOP;

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) dx =  LEFT;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) dx = RIGHT;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) dy =	UP;
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) dy =  DOWN;

		if (GetAsyncKeyState((unsigned short)' ') & 0x8000)
		{
			if (auto mg = dynamic_cast<Weapon<Bullet>*>(object_ptr->components["machineGan"])) mg->fire();
		}
		if (GetAsyncKeyState((unsigned short)'R') & 0x8000)
		{
			if (auto rg = dynamic_cast<Weapon<Rocket>*>(object_ptr->components["rocketGan"])) rg->fire();
		}

		object_ptr->speed.dir = { dx, dy };
	}

protected:
	
	GameObject* object_ptr;
};


void handler_server(Server& server)
{
	std::scoped_lock lk(server.mutex);

	auto it = GameObject::list_object.begin();

	for (int curent_user = 0; auto& user : server.connections)
	{
		auto player = *std::next(it, curent_user);

		user.mutex.lock();
		std::string comand = user.comand;
		user.comand = "";
		user.mutex.unlock();

		Direction dx = STOP;
		Direction dy = STOP;

		for (char c : comand)
		{
			if (c == 'A') dx = LEFT;
			if (c == 'D') dx = RIGHT;
			if (c == 'W') dy = UP;
			if (c == 'S') dy = DOWN;
			if (c == ' ')
			{
				if (auto mg = dynamic_cast<Weapon<Bullet>*>(player->components["machineGan"])) mg->fire();
			}
			if (c == 'R')
			{
				if (auto rg = dynamic_cast<Weapon<Rocket>*>(player->components["rocketGan"])) rg->fire();
			}
			if (c == 'Q')
			{
				if (auto lg = dynamic_cast<Weapon<Laser>*>(player->components["laserGan"])) lg->fire();
			}
		}

		player->speed.dir = { dx, dy };

		++curent_user;
	}
}


//				cycle
//----------------------------------------//

void create_enemy()
{
	static float temp = 0;
	temp += 1 * PlayTime::elapsedTime;

	if (temp < 1) return;
	temp = 0;

	auto enemy = new SpaceShipEnemy(random(0, 1) ? "enemy_1" : "enemy_2");
	enemy->posithion = { WIDTH, (float)random(0, HEIGHT - 3) };
	enemy->speed = { (float)random(5, 10), 5, LEFT, STOP };
}

void update	  ()
{
	auto it = begin(GameObject::list_object);

	while (it != end(GameObject::list_object))
	{
		auto ptr_obj = *it;

		if (ptr_obj->check_death())
		{
			ptr_obj->death();
			it = GameObject::list_object.erase(it);
			delete ptr_obj;
		}
		else
		{
			ptr_obj->update();
			++it;
		}
	}

	//auto& v = GameObject::list_object;
	//auto it = std::remove_if(v.begin(), v.end(), [](auto& element) { return element->check_death(); });
	//std::for_each(it, v.end(), [](auto& element) { element->death(); /*delete element;*/ });
	//v.erase(it, v.end());
	//for (auto obj_ptr : GameObject::list_object)
	//{
	//	obj_ptr->update();
	//}

	create_enemy();
}
void collision()
{
	for (auto obj_ptr : GameObject::list_object) obj_ptr->collision();
}
void render	  ()
{
	for (auto object_ptr : GameObject::list_object)
	{
		object_ptr->render();
	}
}
//----------------------------------------//



int main()
{
	Image::load_local();

	Server server;

	server.start();

	{
		auto main_ptr = new SpaceShip;
		main_ptr->posithion = { 10, 10 };
	}
	{
		auto main_ptr = new SpaceShip;
		main_ptr->posithion = { 10, 20 };
	}

	float temp = 0;

	while (true)
	{
		PlayTime::update();

		handler_server(server);

		update();
		collision();
		render();
	}

	server.close();
}

//int main()
//{
//	Image::load_local();
//
//	auto main_ptr = new SpaceShip;
//	main_ptr->posithion = { 10, 10 };
//
//	//-------------------------------------------
//	{
//		auto ptr = new SpaceShipEnemy("enemy_2");
//		ptr->posithion = { WIDTH, 10 };
//		ptr->speed = { 5, 0, LEFT, STOP };
//	}
//
//	Controler control;
//	control.set_control(main_ptr);
//	
//	while (true)
//	{
//		PlayTime::update();
//
//		control.evant_handler();
//
//		update	 ();
//		collision();
//		render	 ();
//	}
//}