// SFML.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <SFML\Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "zombieArena.h"
#include "TextureHolder.h"
#include "bullet.h"
#include "Pickup.h"
using namespace sf;


int main()
{
	//here is the instance of texture holder (the one and only)
	TextureHolder holder;


	//game has 4 states

	enum class State
	{
		PAUSED,LEVELING_UP,GAME_OVER,PLAYERING

	};
	//start with gameover

	State state = State::GAME_OVER;

	//screen resolution
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	//create window
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena",Style::Fullscreen);
	//create view
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	//clock for timing everythin
	Clock clock;

	//how long has playing state been active
	Time gameTimeTotal;

	//mouse position in world
	Vector2f mouseWorldPosition;

	//mouse position on screen
	Vector2i mouseScreenPosition;

	//instance of player
	Player player;

	//boundaries of arena
	IntRect arena;

	//create background vertexarray
	VertexArray background;
	//load texture
	//Texture textureBackground;
	//textureBackground.loadFromFile("graphics/background_sheet.png");
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	//prepare for a horde of zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	//100 bullets
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	//when was the fire button last pressed?

	Time lastPressed;

	//hide the mouse pointer and replace it with a crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// About the game
	int score = 0;
	int hiScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver =
		TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon =
		TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	// Load the font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \n to continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);

	gameOverText.setString("Press Enter to play");

	// LEVELING up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	// Load the high-score from a text file
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies remaining
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;

	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 10;

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);
	/*
	**************************************************************************************************************************************************
	**************************                           MAIN GAME LOOP                           ****************************************************
	**************************************************************************************************************************************************
	*/
	while (window.isOpen())
	{

		/*
		**************************************
		*******    Handle Input    ***********
		**************************************
		*/

		//create event
		Event event;
		//handle game events by polling
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				//pause game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYERING)
				{
					state = State::PAUSED;
					
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYERING;
					//reset clock so no frame jump
					clock.restart();

				}

				//start new game while in gameover 
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;
					// Prepare the gun and ammo for next game
					currentBullet = 0;
					bulletSpare = 340;
					bulletsInClip = 30;
					clipSize = 30;
					fireRate = 3;
					// Reset the player's stats
					player.resetPlayerStats();
				}
				if (state == State::PLAYERING)
				{
					//add playing code here
				}

				//reloading
				if (event.key.code == Keyboard::R)
				{
					if (bulletSpare >= clipSize)
					{
						//plenty of bullets. reload.
						bulletsInClip = clipSize;
						bulletSpare -= clipSize;
						reload.play();
					}
					else if (bulletSpare >0)
					{
						//only few bullets left
						bulletsInClip = bulletSpare;
						bulletSpare = 0;
						reload.play();
					}
					else
					{
						//more soon
						reloadFailed.play();
					}
				}

			}

		}//end event pooling

		//handle player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();

		}

		//handle WASD while playing
		if (state == State::PLAYERING)
		{

			//handle Press and Release of WASD
			/******************* W **********/
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			/******************* A **********/
			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			/******************* S **********/
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			/******************* D **********/
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			//fire a bullet
			if (Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate&&bulletsInClip > 0)
				{
					//pass the centre of the player and th centre of the crosshair to shot function
					bullets[currentBullet].shoot(player.getCentre().x, player.getCentre().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet >99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					shoot.play();
					bulletsInClip--;
				}
			}//end FIre Bullet
		}//endWASD

		

		//handle leveling_up
		if (state == State::LEVELING_UP)
		{
			//handle player levellingup
			/*********  Number 1 pressed **********/
			if (event.key.code == Keyboard::Num1)
			{
				// Increase fire rate
				fireRate++;
				state = State::PLAYERING;
			}

			/*********  Number 2 pressed **********/
			if (event.key.code == Keyboard::Num2)
			{
				// Increase clip size
				clipSize += clipSize;
				state = State::PLAYERING;
			}

			/*********  Number 3 pressed **********/
			if (event.key.code == Keyboard::Num3)
			{
				// Increase health
				player.upgradeHealth();
				state = State::PLAYERING;
			}

			/*********  Number4 pressed **********/
			if (event.key.code == Keyboard::Num4)
			{
				// Increase speed
				player.upgradeSpeed();
				state = State::PLAYERING;
			}

			/*********  Number 5 pressed **********/
			if (event.key.code == Keyboard::Num5)
			{
				// Upgrade pickup
				healthPickup.upgrade();
				state = State::PLAYERING;
			}

			/*********  Number 6 pressed **********/
			if (event.key.code == Keyboard::Num6)
			{
				// Upgrade pickup
				ammoPickup.upgrade();
				state = State::PLAYERING;
			}

			if (state ==State::PLAYERING)
			{
				// Increase the wave number
				wave++;

				//prepare level
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;

				//pass vertex array by referance to createbackground function
				int tileSize = createBackground(background, arena);
				//spawn player
				player.spawn(arena, resolution, tileSize);

				// Configure the pickups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				//create a horde zombies
				numZombies = 5 * wave;

				//delete the previously allocated memory(if it exists)
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// Play the powerup sound
				powerup.play();

				//reset clock
				clock.restart();
			}

		}//end of leveling UP


		 /*
		 **************************************
		 *******    Update The Frame    *******
		 **************************************
		 */
		if (state == State::PLAYERING)
		{
			//update delta time
			Time dt = clock.restart();

			//update total game time

			//update total game time
			gameTimeTotal += dt;

			//make a decimal fraction of 1 from delta time
			float dtAsSeconds = dt.asSeconds();

			//where is mouse pointer
			mouseScreenPosition = Mouse::getPosition();
			//convert screen mouse to world mouse
			mouseWorldPosition = window.mapPixelToCoords(mouseScreenPosition, mainView);

			//set the crosshair to the mouse world location 
			spriteCrosshair.setPosition(mouseWorldPosition);

			//update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			//make note of new position
			Vector2f playerPosition(player.getCentre());

			//make view centre around player
			mainView.setCenter(player.getCentre());

			//loop through each zombie and update them
			for (int i = 0; i < numZombies; ++i)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			//update any bullets that are in flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Collision detection
			// Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
					{
						if (bullets[i].GetPosition().intersects
							(zombies[j].getPosition()))
						{
							// Stop the bullet
							bullets[i].stop();
							// Register the hit and see if it was a kill
							if (zombies[j].hit())
							{
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;
								// When all the zombies are dead (again)
								if (numZombiesAlive == 0)
								{
									state = State::LEVELING_UP;
								}
							}
							// Make a splat sound
							splat.play();
						}
					}
				}
			}// End zombie being shot

			 // Have any zombies touched the player
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
					(zombies[i].getPosition()) && zombies[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						// More here later
						hit.play();
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}// End player touched

			 // Has the player touched health pickup
			if (player.getPosition().intersects
				(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				// Play a sound
				pickup.play();
			}
			// Has the player touched ammo pickup
			if (player.getPosition().intersects
				(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletSpare += ammoPickup.gotIt();
				// Play a sound
				reload.play();
			}

			 // size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));

			// Increment the number of frames since the previous update
			framesSinceLastHUDUpdate++;
			// re-calculate every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				// Update the ammo text
				ssAmmo << bulletsInClip << "/" << bulletSpare;
				ammoText.setString(ssAmmo.str());
				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());
				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());
				// Update the high score text
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());
				framesSinceLastHUDUpdate = 0;
			}// End HUD update


		}//end of Update Frame

		 /*
		 **************************************
		 *******    Drawing the scene    ******
		 **************************************
		 */

		if (state == State::PLAYERING)
		{
			window.clear();
			//set mainView to  sisplay
			window.setView(mainView);
			//draw background
			window.draw(background, &textureBackground);
			//draw player
			window.draw(player.getSprite());

			// Draw the pickups, if currently spawned
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}

			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			//draw zombies
			for (int i = 0; i < numZombies; ++i)
			{
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}
			//draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);
			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);

		}//End of drawing the scene
		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}
		window.display();

	}//end of main game loop

	//delete the prevviously allocated memory (if it exists)
	delete[] zombies;

    return 0;
}

