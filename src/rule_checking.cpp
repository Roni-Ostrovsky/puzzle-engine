#include "Board.hpp"
#include "rule_checking.hpp"
#include <vector>
#include <cstdint>
#include <optional>

/// @brief Checks a line (row or column) for rule violations
/// @param b The board to check
/// @param o The orientation of the line (Row or Column)
/// @param index The index of the line to check
/// @return The first violation found, or std::nullopt if no violations are found
[[nodiscard]] std::optional<Violation> CheckLine(const Board& b, Orientation o, size_t index) {
    size_t step = (o == Orientation::Row) ? 1 : b.GetSize();
    size_t start = (o == Orientation::Row) ? index*b.GetSize() : index;
    Symbol twoPrev = Symbol::Empty;
    Symbol prev = Symbol::Empty;
    size_t countSun = 0;
    size_t countMoon = 0;
    for (size_t i=0;i<b.GetSize();i++) {
        CellRelationType constraint = CellRelationType::None;
        Symbol current = b.GetCell(start + i*step);
        if(i>0) {
            constraint = b.GetConstraint(start + i*step, start + (i-1)*step);
        }
        // Check for constraint violation
        switch (constraint) {
            case CellRelationType::Equal:
                if (prev != Symbol::Empty && current != Symbol::Empty && current != prev) {
                    return Violation{Rule::Equal, o, index, start+(i-1)*step};
                }
                break;
            case CellRelationType::UnEqual:
                if (prev != Symbol::Empty && current == prev) {
                    return Violation{Rule::UnEqual, o, index, start+(i-1)*step};
                }
                break;
            case CellRelationType::None:
                break;
        } 
        // Check for triplet violation
        if (twoPrev != Symbol::Empty && current == twoPrev && twoPrev == prev) {
            return Violation{Rule::Triplet, o, index, start+(i-2)*step};
        }
        //Update counts
        twoPrev = prev;
        prev = current;
        switch (prev) {
            case Symbol::Empty:
                break;
            case Symbol::Sun:
                countSun++;
                break;
            case Symbol::Moon:
                countMoon++;
                break;
        }
    }
    // Check for balance violation
    if (countSun > b.GetSize()/2 || countMoon > b.GetSize()/2) {
        return Violation{Rule::Balance, o, index, std::nullopt};
    }
    return std::nullopt;
}

/// @brief Checks the entire board for rule violations
/// @param b The board to check
/// @return The first violation found, or std::nullopt if no violations are found
[[nodiscard]] std::optional<Violation> CheckBoard(const Board& b) {
    for (size_t i=0;i<b.GetSize();i++) {
        if (auto v = CheckLine(b, Orientation::Row, i))
            return v;
        if (auto v = CheckLine(b, Orientation::Column, i))
            return v;
    }
    return std::nullopt;
}