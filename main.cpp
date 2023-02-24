#include<SFML/Graphics.hpp>
#include<iostream>
#include<functional>
#include<imgui.h>
#include<imgui-SFML.h>
#include"Function.h"
#include<thread>
//TODO
//intersection of function
int g_oneStep = 50.0f;
bool g_fieldHasChanged = true;
int g_width = 800;
int g_height = 600;
void DrawField(sf::RenderWindow& window);
int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(g_width, g_height), "Function Graph");
    
    sf::View view = window.getView();
    view.setCenter({ 0.0f,0.0f });
    window.setView(view);
    ImGui::SFML::Init(window);
    std::vector<Function> functions;
    sf::Clock clock;
    sf::Vector2f oldPosMouse;
    int itemToDelete = 0;
    char newFuncName[255]{};
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::Resized)
            {
                window.setSize({ event.size.width,event.size.height });
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                sf::View view = window.getView();
                if (event.mouseWheelScroll.delta > 0)
                {
                    view.zoom(0.9f);
                }
                else if (event.mouseWheelScroll.delta < 0)
                {
                    view.zoom(1.1f);
                }
                window.setView(view);
                g_fieldHasChanged = true;
            }
            if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                oldPosMouse.x = event.mouseButton.x;
                oldPosMouse.y = event.mouseButton.y;
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
            {
                float deltaX = oldPosMouse.x - event.mouseButton.x;
                float deltaY = oldPosMouse.y - event.mouseButton.y;
                sf::View currentView = window.getView();
                sf::Vector2f currentCenter = currentView.getCenter();
                currentView.setCenter({ currentCenter.x + deltaX,currentCenter.y + deltaY });
                window.setView(currentView);
                g_fieldHasChanged = true;
            }
        }
        ImGui::SFML::Update(window, clock.restart());
        if (g_fieldHasChanged)
        {
            for (size_t i = 0; i < functions.size(); i++)
            {
                functions[i].recalculate();
            }
            DrawField(window);
            g_fieldHasChanged = false;
        }
        for (size_t i = 0; i < functions.size(); i++)
        {
            functions[i].draw(window, g_oneStep);
        }
        DrawField(window);
        ImGui::Begin("FunctionGraph");
        ImGui::SliderInt("OneStep", &g_oneStep,30,300);
        if (ImGui::Button("Apply"))
        {
            g_fieldHasChanged = true;
        }
        ImGui::InputText("New Function name", newFuncName, 255);
        if (ImGui::Button("create Function"))
        {
            functions.push_back(Function(newFuncName));
        }
        const char* items[255]{};
        for (size_t i = 0; i < functions.size(); i++)
        {
            items[i] = functions[i].getName();
        }
        ImGui::ListBox("Functions:", &itemToDelete, items, functions.size());
        if(ImGui::Button("delete Function"))
        {
            functions.erase(functions.begin()+ itemToDelete);
        }
        ImGui::End();

        ImGui::SFML::Render(window);
        window.display();
        window.clear();
    }
    ImGui::SFML::Shutdown();
	return 0;
}
void DrawField(sf::RenderWindow& window)
{
    static std::vector<Line> lines;
    static std::vector<sf::Text> texts;
    sf::Font font;
    font.loadFromFile("1.ttf");
    sf::Text text("", font, g_oneStep / 3);
    text.setStyle(sf::Text::Bold);
    text.setColor(sf::Color::White);
    sf::View view = window.getView();
    sf::Vector2f sizeOfViewport = view.getSize();
    sf::Vector2f centerOfViewport = view.getCenter();
    sf::Vector2f bordersOfViewX = { centerOfViewport.x - sizeOfViewport.x / 2.0f,centerOfViewport.x + sizeOfViewport.x / 2.0f };
    sf::Vector2f bordersOfViewY = { centerOfViewport.y - sizeOfViewport.y / 2.0f,centerOfViewport.y + sizeOfViewport.y / 2.0f };
    if (g_fieldHasChanged)
    {
        lines.clear();
        texts.clear();
        const int indent = 5;
        std::cout << "X1: " << bordersOfViewX.x << std::endl;
        std::cout << "X2: " << bordersOfViewX.y << std::endl;
        float widthIndentation = 0.0f;
        //left
        for (int i = 0; i > bordersOfViewX.x / g_oneStep; i--)
        {
            Line line(widthIndentation, indent, widthIndentation, -indent);
            lines.push_back(line);

            text.setString(std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.y -= 8;
            text.setPosition(textPos->position);
            texts.push_back(sf::Text(text));
            widthIndentation -= g_oneStep;
        }
        //right
        widthIndentation = 0.0f;
        for (int i = 0; i < bordersOfViewX.y / g_oneStep; i++)
        {
            Line line(widthIndentation, indent, widthIndentation, -indent);
            lines.push_back(line);
            text.setString(std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.y -= 8;
            text.setPosition(textPos->position);
            if (i != 0)
            {
                texts.push_back(sf::Text(text));
            }
            widthIndentation += g_oneStep;
        }
        std::cout << "Y1: " << bordersOfViewY.x << std::endl;
        std::cout << "Y2: " << bordersOfViewY.y << std::endl;
        //up
        float heightIndentation = 0.0f;
        for (int i = 0; i > bordersOfViewY.x / g_oneStep; i--)
        {
            Line line(indent, heightIndentation, -indent, heightIndentation);
            lines.push_back(line);
            text.setString(std::to_string(abs(i)));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.x += 8;
            text.setPosition(textPos->position);
            if (i != 0)
            {
                texts.push_back(sf::Text(text));
            }
            heightIndentation -= g_oneStep;
        }
        //down
        heightIndentation = 0;
        for (int i = 0; i < bordersOfViewY.y / g_oneStep; i++)
        {
            Line line(indent, heightIndentation, -indent, heightIndentation);
            lines.push_back(line);
            text.setString('-' + std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.x += 8;
            text.setPosition(textPos->position);
            if (i != 0)
            {
                texts.push_back(sf::Text(text));
            }
            heightIndentation += g_oneStep;
        }
    }
    for (size_t i = 0; i < lines.size(); i++)
    {
        window.draw(lines[i].getVertices(), 2, sf::Lines);
    }
    for (size_t i = 0; i < texts.size(); i++)
    {
        window.draw(texts[i]);
    }

    text.setCharacterSize(15);
    text.setString("X");
    text.setPosition(sf::Vector2f(g_width /2.0f - 15.0f, -30.0f));
    window.draw(text);
    text.setString("Y");
    text.setPosition(sf::Vector2f(30.0f, -g_height / 2.0f));
    window.draw(text);
    Line lineX(bordersOfViewX.x, 0.f, bordersOfViewX.y, 0.f);
    Line lineY(0.0f, bordersOfViewY.x, 0.0f, bordersOfViewY.y);
    window.draw(lineX.getVertices(), 2, sf::Lines);
    window.draw(lineY.getVertices(), 2, sf::Lines);

}
