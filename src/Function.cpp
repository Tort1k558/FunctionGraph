#include "Function.h"
#include<map>
#include<string>
#include<set>
#include<vector>
#include<stack>
#include<iostream>
#include<SFML/Graphics.hpp>
#include<imgui.h>

const double PI = 3.141592653589793238462643383279;
const double e = 2.718281828459045235360287471352;

static double fact(double n);
static double op_plus(std::stack<double>& s);
static double op_minus(std::stack<double>& s);
static double op_mul(std::stack<double>& s);
static double op_div(std::stack<double>& s);
static double op_deg(std::stack<double>& s);
static double op_opposite(std::stack<double>& s);
static double op_factorial(std::stack<double>& s);
static double op_odiv(std::stack<double>& s);
static double op_and(std::stack<double>& s);
static double op_or(std::stack<double>& s);
static double op_not(std::stack<double>& s);
static double op_sin(std::stack<double>& s);
static double op_cos(std::stack<double>& s);
static double op_tan(std::stack<double>& s);
static double op_asin(std::stack<double>& s);
static double op_acos(std::stack<double>& s);
static double op_atan(std::stack<double>& s);
static double op_exp(std::stack<double>& s);
Function::Function(std::string expression) :
	m_name(expression),
	m_calculated(false),
	m_color(sf::Color::Blue),
	m_newName(m_name.c_str())
{
    const std::string delimiters(" ()+/*-^&|!%[]");
    for (int i = 0; i < delimiters.size(); i++)
    {
        m_delimSet.insert(delimiters[i]);
    }
	m_priorOps["+"] = 10;
	m_priorOps["-"] = 10;
	m_priorOps["*"] = 20;
	m_priorOps["/"] = 20;
	m_priorOps["^"] = 30;
	m_priorOps["opposite"] = 10;
	m_priorOps["fact"] = 30;
	m_priorOps["%"] = 20;
	m_priorOps["&"] = 5;
	m_priorOps["|"] = 5;
	m_priorOps["!"] = 40;
	m_ops["+"] = op_plus;
	m_ops["-"] = op_minus;
	m_ops["*"] = op_mul;
	m_ops["/"] = op_div;
	m_ops["^"] = op_deg;
	m_ops["opposite"] = op_opposite;
	m_ops["fact"] = op_factorial;
	m_ops["%"] = op_odiv;
	m_ops["&"] = op_and;
	m_ops["|"] = op_or;
	m_ops["!"] = op_not;
	m_ops["sin"] = op_sin;
	m_ops["cos"] = op_cos;
	m_ops["tan"] = op_tan;
	m_ops["acos"] = op_acos;
	m_ops["asin"] = op_asin;
	m_ops["atan"] = op_atan;
	m_ops["exp"] = op_exp;

	m_variables["pi"] = PI;
	m_variables["e"] = e;
	calculateTokens();
}
bool Function::isDelimiter(char sym) {
    return m_delimSet.count(sym) > 0;
};
void Function::setName(std::string name)
{
	m_name = name.c_str();
	calculateTokens();
	m_calculated = false;
}
void Function::createTokensFromExpression(std::string& expr)
{
	std::string ex = expr + " ";
	std::string name;
	for (size_t i = 0; i < ex.size() - 1;)
	{
		name = "";
		if (isDelimiter(ex[i]))
		{
			if (ex[i] == ' ')
			{
				i++;
				continue;
			}
			name = ex[i];
			i++;
		}
		else
		{
			while (!isDelimiter(ex[i]))
			{
				name += ex[i];
				i++;
			}
		}
		m_texpr.push_back(Token(name, var));
	}
	for (size_t i = 0; i < m_texpr.size(); i++)
	{
		if (m_texpr[i].name[0] == '[')
		{
			m_texpr[i].type = op_sbr;
			continue;
		}
		if (m_texpr[i].name[0] == ']') {
			m_texpr[i].type = cl_sbr;
			continue;
		}
		if (m_texpr[i].name[0] == '(') {
			m_texpr[i].type = op_br;
			continue;
		}
		if (m_texpr[i].name[0] == ')') {
			m_texpr[i].type = cl_br;
			continue;
		}
		if (isdigit(m_texpr[i].name[0])) {
			m_texpr[i].type = num;
			continue;
		}
		if (isalpha(m_texpr[i].name[0])) {
			if (i < m_texpr.size() - 1 && m_texpr[i + 1].name[0] == '[')
				m_texpr[i].type = mas;
			//continue;
		}
		if (isalpha(m_texpr[i].name[0])) {
			if (i < m_texpr.size() - 1 && m_texpr[i + 1].name[0] == '(')
				m_texpr[i].type = func;
			continue;
		}
		m_texpr[i].type = op;
	}
	for (int i = 0; i < m_texpr.size(); i++) {
		if (m_texpr[i].name == "-" && (i == 0 || m_texpr[i - 1].type == op_br))
			m_texpr[i].name = "opposite";
		if (m_texpr[i].name == "!" && (i == m_texpr.size() - 1 || m_texpr[i + 1].type == cl_br || m_texpr[i + 1].type == op))
			m_texpr[i].name = "fact";
	}
	return;
}
void Function::createPostfixFromTokens()
{
	std::stack<Token> tStack;
	for (int i = 0; i < m_texpr.size(); i++) {
		switch (m_texpr[i].type) {
		case var:
		case num:
			m_pexpr.push_back(m_texpr[i]);
			break;

		case op_br:
			tStack.push(m_texpr[i]);
			break;

		case cl_br:
			while (tStack.top().type != op_br) {
				m_pexpr.push_back(tStack.top());
				tStack.pop();
			}
			tStack.pop();
			break;

		case op_sbr:
			tStack.push(m_texpr[i]);
			break;

		case cl_sbr:
			while (tStack.top().type != op_sbr) {
				m_pexpr.push_back(tStack.top());
				tStack.pop();
			}
			tStack.pop();
			break;

		case op:
			if (tStack.size()) {
				while (tStack.size() && ((tStack.top().type == op && m_priorOps[m_texpr[i].name] <= m_priorOps[tStack.top().name]) ||
					tStack.top().type == func ||
					tStack.top().type == mas)) {
					m_pexpr.push_back(tStack.top());
					tStack.pop();
				}
			}
			tStack.push(m_texpr[i]);
			break;

		case mas:
			while (tStack.size() && tStack.top().type == mas) {
				m_pexpr.push_back(tStack.top());
				tStack.pop();
			}
			tStack.push(m_texpr[i]);
			break;

		case func:
			while (tStack.size() && tStack.top().type == func) {
				m_pexpr.push_back(tStack.top());
				tStack.pop();
			}
			tStack.push(m_texpr[i]);
			break;
		}
	}

	while (tStack.size()) {
		m_pexpr.push_back(tStack.top());
		tStack.pop();
	}
}
void Function::calculateTokens()
{
	m_texpr.clear();
	m_pexpr.clear();
	createTokensFromExpression(m_name);
	createPostfixFromTokens();
}
double Function::findValueY()
{
	std::stack<double> s;

	for (int i = 0; i < m_pexpr.size(); i++) {
		switch (m_pexpr[i].type) {
		case num: {
			s.push(atoi(m_pexpr[i].name.c_str()));
			break;
		}

		case var: {
			if (m_variables.find(m_pexpr[i].name) != m_variables.end()) {
				s.push(m_variables[m_pexpr[i].name]);
				break;
			}
		}


		case func:
		case op: {
			s.push(m_ops[m_pexpr[i].name](s));
		}

		}
	}

	return s.top();
}
void Function::calculate(sf::RenderWindow& window,float lineLength,int oneStep)
{
	m_lines.clear();
	m_intersectionWithAxisX.clear();
	m_intersectionWithAxisY.clear();
	sf::View view = window.getView();
	sf::Vector2f sizeOfViewport = view.getSize();
	sf::Vector2f centerOfViewport = view.getCenter();
	sf::Vector2f bordersOfViewX = { centerOfViewport.x - sizeOfViewport.x / 2.0f,centerOfViewport.x + sizeOfViewport.x / 2.0f };
	sf::Vector2f oldPosDot = {};
	bool first = true;
	for (float i = -lineLength; i < lineLength; i += 2.0f)
	{
		if (i < bordersOfViewX.x || i > bordersOfViewX.y)
		{
			continue;
		}
		float x = i / oneStep;
		m_variables["x"] = x;
		float y = findValueY();
		if (y == 0)
		{
			m_intersectionWithAxisX.push_back({ x,y });
		}
		if (y == INFINITY)
		{
			first = true;
			continue;
		}
		float j = y * 30.0f;
		if (first)
		{
			oldPosDot.x = i;
			oldPosDot.y = -j;
			first = false;
		}
		MyLine line(oldPosDot.x, oldPosDot.y, i, -j);
		line.setColor(m_color);
		m_lines.push_back(line);
		oldPosDot.x = i;
		oldPosDot.y = -j;
	}
	m_calculated = true;
}

void Function::draw(sf::RenderWindow& window, float lineLength, int oneStep)
{
	if (!m_calculated)
	{
		calculate(window, lineLength,oneStep);
	}
    for (size_t i = 0; i < m_lines.size();i++)
    {
        window.draw(m_lines[i].getVertices(), 2, sf::Lines);
    }
	ImGui::Begin(m_name.c_str());
	ImGui::InputText("Function name", m_newName.data(), 256);
	if (ImGui::Button("setName"))
	{
		setName(m_newName);
	}
	ImGui::InputDouble("X: ", &m_x);
	if (ImGui::Button("Calculate"))
	{
		m_variables["x"] = m_x;
		m_y = findValueY();
	}
	ImGui::Text((std::string("Y: ") + std::to_string(m_y)).c_str());
	
	ImGui::SliderInt3("RGB", m_rgb,0,255);
	if (ImGui::Button("Set Color"))
	{
		setColor(sf::Color(m_rgb[0], m_rgb[1], m_rgb[2]));
		recalculate();
	}
	std::string intersectionWithAxisX("IntersectionsWithAxisX:");
	for (size_t i = 0; i < m_intersectionWithAxisX.size(); i++)
	{
		intersectionWithAxisX += std::string("{" + std::to_string(m_intersectionWithAxisX[i].first) + "," + std::to_string(m_intersectionWithAxisX[i].second) + "};");
	}
	ImGui::Text(intersectionWithAxisX.c_str());
	ImGui::End();
}
static double fact(double n) {
	if (n < 0.0)
	{
		return INFINITY;
	}
	int a = static_cast<int>(n);
	if (a == 0)
		return 1;
	return a * fact(a - 1);
}
static double op_plus(std::stack <double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a + b;
}
static double op_minus(std::stack <double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b - a;
}
static double op_mul(std::stack<double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a * b;
}
static double op_div(std::stack<double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b / a;
}
static double op_deg(std::stack<double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	//b^a!!
	return pow(b, a);
}
static double op_opposite(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return -a;
}
static double op_factorial(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return fact(a);
}
static double op_odiv(std::stack <double>& s) {
	long long a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return b % a;
}
static double op_and(std::stack <double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a && b;
}
static double op_or(std::stack <double>& s) {
	double a, b;
	a = s.top();
	s.pop();
	b = s.top();
	s.pop();
	return a || b;
}
static double op_not(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return !a;
}
static double op_sin(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return sin(a);
}
static double op_cos(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return cos(a);
}
static double op_tan(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return tan(a);
}
static double op_asin(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return asin(a);
}
static double op_acos(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return acos(a);
}
static double op_atan(std::stack <double>& s) {
	double a;
	a = s.top();
	s.pop();
	return atan(a);
}
static double op_exp(std::stack <double>& s)
{
	double a = s.top();
	s.pop();
	return exp(a);
}