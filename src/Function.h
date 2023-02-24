#pragma once
#include<map>
#include<string>
#include<set>
#include<vector>
#include<stack>
#include<functional>
#include<SFML/Graphics.hpp>
#include<Line.h>

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
};

class Function
{
public:
    Function(std::string name);
    void draw(sf::RenderWindow& window, int oneStep);
    void setColor(sf::Color col) { m_color = col; }
    void recalculate() { m_calculated = false; }
    void setName(std::string name);
    const char* getName() { return m_name.c_str(); }
    double calculateValueY();
private:
    bool isDelimiter(char sym);
    void createTokensFromExpression(std::string& expr);
    void createPostfixFromTokens();
    void calculate(sf::RenderWindow& window, int oneStep);
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
    std::vector<Line> m_lines;
    std::string m_newName;
    std::set<std::pair<double, double>> m_intersectionWithAxisX;
    std::set<std::pair<double, double>> m_intersectionWithAxisY;
    int m_rgb[3]{};
    double m_x = 0;
    double m_y = 0;
};