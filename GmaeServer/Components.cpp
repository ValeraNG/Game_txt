#include "GameObject.h"
#include "Characters.h"



void Handler<Sprite*, AnimFrame>::update()
{
	auto& [sprite_ptr, animFrame] = param;

	animFrame.update(PlayTime::elapsedTime);
	sprite_ptr->react = animFrame.get_react();
}




void Weapon<Bullet>::__fire()
{
	auto bullet = new Bullet(parent->identifier);
	auto [w, h] = parent->main_sprite.size();

	bullet->posithion = { parent->posithion, w, h / 2 };
	bullet->speed.dir = { RIGHT, STOP };
}





void Weapon<Rocket>::__fire()
{
	auto rocket = new Rocket(parent->identifier);
	auto [w, h] = parent->main_sprite.size(); 

	rocket->posithion = { parent->posithion, w - 3, h / 2 };
	rocket->speed.dir = { RIGHT, STOP };
}



void Weapon<Laser>::__fire()
{
	auto laser = new Laser(parent);
	auto [w, h] = parent->main_sprite.size();

	laser->posithion = { parent->posithion, w, h / 2 };
}