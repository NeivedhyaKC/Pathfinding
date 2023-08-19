#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include<unordered_map>
#include<iterator>
#include"Node.h"

class Game
{
public:
    Game();
    ~Game();

    void run();
private:
    void UpdatemousePos();
    void SpawnSquares();
    void updateSFMLEvents();
    void update();
    void render();
    void SurroundingNodes(sf::Vector2i arrayPos);
    void LowestNodeScore();
    void PathFinding();
    void EndNodeFound();
    void Reset();
    
    float Y(float y);
    float X(float x);

    sf::Vector2i startNodePos;
    sf::Vector2i endNodePos;
    sf::Vector2i mousePos;
    std::unordered_map<int, Node*> openMap;
    std::unordered_map<int, Node*> closedMap;
    std::unordered_map<int, Node*> blockedMap;
    
    sf::RenderWindow* renderWindow;
    sf::Event event;
    sf::Clock clock;
    float dt = 0.f;
    sf::RectangleShape squares[54][30];
    sf::Sprite playSprite;
    bool readyForPathfinding = false;
    bool firstNodeselected = false;
};

