#include "BoardRendering.hpp"


namespace ne {

namespace detail {

//Hacky function, but it gets the job done
std::string convertTileToTextureAtlasKey(Tile tile) {
    
    if(!tile.isRevealed)
    {
        if(tile.isFlagged)
        {
            return "masked_tile_flag";
        }
        else
        {
            return "masked_tile";
        }
    }

    if(tile.isBomb)
    {
        return "revealed_tile_bomb";
    }

    if(tile.value > 0)
    {
        return "revealed_tile_" + std::to_string(tile.value);
    }
    else
    {
        return "revealed_tile";
    }
    
    std::unreachable();
}

} //namespace detail

BoardRenderer::BoardRenderer(Board& board, TextureAtlas& atlas, sf::Vector2f size) : 
    m_board(&board),
    m_atlas(&atlas),
    m_size(size)
{    
    const std::size_t n = m_board->getTileCount();

    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(n * 6); //6 per quad (2 triangles)
    
    update();
    resize(); 
}

void BoardRenderer::resize() {
    const sf::Vector2u size = m_board->getSize();
    
    const float tileWidth = m_size.x / static_cast<float>(size.x);
    const float tileHeight = m_size.y / static_cast<float>(size.y);
    std::size_t vertexIndex = 0;

    for(unsigned int y = 0; y < size.y; y++)
    {
        float tileY = tileHeight * y;

        for(unsigned int x = 0; x < size.x; x++)
        {
            float tileX = tileWidth * x;

            m_vertices[vertexIndex++].position = sf::Vector2f(tileX, tileY);
            m_vertices[vertexIndex++].position = sf::Vector2f(tileX + tileWidth, tileY);
            m_vertices[vertexIndex++].position = sf::Vector2f(tileX, tileY + tileHeight);

            m_vertices[vertexIndex++].position = sf::Vector2f(tileX + tileWidth, tileY);
            m_vertices[vertexIndex++].position = sf::Vector2f(tileX + tileWidth, tileY + tileHeight);
            m_vertices[vertexIndex++].position = sf::Vector2f(tileX, tileY + tileHeight);
        }
    }
}

void BoardRenderer::update() {
    const sf::Vector2u size = m_board->getSize();

    std::size_t tileIndex = 0;
    std::size_t vertexIndex = 0;

    for(unsigned int y = 0; y < size.y; y++)
    {
        for(unsigned int x = 0; x < size.x; x++)
        {
            const Tile& tile = m_board->getTile(tileIndex++); 
                
            //Update non-flagged-tiles (ensure we accumulate index!)
            if(!tile.isDirty)
            {
                vertexIndex += 6;
                continue;
            }
            
            tile.isDirty = false;
        
            //Get texture rect from atlas
            const auto textureKey = detail::convertTileToTextureAtlasKey(tile);
            const sf::IntRect texRect = m_atlas->getFrame(textureKey);

            const float texX      = texRect.position.x;
            const float texY      = texRect.position.y;
            const float texWidth  = texRect.size.x;
            const float texHeight = texRect.size.y;
            
            m_vertices[vertexIndex++].texCoords = sf::Vector2f(texX, texY);
            m_vertices[vertexIndex++].texCoords = sf::Vector2f(texX + texWidth, texY);
            m_vertices[vertexIndex++].texCoords = sf::Vector2f(texX, texY + texHeight);

            m_vertices[vertexIndex++].texCoords = sf::Vector2f(texX + texWidth, texY);
            m_vertices[vertexIndex++].texCoords = sf::Vector2f(texX + texWidth, texY + texHeight);
            m_vertices[vertexIndex++].texCoords = sf::Vector2f(texX, texY + texHeight);  
        }
    }
}

void BoardRenderer::setSize(sf::Vector2f size) {
    m_size = size;
    resize();
}
    
sf::Vector2f BoardRenderer::getSize() const {
    return m_size;
}

void BoardRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &m_atlas->getTexture();
    states.transform *= getTransform();

    target.draw(m_vertices, states);
}

} //namespace ne