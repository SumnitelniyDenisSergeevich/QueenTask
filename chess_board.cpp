
#include "chess_board.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>


using namespace std;

void ShuffleVector(std::vector<size_t>& v) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
}

Chessboard::Chessboard(size_t size) : map_(size, -1) {
    queen_count_ = 0;
    size_ = size;
    right_diagonal_.resize(size * 2 - 1);
    left_diagonal_.resize(size * 2 - 1);
    double u = log10(size_);
    if (size_ < 30000) {
        base_level2_ = size_ - round(12.749568 * pow(u, 3) - 46.535838 * u * u + 120.011829 * u - 89.600272);
        base_level3_ = size_ - round(9.717958 * pow(u, 3) - 46.144187 * u * u + 101.296409 * u - 50.669273);
    }
    else {
        base_level2_ = size_ - round(-0.886344 * pow(u, 3) + 56.136743 * u * u + 146.486415 * u + 227.967782);
        base_level3_ = size_ - round(14.959815 * pow(u, 3) - 253.661725 * u * u + 1584.713376 * u - 3060.691342);
    }
}

void Chessboard::SetInputCopy() {
    start_one_map_ = map_;
    start_one_queen_count_ = queen_count_;
    start_one_right_diagonal_ = right_diagonal_;
    start_one_left_diagonal_ = left_diagonal_;
}

void Chessboard::SetBlock1Copy() {
    block1_map_ = map_;
    block1_queen_count_ = queen_count_;
    block1_right_diagonal_ = right_diagonal_;
    block1_left_diagonal_ = left_diagonal_;
}

string Chessboard::Decision(std::pair<int, int> queen_coord) {
    map_[queen_coord.first] = queen_coord.second;
    right_diagonal_[queen_coord.first + queen_coord.second] = true;
    left_diagonal_[size_ + queen_coord.first - queen_coord.second - 1] = true;
    ++queen_count_;
    SetInputCopy();
    Block1();
    return PrintMap();
}

void Chessboard::SetFreeRowsCols(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    for (size_t i = 0; i < map_.size(); ++i) {
        rows.push_back(i);
    }
    for (size_t i = 0; i < map_.size(); ++i) {
        if (map_.at(i) == -1) {
            cols.push_back(i);
        }
        else {
            rows.erase(find(rows.begin(), rows.end(), map_.at(i)));
        }
    }
}

void Chessboard::SetInputData() {
    map_ = start_one_map_;
    queen_count_ = start_one_queen_count_;
    right_diagonal_ = start_one_right_diagonal_;
    left_diagonal_ = start_one_left_diagonal_;
}


void Chessboard::SetBlock1Data() {
    map_ = block1_map_;
    queen_count_ = block1_queen_count_;
    right_diagonal_ = block1_right_diagonal_;
    left_diagonal_ = block1_left_diagonal_;
}

bool Chessboard::FreeHasDecision(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    for (size_t i = 0; i < cols.size(); ++i) {
        for (size_t j = 0; j < rows.size(); ++j) {
            if (!right_diagonal_[cols[i] + rows[j]] && !left_diagonal_[size_ + cols[i] - rows[j] - 1]) {
                return true;
            }
        }
    }
    return false;
}

void Chessboard::Block1() {
    vector<size_t> free_rows;
    vector<size_t> free_cols;
    SetFreeRowsCols(free_cols, free_rows);
    vector<size_t> free_rows_copy = free_rows;
    vector<size_t> free_cols_copy = free_cols;
    ShuffleVector(free_rows);
    ShuffleVector(free_cols);

    bool has_decision = true;

    size_t while_count = 0;
    while (queen_count_ <  base_level2_) {
        if (while_count > 1000) {
            has_decision = false;
        }
        if (!FreeHasDecision(free_cols, free_rows)) {
            free_rows = free_rows_copy;
            free_cols = free_cols_copy;
            ShuffleVector(free_rows);
            ShuffleVector(free_cols);
            SetInputData();
        }
        ++while_count;
        for (int i = free_rows.size() - 1; i > 0; --i) {
            if (TrySetQueen(free_cols.at(i), free_rows.at(i))) {
                free_cols.erase(free_cols.begin() + i);
                free_rows.erase(free_rows.begin() + i);
            }
        }
        ShuffleVector(free_rows);
        ShuffleVector(free_cols);
    }
    if (has_decision) {
        Block2(free_cols, free_rows);
    }
}

void Chessboard::Block2(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    SetBlock1Copy();
    Block3(cols, rows);
}

void Chessboard::Block3(std::vector<size_t>& cols, std::vector<size_t>& rows) {

    vector<size_t> free_rows_copy = rows;
    vector<size_t> free_cols_copy = cols;
    while (queen_count_ <  25/*base_level3_*/) {
        size_t random_free_row = rows.at(std::rand() % rows.size());
        vector<size_t> free_cols_in_free_row = GetFreeColsInRow(random_free_row, cols); // проверка на пустоту !!!
        if (!free_cols_in_free_row.size()) {
            cout << "empty" << endl;
            system("pause");
        }
        size_t random_free_col = free_cols_in_free_row.at( std::rand() % free_cols_in_free_row.size() );
        if (!TrySetQueen(random_free_col, random_free_row)) {
            cout << "error" << endl;
            system("pause");
        }
        rows.erase(find(rows.begin(), rows.end(), random_free_row));
        cols.erase(find(cols.begin(), cols.end(), random_free_col));
    }
}

std::vector<size_t> Chessboard::GetFreeColsInRow(size_t row, vector<size_t>& cols) {
    vector<size_t> result;
    for (const size_t col : cols) {
        if (!right_diagonal_[col + row] && !left_diagonal_[size_ + col - row - 1]) {
            result.push_back(col);
        }
    }
    return result;
}

bool Chessboard::TrySetQueen(size_t col, size_t row) {
    if (!right_diagonal_[col + row] && !left_diagonal_[size_ + col - row - 1]) {
        map_[col] = row;
        right_diagonal_[col + row] = true;
        left_diagonal_[size_ + col - row - 1] = true;
        ++queen_count_;
        return true;
    }
    return false;
}

std::string Chessboard::PrintMap() {
    string result = "";
    has_decision_ = true;
    bool b = false;
    if (has_decision_) {
        for (const int ferz : map_) {
            if (b) {
                result += '\n';
            }
            b = true;
            if (ferz == -1) {
                result += std::string(size_, '!');
                continue;
            }
            result += std::string(ferz, ',') + "Q"s + std::string(size_ - ferz - 1, ',');
        }
    }
    return result;
}