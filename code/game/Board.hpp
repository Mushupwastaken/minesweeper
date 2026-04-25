#pragma once

#include "Tile.hpp"
#include <random>

//Container class for board
//Doesnt process rendering (inc. interfacing with rendering)

namespace ne {



class Board {
private:
    sf::Vector2u m_size = {8, 8};
    unsigned int m_bombCount = 10;
    unsigned int m_flagCount = 0;

    bool m_hasRevealedTile = false;
    bool m_hasRevealedBomb = false;

    std::vector<Tile> m_tiles;
public:
    Board(sf::Vector2u size, unsigned int bombCount);

    void generate();

    void setTile(std::size_t index, bool isBomb);
    void flagTile(std::size_t index);    
    void revealTile(std::size_t index);
 
    [[nodiscard]] const Tile& getTile(std::size_t index) const;
    [[nodiscard]] std::size_t getTileCount() const;

    [[nodiscard]] sf::Vector2u getSize() const; 
    [[nodiscard]] unsigned int getBombCount() const;
    [[nodiscard]] unsigned int getFlagCount() const;
private:
    template <typename Func>
    void forEachNeighbouringIndex(std::size_t index, Func&& func);
    
    template <typename Func>
    void forEachNeighbouringTile(std::size_t index, Func&& func);
};

} //namespace ne

#include "Board.inl"