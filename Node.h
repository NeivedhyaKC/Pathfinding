#pragma once
#include<SFML/Graphics.hpp>

class Node  
{
public:

    Node(Node* parentNode,sf::Vector2i squarePos);

    void CalculateNodeScore(sf::Vector2i endNodePos);

    sf::Vector2i squarePos;
    bool isBlocked = false;
    bool isClosed = false;
    float nodeScore = 0.f;
    Node* parentNode = nullptr;
    float distanceScore = 0.f;

private:
    float heuristicScore = 0.f;
};

