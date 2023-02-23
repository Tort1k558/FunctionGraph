#pragma once
#include<map>
#include<string>
#include<set>
#include<vector>
#include<stack>
#include<functional>
#include<SFML/Graphics.hpp>

class MyLine
{
public:
    MyLine(float x0, float y0, float x1, float y1) : m_vertices(sf::Lines, 2)
    {
        m_vertices[0].position.x = x0;
        m_vertices[0].position.y = y0;
        m_vertices[1].position.x = x1;
        m_vertices[1].position.y = y1;
    }
    sf::Vertex* getVertices()
    {
        return &m_vertices[0];
    }
    void setColor(sf::Color color)
    {
        m_vertices[0].color = color;
        m_vertices[1].color = color;
    }

private:
    sf::VertexArray m_vertices;
};

enum tokentype
{
    var, num, op_br, cl_br, func, op, mas, op_sbr, cl_sbr
};

struct Token
{
    std::string name;
    tokentype type;
    Token(std::string str, tokentype typ) :
        name(str),
        type(typ)
    {
    }
    Token() {}
};

class Function
{
public:
    Function(std::string name);
    void draw(sf::RenderWindow& window, float lineLength, int oneStep);
    void setColor(sf::Color col) { m_color = col; }
    void recalculate() { m_calculated = false; }
    void setName(std::string name);
    const char* getName() { return m_name.c_str(); }
    double findValueY();
private:
    bool isDelimiter(char sym);
    void createTokensFromExpression(std::string& expr);
    void createPostfixFromTokens();
    void calculate(sf::RenderWindow& window, float lineLength, int oneStep);
    void calculateTokens();

    bool m_calculated;
    std::string m_name;
    sf::Color m_color;
    std::vector<Token> m_texpr;
    std::vector<Token> m_pexpr;
    std::map<std::string, int> m_priorOps;
    std::unordered_map<std::string, double> m_variables;
    std::set<char> m_delimSet;
    std::map<std::string, std::function<double(std::stack<double>&)>> m_ops;
    std::vector<MyLine> m_lines;
    std::string m_newName;
    std::set<std::pair<double, double>> m_intersectionWithAxisX;
    std::set<std::pair<double, double>> m_intersectionWithAxisY;
    int m_rgb[3]{};
    double m_x = 0;
    double m_y = 0;
};