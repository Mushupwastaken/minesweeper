#pragma once


namespace ne {

template <typename T>
constexpr int intFloor(const T& value) {
    int intValue = static_cast<int>(value);
    return (value < intValue)? intValue - 1 : intValue;
}

template <typename T>
constexpr int intCeil(const T& value) {
    int intValue = static_cast<int>(value);
    return (value > intValue)? intValue + 1 : intValue;
}

} //namespace ne