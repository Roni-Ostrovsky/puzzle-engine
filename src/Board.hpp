#pragma once
#include <utility>
#include <vector>
#include <stdexcept>
#include <cstdint>

enum class Symbol : uint8_t { Empty = 0, Sun = 1, Moon = 2 };
enum class CellRelationType { Equal, UnEqual, None };

class Board {
    private:
        std::vector<Symbol> cells;
        std::vector<CellRelationType> constraints;
        std::vector<int> constraintIndices;
        size_t size;

    public:
        Board(size_t s) : size(s) {
            if (s < 6 || s%2!=0) throw std::invalid_argument("Board size must be even and >= 6.");
            constraints = std::vector<CellRelationType>(2*s*(s-1), CellRelationType::None);
            cells = std::vector<Symbol>(s*s, Symbol::Empty);
        }
        std::vector<Symbol> GetRow(size_t r) const {
            std::vector<Symbol> row;
            for (size_t i=0;i<size;i++)
                row.push_back(cells[r*size + i]);
            return row;
        }
        std::vector<Symbol> GetColumn(size_t c) const {
            std::vector<Symbol> column;
            for (size_t i=0;i<size;i++)
                column.push_back(cells[i*size + c]);
            return column;
        }
        size_t GetSize() const { return size; }
        Symbol GetCell(size_t i) const { return cells[i]; }
        std::vector<CellRelationType> GetConstraints() const { return constraints; }
        int AddConstraint(size_t c1, size_t c2, CellRelationType t) {
            if (abs(c1-c2) != 1 && abs(c1-c2) != size) throw std::invalid_argument("Constraints can only be added between adjacent cells.");
            if(c1 < 0 || c1 >= size*size || c2 < 0 || c2 >= size*size) throw std::invalid_argument("Cell indices out of bounds.");
            if(c1 == c2) throw std::invalid_argument("Cannot add constraint between the same cell.");
            if(abs(c1%size-c2%size) > 1 || abs(c1/size-c2/size) > 1) throw std::invalid_argument("Constraints can only be added between adjacent cells.");
            
            if(abs(c1-c2) == 1) {
                if(constraints[c1*(size-1)+c2] != CellRelationType::None) throw std::invalid_argument("Constraint already exists between these cells.");
                else {
                    constraints[c1*(size-1)+c2] = t;
                    constraintIndices.push_back(c1*(size-1)+c2);
                }
            }
            else if(abs(c1-c2) == size) {
                if(constraints[size*(size-1)+c1] != CellRelationType::None) throw std::invalid_argument("Constraint already exists between these cells.");
                else {
                    constraints[size*(size-1)+c1] = t;
                    constraintIndices.push_back(size*(size-1)+c1);
                }
            }
        }
};