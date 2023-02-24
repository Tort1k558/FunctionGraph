#pragma once
#include<SFML/Graphics.hpp>
class Line
{
public:
    Line(float x0, float y0, float x1, float y1);
    void setColor(sf::Color color);
    sf::Vertex* getVertices()
    {
        return &m_vertices[0];
    }

private:
    sf::VertexArray m_vertices;
};