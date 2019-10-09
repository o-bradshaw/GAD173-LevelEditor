#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <list>
using namespace std;

//Setup window size
const int windowWidth = 1025;
const int windowHeight = 650;

//Grid amounts
const int x = 30;
const int y = 20;

//Mouse Pos
sf::Vector2f worldPos;

//Tile class, it's drawable and has a transform
class Tile : public sf::Drawable, public sf::Transformable
{
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(sprite, states);
	}
public:
	bool isGroundTile = false;

	sf::Sprite sprite;
	sf::FloatRect rBounds;

	sf::Texture blockSkyTexture;
	sf::Texture blockPlatformTexture;

	void init(int x, int y)
	{
		//Load textures
		loadTextures();

		//Set the position of this tiles sprite
		sprite.setPosition(x, y);
		ChangeType(false);
	}

	void loadTextures()
	{
		if (!blockSkyTexture.loadFromFile("SpritesPlatformer/BlockSky.png"))
		{
			cout << "Failed to load BlockSky.png";
		}

		if (!blockPlatformTexture.loadFromFile("SpritesPlatformer/BlockPlatform.png"))
		{
			cout << "Failed to load BlockPlatform.png";
		}
	}

	bool mouseOver(sf::Vector2f windPos)
	{
		rBounds = sprite.getGlobalBounds();
		if (rBounds.contains(windPos.x, windPos.y))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void ChangeType(bool isGround)
	{
		isGroundTile = isGround;
		if (isGroundTile)
		{
			sprite.setTexture(blockPlatformTexture);
		}
		else
		{
			sprite.setTexture(blockSkyTexture);
		}
	}
};

//printing out tiles
void PrintToConsole(Tile tile[x][y])
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			if (tile[i][j].isGroundTile)
			{
				cout << "1";
			}
			else
			{
				cout << "0";
			}
			cout << ",";
		}
		cout << "\n";
	}
	cout << "Success \n";
}

int main()
{
	//Setup Window
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Level Editor", sf::Style::Titlebar | sf::Style::Close);

	//Setup Tiles
	Tile tile[x][y];

	//Draw Tiles
	for (int i = 0; i < x; i++) //Rows
	{
		for (int j = 0; j < y; j++) //Columns
		{
			tile[i][j].init(i * 32 + ((windowWidth / 2) - ((32 * x) / 2)), j * 32);
		}
	}

	//Game Loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			window.close();
			break;
			}
		}

		//Prepare the window for displaying stuff
		window.clear(sf::Color::White);

		//Track mouse position
		worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		//Loop through all tiles  to draw and change if clicked
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				//check for click
				if (tile[i][j].mouseOver(worldPos) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					//return the opposite of the current type 
					tile[i][j].ChangeType(true/*!tile[i][j].isGroundTile*/);
				}

				//draw tiles
				window.draw(tile[i][j]);
			}
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			PrintToConsole(tile);
		}

		//Draw everything onto screen
		window.display();
	}
}