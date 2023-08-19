#include "Node.h"

Node::Node(Node* parentNode,sf::Vector2i squarePos) :
    parentNode(parentNode),
    squarePos(squarePos)
{

}

void Node::CalculateNodeScore(sf::Vector2i endNodePos)
{
    if (parentNode != nullptr)
    {
        sf::Vector2i diffParentPos = squarePos - parentNode->squarePos;
        if (diffParentPos.x != 0 && diffParentPos.y != 0)
        {
            this->distanceScore = parentNode->distanceScore + 14;
        }
        else
        {
            this->distanceScore = parentNode->distanceScore + 10;
        }
        sf::Vector2i diffEndPos = squarePos - endNodePos;
        diffEndPos.x = abs(diffEndPos.x);
        diffEndPos.y = abs(diffEndPos.y);
        if (diffEndPos.y > diffEndPos.x)
        {
            this->heuristicScore = diffEndPos.x * 14 + (diffEndPos.y - diffEndPos.x) * 10;
        }
        else
        {
            this->heuristicScore = diffEndPos.y * 14 + (diffEndPos.x - diffEndPos.y) * 10;
        }
        nodeScore = heuristicScore + distanceScore;
    }
    
}
