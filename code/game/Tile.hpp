#pragma once


namespace ne {



struct Tile {     
    std::uint8_t value  : 4 {0};
    
    bool isBomb         : 1 {0};
    bool isRevealed     : 1 {0};
    bool isFlagged      : 1 {0};

    //For rendering only! (mutable because it needs to be modifiable in BoardRendering)
    mutable bool isDirty : 1 {1};
};

} //namespace ne