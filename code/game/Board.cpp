#include "Board.hpp"


namespace ne {

Board::Board(sf::Vector2u size, unsigned int bombCount) : 
    m_size(size),
    m_bombCount(bombCount)
{
    generate();
}

void Board::generate() {
    const std::size_t n = getTileCount();

    //Reset flags
    m_flagCount = 0;
    m_bombCount = std::min<unsigned int>(m_bombCount, n);

    m_hasRevealedTile = false;
    m_hasRevealedBomb = false;
    
    m_tiles.assign(n, Tile{0, false});
    
    //Find n unique indices to place the bombs
    static std::mt19937 gen(std::random_device{}()); 
    std::uniform_int_distribution<std::size_t> dist(0, n - 1);
    std::unordered_set<std::size_t> unique{};
    
    while(unique.size() < m_bombCount) 
    {
        std::size_t index = dist(gen);
        auto [_, inserted] = unique.insert(index);
        
        if(inserted)
        {
            setTile(index, true);
        }
    }
}

void Board::setTile(std::size_t index, bool isBomb) {
    Tile& primaryTile = m_tiles[index];
    
    //Skip if there is no change in state
    if(primaryTile.isBomb == isBomb)
    {
        return;
    }
    
    //Recompute values using neighbours
    primaryTile.value = 0;
    primaryTile.isBomb = isBomb;
    primaryTile.isDirty = true;

    if(isBomb)
    {
        //No guard necessary as value cannot exceed 8
        forEachNeighbouringTile(index, [&](Tile& neighbor) {
            neighbor.value++;
            
            neighbor.isDirty = true;
        });
    }
    else
    {
        forEachNeighbouringTile(index, [&](Tile& neighbor) {
            if(neighbor.isBomb)
            {
                primaryTile.value++;
            }
            
            if(neighbor.value > 0)
            {
                neighbor.value--;
            }

            neighbor.isDirty = true;
        });
    }
}

void Board::flagTile(std::size_t index) {
    //Ensure the playfield can be tampered with before acting (SHOULD MAKE A FUNCTION)
    if(m_hasRevealedBomb)
    {
        return;
    }
    
    Tile& tile = m_tiles[index];
    
    if(tile.isFlagged)
    {
        tile.isFlagged = false;
        m_flagCount--;
    }
    else if(m_flagCount < m_bombCount)
    {
        tile.isFlagged = true;
        m_flagCount++;
    }

    tile.isDirty = true;
}

void Board::revealTile(std::size_t index) {
    Tile& tile = m_tiles[index];
    
    //Quick exit (recursion)
    if(tile.isRevealed || tile.isFlagged || m_hasRevealedBomb)
    {
        return;
    }
    
    //First reveal bomb check (matches microsoft impl)
    if(!m_hasRevealedTile && tile.isBomb)
    {
        //Flag first regular tile as bomb (noexcept)
        for(std::size_t i = 0; i < getTileCount(); i++)
        {
            if(m_tiles[i].isBomb)
            {
                continue;
            }

            setTile(i, true);
            break;
        }

        //Flag current bomb tile as regular
        setTile(index, false);
    }
    else if(!m_hasRevealedBomb && tile.isBomb)
    {
        //Cannot be set if first tile check occurs 
        m_hasRevealedBomb = true;
    }

    m_hasRevealedTile = true;

    
    tile.isRevealed = true;
    tile.isFlagged  = false;
    tile.isDirty    = true;
    
    //Recurse function for every neighbour (flood fill)
    if(!tile.isBomb && tile.value == 0)
    {
        forEachNeighbouringIndex(index, [&](const std::size_t recursedIndex) {
            revealTile(recursedIndex);
        });
    }
}

const Tile& Board::getTile(std::size_t index) const {
    return m_tiles[index];
}

std::size_t Board::getTileCount() const {
    return static_cast<std::size_t>(m_size.x * m_size.y);
}

sf::Vector2u Board::getSize() const {
    return m_size;
}

unsigned int Board::getBombCount() const {
    return m_bombCount;
}

unsigned int Board::getFlagCount() const {
    return m_flagCount;
}

} //namespace ne