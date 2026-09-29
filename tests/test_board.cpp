#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Board.hpp"

TEST_CASE("Board Initialization") {
    CHECK_THROWS_AS(Board(4), std::invalid_argument); // Size less than 6
    CHECK_THROWS_AS(Board(7), std::invalid_argument); // Size not even
    CHECK_THROWS_AS(Board(-1), std::invalid_argument); // negative size
    CHECK_NOTHROW(Board(6)); // Valid size
    CHECK_NOTHROW(Board(16)); // large Valid size

    Board b(6);
    for (size_t i=0; i < 36; i++) {
        CHECK(b.GetCell(i) == Symbol::Empty); // All cells should be empty
    }
    auto horizontalConstraints = b.GetHorizontalConstraints();
    auto verticalConstraints = b.GetVerticalConstraints();
    for (size_t i=0; i < horizontalConstraints.size(); i++) {
        CHECK(horizontalConstraints[i] == CellRelationType::None); // All horizontal constraints should be None
    }
    for (size_t i=0; i < verticalConstraints.size(); i++) {
        CHECK(verticalConstraints[i] == CellRelationType::None); // All vertical constraints should be None
    }
}

TEST_CASE("Board Cell Manipulation") {
    Board b(6);
    b.SetCell(0, Symbol::Sun);
    CHECK(b.GetCell(0) == Symbol::Sun);
    b.SetCell(1, Symbol::Moon);
    CHECK(b.GetCell(1) == Symbol::Moon);
    CHECK_NOTHROW(b.SetCell(35, Symbol::Sun)); // Last valid cell
    CHECK_THROWS_AS(b.SetCell(36, Symbol::Sun), std::invalid_argument); // Out of bounds
    CHECK_THROWS_AS(b.GetCell(36), std::invalid_argument); // Out of bounds
}

TEST_CASE("constraints Handling") {
    Board b(6);
    b.AddConstraint(0, 1, CellRelationType::Equal);
    b.AddConstraint(14, 15, CellRelationType::Equal);
    b.AddConstraint(13, 14, CellRelationType::UnEqual);
    b.AddConstraint(8, 14, CellRelationType::UnEqual);
    b.AddConstraint(14, 20, CellRelationType::UnEqual);
    b.AddConstraint(0, 6, CellRelationType::UnEqual);    
    b.AddConstraint(34, 35, CellRelationType::Equal);    
    b.AddConstraint(29, 35, CellRelationType::UnEqual);    

    auto horizontalConstraints = b.GetHorizontalConstraints();
    auto verticalConstraints = b.GetVerticalConstraints();
    SUBCASE("Check constraints in vectors") {
        CHECK(horizontalConstraints[0] == CellRelationType::Equal); //row constraint
        CHECK(horizontalConstraints[11] == CellRelationType::UnEqual); //row constraint
        CHECK(horizontalConstraints[12] == CellRelationType::Equal); //row constraint
        CHECK(horizontalConstraints[29] == CellRelationType::Equal); //row constraint
        CHECK(verticalConstraints[0] == CellRelationType::UnEqual); // column constraint
        CHECK(verticalConstraints[14] == CellRelationType::UnEqual); // column constraint
        CHECK(verticalConstraints[29] == CellRelationType::UnEqual); // column constraint
    }
    SUBCASE("Check GetConstraint") {
        CHECK(b.GetConstraint(0, 1) == CellRelationType::Equal); // first row constraint
        CHECK(b.GetConstraint(15, 14) == CellRelationType::Equal); // reverse order general row constraint
        CHECK(b.GetConstraint(35, 34) == CellRelationType::Equal); // reverse order last row constraint
        CHECK(b.GetConstraint(0, 6) == CellRelationType:: UnEqual); // first column constraint
        CHECK(b.GetConstraint(20, 14) == CellRelationType:: UnEqual); // reverse order general column constraint
        CHECK(b.GetConstraint(29, 35) == CellRelationType::UnEqual); // reverse order last column constraint
    }
    SUBCASE("Check constraints in NeighborConstraints struct") {
        auto n1 = b.GetNeighborConstraints(0);
        CHECK(n1.right == CellRelationType::Equal);
        CHECK(n1.down == CellRelationType::UnEqual);
        CHECK(n1.left == CellRelationType::None);
        CHECK(n1.up == CellRelationType::None);
        auto n2 = b.GetNeighborConstraints(14);
        CHECK(n2.left == CellRelationType::UnEqual);
        CHECK(n2.right == CellRelationType::Equal);
        CHECK(n2.up == CellRelationType::UnEqual);
        CHECK(n2.down == CellRelationType::UnEqual);
    }
    SUBCASE("Check existing constraints") {
        CHECK_THROWS_AS(b.AddConstraint(0, 1, CellRelationType::UnEqual), std::invalid_argument); // Constraint already exists (row)
        CHECK_THROWS_AS(b.AddConstraint(1, 0, CellRelationType::UnEqual), std::invalid_argument); // Constraint already exists (row reverse order)
        CHECK_THROWS_AS(b.AddConstraint(0, 6, CellRelationType::Equal), std::invalid_argument); // Constraint already exists (column)
        CHECK_THROWS_AS(b.AddConstraint(6, 0, CellRelationType::Equal), std::invalid_argument); // Constraint already exists (column reverse order)
    }
    SUBCASE("Check invalid constraints") {
        CHECK_THROWS_AS(b.AddConstraint(0, 4, CellRelationType::Equal), std::invalid_argument); // Not adjacent (row)
        CHECK_THROWS_AS(b.AddConstraint(4, 16, CellRelationType::Equal), std::invalid_argument); // Not adjacent (column)
        CHECK_THROWS_AS(b.AddConstraint(2, 16, CellRelationType::Equal), std::invalid_argument); // Not adjacent (row & column)
        CHECK_THROWS_AS(b.AddConstraint(5, 6, CellRelationType::Equal), std::invalid_argument); // Not adjacent (wrap around)
        CHECK_THROWS_AS(b.AddConstraint(35, 35, CellRelationType::Equal), std::invalid_argument); // Same cell
        CHECK_THROWS_AS(b.AddConstraint(35, 36, CellRelationType::Equal), std::invalid_argument); // Out of bounds
    }
    
}

TEST_CASE("TOSTRING") {
    Board b(6);
    b.SetCell(0, Symbol::Sun);
    b.SetCell(1, Symbol::Moon);
    b.SetCell(2, Symbol::Moon);
    b.SetCell(6, Symbol::Sun);
    b.SetCell(12, Symbol::Moon);
    b.AddConstraint(0, 1, CellRelationType::UnEqual);
    b.AddConstraint(1, 2, CellRelationType::Equal);
    b.AddConstraint(0, 6, CellRelationType::Equal);
    b.AddConstraint(6, 12, CellRelationType::UnEqual);
    std::string expected = "☀ x ☾ = ☾   _   _   _ \n=                       \n☀   _   _   _   _   _ \nx                       \n☾   _   _   _   _   _ \n                        \n_   _   _   _   _   _ \n                        \n_   _   _   _   _   _ \n                        \n_   _   _   _   _   _ \n\n";
    CHECK(b.ToString() == expected);
}