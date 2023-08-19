#include "Game.h"

Game::Game()
{
	this->renderWindow = new sf::RenderWindow(sf::VideoMode(1080, 720), "Pathfinding");
	this->dt = 0.f;
	sf::Texture playTexture;
	playTexture.loadFromFile("PlaySprite.png");
	this->playSprite.setTexture(playTexture);
	playSprite.setPosition(sf::Vector2f(30, 20));
}

Game::~Game()
{
	delete this->renderWindow;
}

void Game::UpdatemousePos()
{
	this->mousePos.x = (10 + sf::Mouse::getPosition(*this->renderWindow).x) / 20;
	this->mousePos.y = (660 - sf::Mouse::getPosition(*this->renderWindow).y) / 20;
}

void Game::SpawnSquares()
{
	for (int j = 0; j < 30; j++)
	{
		for (int i = 0; i < 54; i++)
		{
			squares[i][j].setSize(sf::Vector2f(20, 20));
			squares[i][j].setPosition(sf::Vector2f(static_cast<float>(X(i)), static_cast<float>(Y(j))));
			squares[i][j].setOrigin(sf::Vector2f(10, 10));
			squares[i][j].setOutlineThickness(2);
			squares[i][j].setOutlineColor(sf::Color::Black);
			this->renderWindow->draw(squares[i][j]);
		}
	}
}

void Game::updateSFMLEvents()
{
	while (this->renderWindow->pollEvent(this->event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			this->renderWindow->close();
			Reset();
		}
	}
}

void Game::update()
{
	updateSFMLEvents();	
}

void Game::render()
{
	this->renderWindow->clear();
	for (int j = 0; j < 30; j++)
	{
		for (int i = 0; i < 54; i++)
	    {
			this->renderWindow->draw(squares[i][j]);
		}
	}
	this->renderWindow->draw(this->playSprite);
	this->renderWindow->display();
}

void Game::SurroundingNodes(sf::Vector2i arrayPos)
{
	closedMap.at(arrayPos.x * 100 + arrayPos.y)->isClosed = true;
	for (int j = -1; j < 2; j++)
	{
		for (int i = -1; i < 2; i++)
		{
			if (i != 0 || j != 0)
			{
				int surrNodeX = std::min(std::max(arrayPos.x + i, 0), 53);
				int surrNodeY = std::min(std::max(arrayPos.y + j, 0), 29);
				Node* surrNewNode = new Node(closedMap.at(arrayPos.x * 100 + arrayPos.y), sf::Vector2i(surrNodeX, surrNodeY));
				surrNewNode->CalculateNodeScore(endNodePos);
				if (openMap.find(surrNodeX * 100 + surrNodeY) == openMap.end() && closedMap.find(surrNodeX * 100 + surrNodeY) == closedMap.end()
					 && blockedMap.find(surrNodeX * 100 + surrNodeY)== blockedMap.end())
				{
					openMap.insert({ surrNodeX * 100 + surrNodeY,surrNewNode }); 
					squares[surrNodeX][surrNodeY].setFillColor(sf::Color::Blue);
				}
				else 
				if(closedMap.find(surrNodeX * 100 + surrNodeY) == closedMap.end() && 
				   blockedMap.find(surrNodeX * 100 + surrNodeY) == blockedMap.end())
				{
					Node* surrNode = openMap.at(surrNodeX * 100 + surrNodeY);
					if (surrNewNode->nodeScore < surrNode->nodeScore)
					{
						openMap.erase(surrNode->squarePos.x * 100 + surrNode->squarePos.y);
						delete surrNode;
						openMap.insert({ surrNewNode->squarePos.x * 100 + surrNewNode->squarePos.y,surrNewNode });
						squares[surrNodeX][surrNodeY].setFillColor(sf::Color::Blue);
					}
					else
					{
						delete surrNewNode;
					}
				}
			}
		}
	}
	for (int j = -1; j < 2; j += 2)
	{
		for (int i = -1; i < 2; i += 2)
		{
			int surrNodeX = std::min(std::max(arrayPos.x + i, 0), 53);
			int surrNodeY = std::min(std::max(arrayPos.y + j, 0), 29);
			if (blockedMap.find(surrNodeX * 100 + arrayPos.y) != blockedMap.end() && blockedMap.find(arrayPos.x * 100 + surrNodeY) != blockedMap.end()
			&& openMap.find(surrNodeX * 100 + surrNodeY) != openMap.end())
			{
				if (blockedMap.at(surrNodeX * 100 + arrayPos.y)->isBlocked && blockedMap.at(arrayPos.x * 100 + surrNodeY)->isBlocked)
				{
					squares[surrNodeX][surrNodeY].setFillColor(sf::Color::White);
					openMap.erase(surrNodeX * 100 + surrNodeY);
				}
			}
			
		}
	}
	update();
	render();
}

void Game::LowestNodeScore()
{
	int lowestScore = 100000;
	sf::Vector2i lowestSquarePos;
	if (openMap.size()> 0)
	{
		for (std::pair<int, Node*> pair : openMap)
		{
			if (pair.second->nodeScore < lowestScore && !pair.second->isBlocked)
			{
				lowestSquarePos = pair.second->squarePos;
				lowestScore = pair.second->nodeScore;
			}
		}
		closedMap.insert({ lowestSquarePos.x * 100 + lowestSquarePos.y, openMap.at(lowestSquarePos.x * 100 + lowestSquarePos.y) });
		openMap.erase(lowestSquarePos.x * 100 + lowestSquarePos.y);
		squares[lowestSquarePos.x][lowestSquarePos.y].setFillColor(sf::Color::Red);
	}
	update();
	render();
	if (lowestSquarePos == endNodePos)
	{
		EndNodeFound();
	}
	else
	{
		if (openMap.size() < 0)
		{
			std::cout << "No Path Available" << std::endl;
		}
		else
		{
			PathFinding();
		}
	}
}

void Game::PathFinding()
{
	for (std::pair<int, Node*> pair : closedMap)/*openMapIterator = openMap.begin(); openMapIterator != openMap.end(); openMapIterator++*/
	{
		if (!pair.second->isClosed)
		{
			SurroundingNodes(pair.second->squarePos);
		}
	}
	LowestNodeScore();
	update();
	render();
}

void Game::EndNodeFound()
{
	sf::Vector2i pathPos = endNodePos;
	while (pathPos != startNodePos)
	{
		squares[pathPos.x][pathPos.y].setFillColor(sf::Color::Green);
		pathPos = closedMap.at(pathPos.x * 100 + pathPos.y)->parentNode->squarePos;
		update();
		render();
	}
	
}

void Game::Reset()
{
	for (std::pair<int, Node*> pair : openMap)
	{
		int x = pair.first / 100;
		int y = std::min(pair.first % 100, 29);
		squares[x][y].setFillColor(sf::Color::White);
		openMap.erase(pair.first);
		delete pair.second;
	}
	openMap.clear();
	for (std::pair<int, Node*> pair : closedMap)
	{
		int x = pair.first / 100;
		int y = pair.first % 100;
		squares[x][y].setFillColor(sf::Color::Black);
		closedMap.erase(pair.first);
		delete pair.second;
	}
	closedMap.clear();
	for (std::pair<int,Node*> pair : closedMap)
	{
		int x = pair.second->squarePos.x;
		int y = pair.second->squarePos.y;
		squares[x][y].setFillColor(sf::Color::Black);
		blockedMap.erase(pair.first);
		delete pair.second;
	}
	blockedMap.clear();
	firstNodeselected = false;
	readyForPathfinding = false;
}


void Game::run()
{
	SpawnSquares();
	while (this->renderWindow->isOpen())
	{
		
		UpdatemousePos();
		update();
		render();
		sf::Vector2i mousePos=sf::Mouse::getPosition(*this->renderWindow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && playSprite.getTextureRect().contains(mousePos)
			&& readyForPathfinding)
		{
			PathFinding();
		} 
		else if (this->mousePos.x >= 0 && this->mousePos.x <= 53 && this->mousePos.y >= 0 && this->mousePos.y <= 29)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if (openMap.find(this->mousePos.x * 100 + this->mousePos.y) == openMap.end())
				{
					Node* blockNode = new Node(nullptr, sf::Vector2i(this->mousePos.x, this->mousePos.y));
					squares[this->mousePos.x][this->mousePos.y].setFillColor(sf::Color::Black);
					blockedMap.insert({ this->mousePos.x * 100 + this->mousePos.y,blockNode });
					blockNode->isBlocked = true;
				}
				
			}
			else if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right) && !readyForPathfinding)
			{
				if (!firstNodeselected)
				{
					Node* startNode = new Node(nullptr, sf::Vector2i(this->mousePos.x, this->mousePos.y));
					this->startNodePos = sf::Vector2i(this->mousePos.x, this->mousePos.y);
					closedMap.insert({ this->startNodePos.x * 100 + this->startNodePos.y,startNode });
					squares[this->mousePos.x][this->mousePos.y].setFillColor(sf::Color::Red);
					firstNodeselected = true;
					startNode->CalculateNodeScore(this->endNodePos);
				}
				else if (firstNodeselected && startNodePos != sf::Vector2i(this->mousePos.x, this->mousePos.y))
				{
					this->endNodePos = sf::Vector2i(this->mousePos.x, this->mousePos.y);
					squares[this->mousePos.x][this->mousePos.y].setFillColor(sf::Color::Red);
					readyForPathfinding = true;
				}
			}
		}


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			this->renderWindow->close();
		}
	}
}



float Game::Y(float y)
{
	return 660.f - y * 20;
}

float Game::X(float x)
{
	return x * 20 + 10.f;
}


