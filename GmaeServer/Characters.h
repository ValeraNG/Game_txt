#pragma once
#include "GameObject.h"



class Bullet : public GameObject
{
public:
	Bullet(Identifier identifier) : parent(parent)
	{
		main_sprite = { "bullet" };
		collider	= { "bullet" };
		speed		= { 20, 0 };
	}

protected:
	void __collision(GameObject* other) override
	{
		if (parent == other->identifier) return;
		if (!collider.collision(posithion, other->collider, other->posithion)) return;

		other->life.damage(2);
		life.death();
	}

	Identifier parent;
};








//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
class Explosion : public GameObject {
public:
	Explosion(Direction dx, Direction dy)
	{
		main_sprite = { "explosion_2", 1 };
		collider    = { "explosion_2" };
		speed       = { 4, 0, dx, dy };

		components["explosion_anim_frame"] = new Handler{
			&main_sprite,
			AnimFrame {
				{
					{ 0,  2, 5,  3 },
					{ 5,  1, 9,  5 },
					{ 14, 0, 14, 7 },
				},
				6.f
			}
		};
	}

	void __update() override
	{
		auto [sprite_ptr, anim_frame] = static_cast<Handler<Sprite*, AnimFrame>*>(components["explosion_anim_frame"])->param;

		end = anim_frame.end_anim();

		collider.react = main_sprite.react = anim_frame.get_react();
		
		auto [w, h] = sprite_ptr->size();
		main_sprite.posithion.y = -h / 2;
		collider   .posithion.y = -h / 2;
	}

	bool check_death() override
	{
		return end;
	}

protected:
	void __collision(GameObject* other) override
	{
		if (!collider.collision(posithion, other->collider, other->posithion)) return;

		other->life.damage(10);
	}

	bool end = false;
};



class Laser : public GameObject
{
public:
	Laser(GameObject* parent) : parent(parent)
	{
		main_sprite = { "laser" };
		collider    = { "laser" };

		components["laser_anim_frame"] = new Handler{
			&main_sprite,
			AnimFrame {
				{
					{ 1,  0, WIDTH, 1 },
					{ 0,  0, WIDTH, 1 }
				},
				5.f
			}
		};
	}

	void __update() override
	{
		auto [sprite_ptr, anim_frame] = static_cast<Handler<Sprite*, AnimFrame>*>(components["laser_anim_frame"])->param;

		int width_parent = 5;
		int height_parent = 2;
		auto [xp, yp] = parent->posithion;

		main_sprite.react = collider.react = anim_frame.get_react();
		main_sprite.react.w = collider.react.w = WIDTH - xp - width_parent;

		timer += PlayTime::elapsedTime * 0.2;

		posithion = { parent->posithion, 5, 1 };
	}

	bool check_death() override
	{
		if (parent->life.check_death()) return true;
		if (timer < 1) return false;
		return true;
	}

protected:
	void __collision(GameObject* other) override
	{
		if (!collider.collision(posithion, other->collider, other->posithion)) return;

		other->life.damage(10 * PlayTime::elapsedTime);
	}

	float timer = 0;
	GameObject* parent = nullptr;
};
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//














class Rocket : public GameObject
{
public:
	Rocket(Identifier identifier) : parent(identifier)
	{
		fire_sprite = { "fire_anim_l" };
		main_sprite = { "rocket", 1 };
		collider    = { "rocket" };
		speed		= { 10, 0 };

		components["fire_anim_frame"] = new Handler{
			&fire_sprite,
			AnimFrame {
				{
					{ 0, 0, 1, 1 },
					{ 1, 0, 1, 1 }
				},
				5.f
			}
		};

		fire_sprite.posithion = { -1 ,0 };
	}

	void death() override
	{
		auto [x, y] = posithion;
		auto [dx, dy] = speed.dir;

		auto explosion = new Explosion(dx, STOP);
		explosion->posithion = { (float)x, (float)y };
	}

protected:
	void __collision(GameObject* other) override
	{
		if (parent == other->identifier) return;
		if (!collider.collision(posithion, other->collider, other->posithion)) return;

		other->life.damage(10);
		life.death();
	}
	void __render() override
	{
		Window::draw(fire_sprite, posithion);
	}

	Sprite	   fire_sprite;
	float	   timer = 5;
	Identifier parent;
};





class SpaceShip : public GameObject
{
public:
	SpaceShip()
	{
		main_sprite = { "spaceship_1", 10 };
		fire_sprite = { "fire_anim_l", 10 };
		collider	= { "spaceship_1" };
		speed		= { 10, 10 };
		life		= { 10 };

		components["fire_anim_frame"] = new Handler{
			&fire_sprite,
			AnimFrame {
				{
					{ 0, 0, 1, 1 },
					{ 1, 0, 1, 1 }
				},
				5.f
			}
		};
		components["machineGan"		] = new Weapon<Bullet>(this, 5);
		components["rocketGan"		] = new Weapon<Rocket>(this, 0.2);
		components["laserGan"		] = new Weapon<Laser> (this, 0.1);

		fire_sprite.posithion = { -1 ,1 };
	}

	bool check_death() override
	{
		return false;
	}

protected:
	void __collision(GameObject* other) override
	{

	}
	void __render() override
	{
		Window::draw(fire_sprite, posithion);
	}

	Sprite fire_sprite;
};










//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//

class SpaceShipEnemy : public GameObject
{
public:
	SpaceShipEnemy(std::string name_image)
	{
		main_sprite = { name_image, 10 };
		fire_sprite = { "fire_anim_r", 10 };
		collider	= { name_image };
		speed		= { 10, 10 };
		life		= { 10 };

		components["fire_anim_frame"] = new Handler{
			&fire_sprite,
			AnimFrame {
				{
					{ 0, 0, 1, 1 },
					{ 1, 0, 1, 1 }
				},
				5.f
			}
		};

		fire_sprite.posithion = { 6 ,1 };
	}

protected:
	void __collision(GameObject*) override
	{

	}
	void __render() override
	{
		Window::draw(fire_sprite, posithion);
	}

	Sprite fire_sprite;
};

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//