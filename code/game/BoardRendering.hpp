#pragma once

#include "Board.hpp"
#include "core/TextureAtlas.hpp"

namespace ne {

class BoardRenderer : public sf::Drawable, public sf::Transformable {
private:
    Board* m_board;
    TextureAtlas* m_atlas;

    sf::Vector2f m_size;
    sf::VertexArray m_vertices;
public:
    BoardRenderer(Board& board, TextureAtlas& atlas, sf::Vector2f size);
    
    void resize();
    void update();

    void setSize(sf::Vector2f size);
    [[nodiscard]] sf::Vector2f getSize() const;

    //Updating mesh will go here
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

} //namespace ne