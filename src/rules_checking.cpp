#include "Board.hpp"
#include "rules_checking.hpp"
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
    size_t cell = (o == Orientation::Row) ? index*b.GetSize() : index;
    Symbol first = Symbol::Empty;
    Symbol second = Symbol::Empty;
    size_t countSun = 0;
    size_t countMoon = 0;
    for (size_t i=0;i<b.GetSize();i++) {
        CellRelationType constraint = CellRelationType::None;
        if(i>1) {
            constraint = b.GetConstraint(cell + i*step, cell + (i-1)*step);
        }
        // Check for constraint violation
        switch (constraint) {
            case CellRelationType::Equal:
                if (second != Symbol::Empty && b.GetCell(cell + i*step) != second) {
                    return Violation{Rule::Equal, o, index, cell+(i-1)*step};
                }
                break;
            case CellRelationType::UnEqual:
                if (second != Symbol::Empty && b.GetCell(cell + i*step) == second) {
                    return Violation{Rule::UnEqual, o, index, cell+(i-1)*step};
                }
                break;
            case CellRelationType::None:
                break;
        } 
        // Check for triplet violation
        if (first != Symbol::Empty && b.GetCell(cell + i*step) == first && first == second) {
            return Violation{Rule::Triplet, o, index, cell+(i-2)*step};
        }
        //Update counts
        first = second;
        second = b.GetCell(cell + i*step);
        switch (second) {
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
        std::optional<Violation> v = CheckLine(b, Orientation::Row, i);
        if (v.has_value())
            return v;
        v = CheckLine(b, Orientation::Column, i);
        if (v.has_value())
            return v;
    }
    return std::nullopt;
}