
//
// Disclamer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// In order to load the resources like cute_image.png, you have to set up
// your target scheme :
//
// - Select "Edit Scheme…" in the "Product" menu;
// - Check the box "use custom working directory";
// - Fill the text field with the folder path containing your resources;
//        (e.g. your project folder)
// - Click OK.
//

/*  Game: League of Legends: Tower Defense
    
    Something is wrong on Summoners Rift.  Your turrets have stopped working and 
    your Nexus isn't spawning creeps.  Call upon the powers of your league mates to stop
    the Purple side from destroying your nexus.
 */
 
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <deque>
#include <iostream>
#include <math.h>

using namespace sf;
using namespace std;

//classes
class minion {
public:
    float range, moveSpd;
    float atkSpd, health, maxHP;
    Sprite sprite;
	bool spawned;
    //functions
    minion();
    minion(string a, string b);
};

class champion {
public:
    //skill
    //price
    //upgrades
	float range;
    Sprite icon;
	CircleShape rangeArea;
	bool clicked;
	champion();
	champion(string a);
};

class skill {
public:
    //effect
    //cooldown
    //damage
    //target
    Sprite icon;
};

//Globals
deque<minion> minionPool;
deque<champion> champPool;
vector<Sprite> champSpots;
sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
Texture blueM;
Texture purpleM;
Texture blueR;
Texture purpleR;

//champions
Texture ashe;
Texture nasus;
Texture taric;

//stuff
bool champion_selected = false;

//Methods
void wait(float seconds);
void wave1();
void placeSpots(int posX, int posY, int spotN);

int main(int argc, char const** argv)
{
	//minions icons
    if (!blueM.loadFromFile("blue_melee.png")) {
        return EXIT_FAILURE;
    }
    if (!purpleM.loadFromFile("purple_melee.png")) {
        return EXIT_FAILURE;
    }
    if (!blueR.loadFromFile("blue_ranged.png")) {
        return EXIT_FAILURE;
    }
    if (!purpleR.loadFromFile("purple_ranged.png")) {
        return EXIT_FAILURE;
    }

	//champion icons
	if (!ashe.loadFromFile("ashe.png")) {
		return EXIT_FAILURE;
	}
	if (!nasus.loadFromFile("nasus.png")) {
		return EXIT_FAILURE;
	}
	if (!taric.loadFromFile("taric.png")) {
		return EXIT_FAILURE;
	}

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile("icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }
    sf::Text text("Hello SFML", font, 50);
    text.setColor(sf::Color::Black);
    
    //Forest - setup
    Sprite botF;
    Sprite topF;
    Texture forest;
    if (!forest.loadFromFile("forest.png")) {
        return EXIT_FAILURE;
    }
    botF.setTexture(forest);
    botF.setOrigin(0, 250);
    botF.setPosition(0, 600);
    topF.setTexture(forest);
    topF.setPosition(0, 0);
    
    //Road - setup
    Sprite path;
    Texture road;
    if (!road.loadFromFile("road.png")) {
        return EXIT_FAILURE;
    }
    path.setTexture(road);
    path.setOrigin(0, 50);
    path.setPosition(0, 300);
    path.setScale(2, 2);
    
    //River - setup
    Sprite topR;
    Texture river;
    if (!river.loadFromFile("river.png")) {
        return EXIT_FAILURE;
    }
    topR.setTexture(river);
    topR.setOrigin(100, 0);
    topR.setPosition(400, 0);
    
    //turret - setup
    Sprite blueT;
    Texture bT;
    if (!bT.loadFromFile("bTurret.png")) {
        return EXIT_FAILURE;
    }
    blueT.setTexture(bT);
    blueT.setOrigin(80, 108);
    blueT.setPosition(80, 280);
    blueT.setScale(.5, .5);
    Sprite purpleT;
    Texture pT;
    if (!pT.loadFromFile("pTurret.png")) {
        return EXIT_FAILURE;
    }
    purpleT.setTexture(pT);
    purpleT.setOrigin(80, 108);
    purpleT.setPosition(720, 280);
    purpleT.setScale(.5, .5);
    
    //minionPool[0].sprite.setScale(.15, .15);
    
    //start minions button
	Sprite startWave;
	Texture b_pressed;
	Texture b_unpressed;
	if (!b_pressed.loadFromFile("red_button_pressed.png")) {
		return EXIT_FAILURE;
	}
	if (!b_unpressed.loadFromFile("red_button_unpressed.png")) {
		return EXIT_FAILURE;
	}
	startWave.setTexture(b_unpressed);
	startWave.setScale(.5, .5);
	startWave.setOrigin(83, 83);
	startWave.setPosition(400, 450);
    
	champPool.push_back(champion("ashe"));
	champPool.push_back(champion("nasus"));
	champPool.push_back(champion("taric"));
    
	//Champion Spots
	Sprite c_spot;
	Texture spot;
	if (!spot.loadFromFile("spot.png")) {
		return EXIT_FAILURE;
	}
	c_spot.setTexture(spot);
	c_spot.setScale(.5, .5);

    // Start the game loop
    while (window.isOpen())
    {
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed : exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
			//spawning minions
            if (Mouse::isButtonPressed(Mouse::Left) &&
				Mouse::getPosition(window).x >= startWave.getPosition().x - 41.5 &&
				Mouse::getPosition(window).y >= startWave.getPosition().y - 41.5 &&
				Mouse::getPosition(window).x <= startWave.getPosition().x + 41.5 &&
				Mouse::getPosition(window).y <= startWave.getPosition().y + 41.5 &&
				champion_selected != true) {
				
				startWave.setTexture(b_pressed);
				minionPool.push_back(minion("Purple", "Melee"));
				for (int i = 0; i < minionPool.size(); i++) {
					minionPool[i].spawned = true;
				}
            }
			if (event.type == Event::MouseButtonReleased && 
				event.mouseButton.button == Mouse::Left &&
				Mouse::getPosition(window).x >= startWave.getPosition().x - 41.5 &&
				Mouse::getPosition(window).y >= startWave.getPosition().y - 41.5 &&
				Mouse::getPosition(window).x <= startWave.getPosition().x + 41.5 &&
				Mouse::getPosition(window).y <= startWave.getPosition().y + 41.5) {
				
				startWave.setTexture(b_unpressed);
            }

			//picking up champions
			for (int i = 0; i < champPool.size(); i++) {
				if  (Mouse::isButtonPressed(Mouse::Left) &&
					Mouse::getPosition(window).x >= champPool[i].icon.getPosition().x - 60 &&
					Mouse::getPosition(window).y >= champPool[i].icon.getPosition().y - 60 &&
					Mouse::getPosition(window).x <= champPool[i].icon.getPosition().x + 60 &&
					Mouse::getPosition(window).y <= champPool[i].icon.getPosition().y + 60 &&
					champion_selected != true) {
						champPool[i].clicked = true;
						champion_selected = true;
				}
				for (int i = 0; i < champSpots.size(); i++) {
			
				//snap to spot
					if  (event.type == Event::MouseButtonReleased && 
							event.mouseButton.button == Mouse::Left &&
							Mouse::getPosition(window).x >= champSpots[i].getPosition().x &&
							Mouse::getPosition(window).y >= champSpots[i].getPosition().y &&
							Mouse::getPosition(window).x <= champSpots[i].getPosition().x + 60 &&
							Mouse::getPosition(window).y <= champSpots[i].getPosition().y + 60 &&
							champion_selected != false) {
								for (int j = 0; j < champPool.size(); j++) {
									if (champPool[j].clicked == true) {
										champPool[j].icon.setPosition(champSpots[i].getPosition().x + 30, champSpots[i].getPosition().y + 30);
										champPool[j].rangeArea.setPosition(champSpots[i].getPosition().x + 30, champSpots[i].getPosition().y + 30);
									}
								}
					}
				}
				if  (event.type == Event::MouseButtonReleased && 
					event.mouseButton.button == Mouse::Left &&
					Mouse::getPosition(window).x >= champPool[i].icon.getPosition().x - 30 &&
					Mouse::getPosition(window).y >= champPool[i].icon.getPosition().y - 30 &&
					Mouse::getPosition(window).x <= champPool[i].icon.getPosition().x + 30 &&
					Mouse::getPosition(window).y <= champPool[i].icon.getPosition().y + 30  &&
					champion_selected != false) {
						champPool[i].clicked = false;
						champion_selected = false;
				}
			}
		}

        // Clear screen
        window.clear();
		//cout << startWave.getGlobalBounds().width <<  " ";
		//cout << startWave.getOrigin().x << endl;
        
		//draw the environment
		window.draw(topF);
        window.draw(topR);
        window.draw(botF);
        window.draw(path);
        window.draw(blueT);
        window.draw(purpleT);
        window.draw(startWave);

		
		
		//FPS control
		wait(.0166666666667);

		//draw Champions
		for (int i = 0; i < champPool.size(); i++) {
			//allows you to pick up champions
				if (champPool[i].clicked == true) {
					//champPool[i].icon.setOrigin((champPool[i].icon.getPosition().x - Mouse::getPosition(window).x), (champPool[i].icon.getPosition().y - Mouse::getPosition(window).y));
					champPool[i].icon.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
					champPool[i].rangeArea.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
					
				}
				/*
				//need to check if the minions have entered the range of the current champion.
				for (int j = 0; j < minionPool.size(); j++) {
					if (minionPool[i].spawned == true) {
						//heres the problem
						if (((minionPool[j].sprite.getPosition().x - champPool[i].range)*(minionPool[j].sprite.getPosition().x - champPool[i].range) + (minionPool[j].sprite.getPosition().y - 64 - champPool[i].range)*(minionPool[j].sprite.getPosition().y - 64 - champPool[i].range)) < champPool[i].range^2) {
						minionPool[j].sprite.setScale(2, 2);
					}
					
					minionPool[i].sprite.move(-.7, 0);
					if (minionPool[i].sprite.getPosition().x <= 140) {
						minionPool.pop_front();
					}
					//draw the sprite only if there is a minion to draw
					if (minionPool.size() > 0) {
						window.draw(minionPool[i].sprite);
					}
				}
				}
				*/


				if (Mouse::getPosition(window).x >= champPool[i].icon.getPosition().x - 30 &&
					Mouse::getPosition(window).y >= champPool[i].icon.getPosition().y - 30 &&
					Mouse::getPosition(window).x <= champPool[i].icon.getPosition().x + 30 &&
					Mouse::getPosition(window).y <= champPool[i].icon.getPosition().y + 30) {
						window.draw(champPool[i].rangeArea);
				}
				window.draw(champPool[i].icon);
		}
		//drawing the minions
		for (int i = 0; i < minionPool.size(); i++) {

			//seems silly but its not
			if (minionPool.size() > 0) {
				//moves the minions if they have spawned (spawned by pressing a button)
				if (minionPool[i].spawned == true) {
					minionPool[i].sprite.move(-.7, 0);
					if (minionPool[i].sprite.getPosition().x <= 140) {
						minionPool.pop_front();
					}
					//draw the sprite only if there is a minion to draw
					if (minionPool.size() > 0) {
						window.draw(minionPool[i].sprite);
					}
				}
				
				
			}
		}

		//Champion Board Overlay
		if (champSpots.size() < 9) {
			for (int i = 0; i < 9; i++) {
				champSpots.push_back(c_spot);
			}
			placeSpots(130, 200, 0);
		}
		if (champion_selected == true) {
			for (int i = 0; i < champSpots.size(); i++) {
				window.draw(champSpots[i]);
			}
		}


		//cout << champSpots.size();
        // Update the window
        window.display();
    }

    return EXIT_SUCCESS;
}

minion::minion() {
    
}

//create minions
minion::minion(string a, string b) {
    if (a == "Purple") {
        if (b == "Melee") {
            this->health = 400;
            this->atkSpd = .8;
            this->moveSpd = .7;
            this->range = 25;
            this->sprite.setTexture(purpleM);
            this->sprite.setScale(.3, .3);
            this->sprite.setOrigin(0,64);
            this->sprite.setPosition(864, 300);
        } else if (b == "Ranged") {
            this->health = 280;
			this->atkSpd = .8;
            this->moveSpd = .7;
            this->range = 50;
            this->sprite.setTexture(purpleR);
            this->sprite.setScale(.3, .3);
            this->sprite.setOrigin(0,64);
            this->sprite.setPosition(864, 300);
        }
    }
}

//create champions
champion::champion(string a) {
	if (a == "Ashe" || a == "ashe") {
		this->clicked = false;
		this->icon.setTexture(ashe);
		this->icon.setScale(.5, .5);
		this->icon.setOrigin(60, 60);
		this->icon.setPosition(60, 60);
		this->range = 100;
		this->rangeArea.setOrigin(range, range);
		this->rangeArea.setRadius(range);
		this->rangeArea.setPosition(60, 60);
	}
	if (a == "Nasus" || a == "nasus") {
		this->clicked = false;
		this->icon.setTexture(nasus);
		this->icon.setScale(.5, .5);
		this->icon.setOrigin(60, 60);
		this->icon.setPosition(120, 60);
		this->range = 70;
		this->rangeArea.setOrigin(70, 70);
		this->rangeArea.setRadius(range);
		this->rangeArea.setPosition(120, 60);
	}
	if (a == "Taric" || a == "taric") {
		this->clicked = false;
		this->icon.setTexture(taric);
		this->icon.setScale(.5, .5);
		this->icon.setOrigin(60, 60);
		this->icon.setPosition(180, 60);
		this->range = 70;
		this->rangeArea.setOrigin(70, 70);
		this->rangeArea.setRadius(range);
		this->rangeArea.setPosition(180, 60);
	}

}

//void startMinions() {
//    for (int i = 0; i < minionPool.size(); i++) {
//        wait(.005);
//        minionPool[i].sprite.move(-1, 0);
//        if (minionPool[i].sprite.getPosition().x == 140) {
//            minionPool[i].sprite.setPosition(850, 300);
//        }
//       
//        if (minionPool[i].sprite.getPosition().x == 800 && minionPool.size() < 3) {
//            minionPool.push_back(minion("Purple", "Melee"));
//        } else if (minionPool[i].sprite.getPosition().x == 800 && minionPool.size() < 6) {
//            minionPool.push_back(minion("Purple", "Ranged"));
//        }
//        
//    }
//}

void wait (float seconds)
{
    clock_t endwait;
    endwait = clock() + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait)
    {
        //wait
    }
}

void wave1() {
	minionPool.push_back(minion("Purple", "Melee"));
	minionPool.push_back(minion("Purple", "Melee"));
	minionPool.push_back(minion("Purple", "Melee"));
	minionPool.push_back(minion("Purple", "Ranged"));
	minionPool.push_back(minion("Purple", "Ranged"));
	minionPool.push_back(minion("Purple", "Ranged"));
}

void placeSpots(int posX, int posY, int spotN) {
	if (spotN < 9) {
		champSpots[spotN].setPosition(posX, posY);
		placeSpots(posX+60, posY, spotN+1);
	}
}