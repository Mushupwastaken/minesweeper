#include <iosfwd>

#include "game/Board.hpp"
#include "game/BoardRendering.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 800}), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(15u);

    ne::Board board({8, 8}, 10);
    ne::TextureAtlas atlas("assets/minesweeper.png", "assets/minesweeper.json");
    ne::BoardRenderer renderer(board, atlas, sf::Vector2f(window.getSize()));    

    while(window.isOpen())
    {
        while(const std::optional event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }   
            
            //I know these events are stupid, but it works.
            if(auto key = event->getIf<sf::Event::KeyPressed>())
            {
                if(key->scancode == sf::Keyboard::Scancode::R)
                {
                    board.generate();
                    renderer.update();
                }
                else if(key->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                }
            }

            if(auto mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2u boardSize = board.getSize();
                sf::Vector2f rendererSize = renderer.getSize();
                sf::Vector2f tileSize = renderer.getSize().componentWiseDiv(sf::Vector2f(boardSize));
                
                sf::Vector2f mousePos = window.mapPixelToCoords(mouse->position);
                sf::Vector2u indexedPos = sf::Vector2u(mousePos.componentWiseDiv(tileSize));
                
                //This really should be in renderer
                std::size_t index = static_cast<std::size_t>(indexedPos.y * boardSize.x + indexedPos.x);

                if(index < board.getTileCount())
                {
                    if(mouse->button == sf::Mouse::Button::Left)
                    {
                        board.revealTile(index);
                    }
                    else if(mouse->button == sf::Mouse::Button::Right)
                    {                    
                        board.flagTile(index);
                    }
                }

                renderer.update();
            }
        }

        window.clear(sf::Color::Black);
        window.draw(renderer);
        window.display();
    }
}
