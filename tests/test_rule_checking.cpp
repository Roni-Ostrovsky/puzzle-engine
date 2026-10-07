#include "doctest.h"
#include "Board.hpp"
#include "rule_checking.hpp"

namespace doctest {
template <> struct StringMaker<Violation> {
    static String convert(const Violation& v) {
        std::string s = "{rule " + std::to_string(int(v.rule))
                      + ", orientation " + std::to_string(int(v.orientation))
                      + ", index " + std::to_string(v.index)
                      + ", anchor " + (v.anchorCell ? std::to_string(*v.anchorCell) : "none") + "}";
        return s.c_str();
    }
};
template <> struct StringMaker<std::optional<Violation>> {
    static String convert(const std::optional<Violation>& v) {
        return v ? StringMaker<Violation>::convert(*v) : "nullopt";
    }
};
}

Board create_board() {
    Board b(6);
    b.SetCell(1, Symbol::Moon);
    b.SetCell(4, Symbol::Moon);
    b.SetCell(6, Symbol::Moon);
    b.SetCell(8, Symbol::Moon);
    b.SetCell(9, Symbol::Sun);
    b.SetCell(12, Symbol::Sun);
    b.SetCell(14, Symbol::Sun);
    b.SetCell(15, Symbol::Moon);
    b.SetCell(16, Symbol::Moon);
    b.SetCell(18, Symbol::Moon);
    b.SetCell(20, Symbol::Sun);
    b.SetCell(21, Symbol::Moon);
    b.SetCell(23, Symbol::Sun);
    b.SetCell(25, Symbol::Sun);
    b.SetCell(26, Symbol::Moon);
    b.SetCell(27, Symbol::Sun);
    b.SetCell(29, Symbol::Moon);
    b.SetCell(30, Symbol::Sun);
    b.SetCell(31, Symbol::Sun);
    b.SetCell(33, Symbol::Moon);
    b.SetCell(34, Symbol::Sun);
    return b;
}
void valid_fill(Board& b) {
    b.SetCell(0, Symbol::Moon);
    b.SetCell(2, Symbol::Sun);
    b.SetCell(3, Symbol::Sun);
    b.SetCell(5, Symbol::Sun);
    b.SetCell(7, Symbol::Sun);
    b.SetCell(10, Symbol::Sun);
    b.SetCell(11, Symbol::Moon);
    b.SetCell(13, Symbol::Moon);
    b.SetCell(17, Symbol::Sun);
    b.SetCell(19, Symbol::Moon);
    b.SetCell(22, Symbol::Sun);
    b.SetCell(24, Symbol::Sun);
    b.SetCell(28, Symbol::Moon);
    b.SetCell(32, Symbol::Moon);
    b.SetCell(35, Symbol::Moon);
}
void fill_triplet_balance(Board& b, int c) {
    switch (c) {
        case 0: b.SetCell(17, Symbol::Moon); break; // Row 2:    Triplet, anchor 15
        case 1: {
            b.SetCell(10, Symbol::Moon); // Column 4: Triplet, anchor 4
            b.SetCell(19, Symbol::Sun);  // Column 1: Triplet, anchor 19
            break;
        }
        case 2: {
            b.SetCell(35, Symbol::Sun);  // Row 5:    Balance
            b.SetCell(3,  Symbol::Moon); // Column 3: Balance
            break;
        }
    }
}
void fill_constraint_violations(Board& b, int c) {
    switch (c) {
        case 0: {
            b.SetCell(5,  Symbol::Moon);
            b.AddConstraint(4, 5, CellRelationType::UnEqual); // Row 0:    UnEqual, anchor 4
            break;
        }
        case 1: {
            b.SetCell(0,  Symbol::Sun);
            b.AddConstraint(0, 1, CellRelationType::Equal); // Row 0:    Equal, anchor 0
            b.SetCell(2,  Symbol::Moon);
            b.AddConstraint(2, 8, CellRelationType::UnEqual); // Column 2: UnEqual, anchor 2
            b.SetCell(24, Symbol::Moon);
            b.AddConstraint(24, 30, CellRelationType::Equal); // Column 0: Equal, anchor 24
            break;
        }
    }
}
void fill_constraints(Board& b) {
    b.AddConstraint(8, 14, CellRelationType::UnEqual);
    b.AddConstraint(9, 15, CellRelationType::UnEqual);
    b.AddConstraint(9, 10, CellRelationType::Equal);
    b.AddConstraint(12, 13, CellRelationType::UnEqual);
    b.AddConstraint(13, 14, CellRelationType::UnEqual);
    b.AddConstraint(14, 20, CellRelationType::Equal);
    b.AddConstraint(15, 16, CellRelationType::Equal);
    b.AddConstraint(15, 21, CellRelationType::Equal);
    b.AddConstraint(17, 23, CellRelationType::Equal);
    b.AddConstraint(18, 19, CellRelationType::Equal);
    b.AddConstraint(22, 23, CellRelationType::Equal);
    b.AddConstraint(24, 25, CellRelationType::UnEqual);
    b.AddConstraint(25, 26, CellRelationType::UnEqual);
    b.AddConstraint(28, 29, CellRelationType::UnEqual);
    b.AddConstraint(32, 33, CellRelationType::Equal);
    b.AddConstraint(29, 35, CellRelationType::Equal);
    b.AddConstraint(30, 31, CellRelationType::Equal);
}

TEST_CASE("Triplet Checking") {
    Board b = create_board();
    SUBCASE("Violations") {
        fill_triplet_balance(b, 0);
        auto violation = CheckLine(b, Orientation::Row, 2);
        CHECK(violation == Violation({Rule::Triplet, Orientation::Row, 2, 15}));
        fill_triplet_balance(b, 1); //violation should return first error in the line
        violation = CheckLine(b, Orientation::Column, 4);
        CHECK(violation == Violation({Rule::Triplet, Orientation::Column, 4, 4}));
        violation = CheckLine(b, Orientation::Column, 1);
        CHECK(violation == Violation({Rule::Triplet, Orientation::Column, 1, 19}));
    }
    SUBCASE("No Violations") {
        auto violation = CheckBoard(b);
        CHECK(!violation.has_value()); //including empty cells
        valid_fill(b);
        violation = CheckBoard(b);
        CHECK(!violation.has_value()); //no empty cells
    }
}

TEST_CASE("Constraint Checking") {
    Board b = create_board();
    SUBCASE("Violations") {
        fill_constraint_violations(b, 0);
        auto violation = CheckLine(b, Orientation::Row, 0);
        CHECK(violation == Violation({Rule::UnEqual, Orientation::Row, 0, 4}));
        fill_constraint_violations(b, 1);
        violation = CheckLine(b, Orientation::Row, 0); //should return first error in the line
        CHECK(violation == Violation({Rule::Equal, Orientation::Row, 0, 0}));
        violation = CheckLine(b, Orientation::Column, 2);
        CHECK(violation == Violation({Rule::UnEqual, Orientation::Column, 2, 2}));
        violation = CheckLine(b, Orientation::Column, 0);
        CHECK(violation == Violation({Rule::Equal, Orientation::Column, 0, 24}));
    }
    SUBCASE("No Violations") {
        fill_constraints(b);
        auto violation = CheckBoard(b);
        CHECK(!violation.has_value());
    }
}

TEST_CASE("Balane Checking") {
    Board b = create_board();
    SUBCASE("Violations") {
        fill_triplet_balance(b, 2);
        auto violation = CheckLine(b, Orientation::Row, 5);
        CHECK(violation == Violation({Rule::Balance, Orientation::Row, 5, std::nullopt}));
        violation = CheckLine(b, Orientation::Column, 3);
        CHECK(violation == Violation({Rule::Balance, Orientation::Column, 3, std::nullopt}));
    }
    SUBCASE("No Violations") {
        auto violation = CheckBoard(b);
        CHECK(!violation.has_value()); //including empty cells
        valid_fill(b);
        violation = CheckBoard(b);
        CHECK(!violation.has_value()); //no empty cells
    }
}