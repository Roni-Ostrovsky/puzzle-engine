#pragma once
#include "Board.hpp"
#include <vector>
#include <cstdint>
#include <optional>

enum class Rule : uint8_t { Triplet = 0, Equal = 1, UnEqual = 2, Balance = 3 };
enum class Orientation : uint8_t { Row = 1, Column = 0 };

struct Violation {
    Rule rule;
    Orientation orientation;
    size_t index;
    std::optional<size_t> anchorCell; //empty for balance
    //std::string message;
};

[[nodiscard]] std::optional<Violation> CheckLine(const Board& b, Orientation o, size_t index);
[[nodiscard]] std::optional<Violation> CheckBoard(const Board& b);