#include<SFML/Graphics.hpp>
#include<iostream>
#include<functional>
#include<imgui.h>
#include<imgui-SFML.h>
#include"Function.h"
#include<thread>
//TODO
//intersection of function
int g_oneStep = 30.0f;
float g_lineLength = 1000;
bool g_fieldHasChanged = true;
void DrawField(sf::RenderWindow& window)
{
    static std::vector<MyLine> lines;
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
    if (g_fieldHasChanged)
    {
        lines.clear();
        texts.clear();
        const int indent = 5;
        
        int widthIndentation = 0;
        for (size_t i = 0; i < g_lineLength / g_oneStep; i++)
        {
            MyLine line(widthIndentation, indent, widthIndentation, -indent);
            lines.push_back(line);

            text.setString('-' + std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.y -= 8;
            text.setPosition(textPos->position);
            texts.push_back(sf::Text(text));
            widthIndentation -= g_oneStep;
        }
        widthIndentation = 0;
        for (size_t i = 0; i < g_lineLength / g_oneStep; i++)
        {
            MyLine line(widthIndentation, indent, widthIndentation, -indent);
            lines.push_back(line);
            text.setString(std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.y -= 8;
            text.setPosition(textPos->position);
            texts.push_back(sf::Text(text));
            widthIndentation += g_oneStep;
        }
        sf::Vector2f bordersOfViewY = { centerOfViewport.y - sizeOfViewport.y / 2.0f,centerOfViewport.y + sizeOfViewport.y / 2.0f };
        int heightIndentation = 0;
        for (size_t i = 0; i < g_lineLength / g_oneStep; i++)
        {
            MyLine line(indent, heightIndentation, -indent, heightIndentation);
            lines.push_back(line);
            text.setString(std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.x += 8;
            text.setPosition(textPos->position);
            texts.push_back(sf::Text(text));
            heightIndentation -= g_oneStep;
        }
        heightIndentation = 0;
        for (size_t i = 0; i < g_lineLength / g_oneStep; i++)
        {
            MyLine line(indent, heightIndentation, -indent, heightIndentation);
            lines.push_back(line);
            text.setString('-' + std::to_string(i));
            sf::Vertex* textPos = line.getVertices();
            textPos->position.x += 8;
            text.setPosition(textPos->position);
            texts.push_back(sf::Text(text));
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
    text.setPosition(sf::Vector2f(300.0f, -20.0f));
    window.draw(text);
    text.setString("Y");
    text.setPosition(sf::Vector2f(20.0f, -300.0f));
    window.draw(text);
    MyLine lineX(-g_lineLength, 0.f, g_lineLength, 0.f);
    MyLine lineY(0.0f, -g_lineLength, 0.0f, g_lineLength);
    window.draw(lineX.getVertices(), 2, sf::Lines);
    window.draw(lineY.getVertices(), 2, sf::Lines);

}

int main(int argc, char* argv[])
{
    int width = 800;
    int height = 600;
    sf::RenderWindow window(sf::VideoMode(width, height), "Function Graph");
    
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
        ImGui::SFML::Update(window, clock.getElapsedTime());
        if (g_fieldHasChanged)
        {
            g_lineLength = abs(window.getView().getCenter().x) + abs(window.getView().getSize().x / 2.0f);
            if ((abs(window.getView().getCenter().y) + abs(window.getView().getSize().y / 2.0f)) > g_lineLength)
            {
                g_lineLength = abs(window.getView().getCenter().y) + abs(window.getView().getSize().y / 2.0f);
            }
            for (size_t i = 0; i < functions.size(); i++)
            {
                functions[i].recalculate();
            }
            DrawField(window);
            g_fieldHasChanged = false;
        }
        window.clear();
        for (size_t i = 0; i < functions.size(); i++)
        {
            functions[i].draw(window, g_lineLength, g_oneStep);
        }
        DrawField(window);
        ImGui::Begin("FunctionGraph");
        ImGui::SliderInt("OneStep", &g_oneStep,1,50);

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
    }
    ImGui::SFML::Shutdown();
	return 0;
}