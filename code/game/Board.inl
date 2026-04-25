namespace ne {

template <typename Func>
void Board::forEachNeighbouringIndex(std::size_t index, Func&& func) {
    const std::size_t n = getTileCount();
    const std::size_t width = static_cast<std::size_t>(m_size.x);
    
    //State valid directions (will not error if...)
    bool isLeft = (index % width != 0);
    bool isRight = ((index + 1) % width != 0);
    bool isTop   = (index >= width);
    bool isBottom = (index + width < n);

    if(isTop) 
    {
        if(isLeft)  
        {
            func(index - width - 1);
        }

        func(index - width);
        
        if(isRight) 
        {
            func(index - width + 1);
        }
    }

    if(isLeft)  
    {
        func(index - 1);
    }

    if(isRight) 
    {
        func(index + 1);
    }

    if(isBottom) 
    {
        if(isLeft)  
        {
            func(index + width - 1);
        }
        
        func(index + width);
        
        if(isRight) 
        {
            func(index + width + 1);
        }
    }
}

template <typename Func>
void Board::forEachNeighbouringTile(std::size_t index, Func&& func) {
    forEachNeighbouringIndex(index, [&](const std::size_t index) {
        func(m_tiles[index]);
    });
}

} //namespace ne
