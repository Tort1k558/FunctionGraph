#include<Line.h>

Line::Line(float x0, float y0, float x1, float y1) :
    m_vertices(sf::Lines, 2)
{
    m_vertices[0].position.x = x0;
    m_vertices[0].position.y = y0;
    m_vertices[1].position.x = x1;
    m_vertices[1].position.y = y1;
}
void Line::setColor(sf::Color color)
{
    m_vertices[0].color = color;
    m_vertices[1].color = color;
}