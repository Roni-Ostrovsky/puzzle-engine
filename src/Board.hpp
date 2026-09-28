#pragma once
#include <utility>
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <sstream>
#include <cmath>    

enum class Symbol : uint8_t { Empty = 0, Sun = 1, Moon = 2 };
enum class CellRelationType { Equal, UnEqual, None };

class Board {
    private:
        std::vector<Symbol> cells;
        std::vector<CellRelationType> horizontalConstraints;
        std::vector<CellRelationType> verticalConstraints;
        size_t size;

    public:
        Board(size_t s) : size(s) {
            if (s < 6 || s%2!=0)
                throw std::invalid_argument("Board size must be even and >= 6. (size received: " + std::to_string(s) + ")");
            
            horizontalConstraints = std::vector<CellRelationType>(s*(s-1), CellRelationType::None);
            verticalConstraints = std::vector<CellRelationType>(s*(s-1), CellRelationType::None);
            cells = std::vector<Symbol>(s*s, Symbol::Empty);
        }

        std::vector<Symbol> GetRow(size_t r) const {
            if (r >= size)
                throw std::invalid_argument("Row index out of bounds. (index received: " + std::to_string(r) + ")");
            std::vector<Symbol> row;
            for (size_t i=0;i<size;i++)
                row.push_back(cells[r*size + i]);
            return row;
        }

        std::vector<Symbol> GetColumn(size_t c) const {
            if (c >= size)
                throw std::invalid_argument("Column index out of bounds. (index received: " + std::to_string(c) + ")");

            std::vector<Symbol> column;
            for (size_t i=0;i<size;i++)
                column.push_back(cells[i*size + c]);
            return column;
        }

        size_t GetSize() const { return size; }

        Symbol GetCell(size_t i) const { 
            if (i >= size*size)
                throw std::invalid_argument("Cell index out of bounds. (index received: " + std::to_string(i) + ")");

            return cells[i];
        }
        
        void SetCell(size_t i, Symbol s) { 
            if (i >= size*size)
                throw std::invalid_argument("Cell index out of bounds. (index received: " + std::to_string(i) + ")");

            cells[i] = s;
        }

        std::vector<CellRelationType> GetHorizontalConstraints() const { return horizontalConstraints; }

        std::vector<CellRelationType> GetVerticalConstraints() const { return verticalConstraints; }

        /// @brief c1 and c2 are cell indices in the cells vector, t is the type of constraint to add
        /// @param c1 
        /// @param c2 
        /// @param t 
        /// @return none
        void AddConstraint(size_t c1, size_t c2, CellRelationType t) {
            if (c1 >= size*size || c2 >= size*size)
                throw std::invalid_argument("Cell indices out of bounds. (indices received: " + std::to_string(c1) + ", " + std::to_string(c2) + ")");
            if (c1 == c2)
                throw std::invalid_argument("Cannot add constraint between the same cell.");
            if (c1 > c2)
                std::swap(c1, c2);
            if (c2-c1 != 1 && c2-c1 != size)
                throw std::invalid_argument("Constraints can only be added between adjacent cells. (indices received: " + std::to_string(c1) + ", " + std::to_string(c2) + ")");
            if (c1%size == size-1 && c2-c1 == 1)
                throw std::invalid_argument("Constraints can only be added between adjacent cells. (indices received: " + std::to_string(c1) + ", " + std::to_string(c2) + ")");
            
            if(c2-c1 == 1) {
                if(horizontalConstraints[c1-c1/size] != CellRelationType::None)
                    throw std::invalid_argument("Constraint already exists between these cells.");
                else {
                    horizontalConstraints[c1-c1/size] = t;
                }
            }
            else if(c2-c1 == size) {
                if(verticalConstraints[c1] != CellRelationType::None)
                    throw std::invalid_argument("Constraint already exists between these cells.");
                else {
                    verticalConstraints[c1] = t;
                }
            }
        }

        std::string ToString() const {
            std::stringstream result;
            std::stringstream verticalConstraintsRow;
            for (size_t i=0;i<size*size;i++) {
                switch(cells[i]) {
                    case Symbol::Empty:
                        result << "_ ";
                    case Symbol::Sun:
                        result << "☀ ";
                    case Symbol::Moon:
                        result << "☾ ";
                }
                switch(horizontalConstraints[i-i/size]) {
                    case CellRelationType::Equal:
                        result << "= ";
                    case CellRelationType::UnEqual:
                        result << "x ";
                    case CellRelationType::None:
                        result << "  ";
                }
                if (i/size < size-1) {
                    switch(verticalConstraints[i]) {
                        case CellRelationType::Equal:
                            verticalConstraintsRow << "=   ";
                        case CellRelationType::UnEqual:
                            verticalConstraintsRow << "x   ";
                        case CellRelationType::None:
                            verticalConstraintsRow << "    ";
                    }
                }
                if (i%size == size-1) {
                    result << "\n" << verticalConstraintsRow.str() << "\n";
                    verticalConstraintsRow.str("");
                    verticalConstraintsRow.clear();
                }
            }
            return result.str();
        }
};