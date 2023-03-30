#pragma once
#include "Server.h"
#include "Objects.h"



//				GameObject
//----------------------------------------//
class GameObject;

class Component {
public:
	virtual void update() = 0;

	virtual ~Component() {};
};



class GameObject {
public:
	//inline static std::vector<GameObject*> list_object;
	inline static std::list<GameObject*> list_object;

	GameObject(Identifier identifier) 
		: identifier(identifier) 
	{
		list_object.push_back(this);
	}
	GameObject() 
		: identifier() 
	{
		list_object.push_back(this);
	}

	void update	  ()
	{
		posithion.move(speed, PlayTime::elapsedTime);

		__update();

		for (auto& [name, component] : components) component->update();
	}
	void collision()
	{
		for (auto object_ptr : GameObject::list_object)
		{
			if (this != object_ptr) __collision(object_ptr);
		}
	}
	void render	  ()
	{
		__render();

		Window::draw(main_sprite, posithion);
	}

	virtual void death		()
	{

	}
	virtual bool check_death()
	{
		auto [x, y] = posithion;
		return x < -10 || WIDTH + 10 < x || life.check_death();
	}

	virtual ~GameObject()
	{
		for (auto& [name, component] : components) delete component;
	}

protected:
	virtual void __collision(GameObject*) = 0;
	virtual void __update() {}
	virtual void __render() {}

public:
	std::map<std::string, Component*> components;
	
	Sprite	   main_sprite;
	Identifier identifier;
	Posithion  posithion;
	Ñollider   collider;
	Speed	   speed;
	Life	   life;
};
//----------------------------------------//





//			Component template
//----------------------------------------//
template <typename ... Ts>
struct Handler : public Component
{
	Handler(Ts&& ... params) : param(std::forward<Ts>(params) ...) {}

	void update() override;

	std::tuple<Ts...> param;
};



template<typename Projectile>
class Weapon : public Component
{
public:
	Weapon(GameObject* parent, float speed = 5) :
		parent(parent), speed(speed) {}

	void update() override
	{
		shot += PlayTime::elapsedTime * speed;
	}
	void fire()
	{
		if (shot < 1) return;

		__fire();
		shot = NULL;
	}

protected:
	void __fire();

	GameObject* parent = nullptr;
	float       shot   = NULL;
	float		speed  = 5;
};
//----------------------------------------//