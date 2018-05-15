#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 1000;

	//player position
	Vector2f m_Position;
	//player sprite
	Sprite m_Sprite;
	//texture
	Texture m_Texture;
	//screen resolution
	Vector2f m_Resolution;
	//arena size
	IntRect m_Arena;
	//tile size
	int m_TileSize;
	
	//direction Player is moving

	bool m_UpPressed;
	bool m_DownPressed;
	bool m_RightPressed;
	bool m_LeftPressed;

	//player health
	int m_Health;

	//max health
	int m_MaxHealth;

	//time player was last hit
	Time m_LastHit;

	//speed in px per second
	float m_Speed;

	//public functions

public:
	Player();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	void resetPlayerStats(); //call at end of every game
	bool hit(Time timeHit); //player getting hit by the zombie
	Time getLastHitTime(); //how long ago was the player last hit

	//player functions
	FloatRect getPosition(); //where is player
	FloatRect setPosition(FloatRect NewPosition);//sets the players location
	Vector2f getCentre(); //centre of player sprite
	float getRotation(); //angle player is facing
	Sprite getSprite(); //send copy of sprite to main

	//move player
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();


	//stop player
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	//updateevery frame
	void update(float elapsedTime, Vector2i mousePosition);

	//speed boost
	void upgradeSpeed();
	//healthBoost
	void upgradeHealth();
	//increase max health
	void increaseHealthLevel(int amount);
	//read current health
	int getHealth();


};