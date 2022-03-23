
#include "chess_board.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <map>

using namespace std;

void ShuffleVector(std::vector<size_t>& v) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(v.begin(), v.end(), g);
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
};

void Chessboard::EraseQueen(size_t col, size_t row) {
    map_[col] = -1;
    right_diagonal_[col + row] = false;
    left_diagonal_[size_ + col - row - 1] = false;
    --queen_count_;
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

void Chessboard::SetBlock3Copy() {
    block3_map_ = map_;
    block3_queen_count_ = queen_count_;
    block3_right_diagonal_ = right_diagonal_;
    block3_left_diagonal_ = left_diagonal_;
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
    cols.clear();
    rows.clear();
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


void Chessboard::SetBlock3Data() {
    map_ = block3_map_;
    queen_count_ = block3_queen_count_;
    right_diagonal_ = block3_right_diagonal_;
    left_diagonal_ = block3_left_diagonal_;
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
    bool has_decision;
    size_t beak_treak_count = 0;
    do {
        ++beak_treak_count;
        if (beak_treak_count > 100) {
            cout << "has_decision_ = false" << endl;
            has_decision_ = false;
            break;
        }
        do {
            has_decision = true;
            SetInputData();
            free_rows = free_rows_copy;
            free_cols = free_cols_copy;
            ShuffleVector(free_rows);
            ShuffleVector(free_cols);
            size_t while_count = 0;
            while (queen_count_ < base_level2_) {
                if (while_count > 1000) {
                    has_decision = false;
                    break;
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
        } while (!has_decision); // пока не будет решения, отсюда не выйдем, может получиться бесконечный цикл?
    } while (!Block2(free_cols, free_rows));
}

bool Chessboard::Block2(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    SetBlock1Copy();
    return Block3(cols, rows);
}

bool Chessboard::Block3(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    vector<size_t> free_rows_copy = rows;
    vector<size_t> free_cols_copy = cols;
    size_t beak_treak_count = 0;
    do {
        ++beak_treak_count;
        if (beak_treak_count > 50) {
            return false;
        }
        SetBlock1Data();
        rows = free_rows_copy;
        cols = free_cols_copy;
        ShuffleVector(rows);
        ShuffleVector(cols);
        size_t while_count = 0;
        while (queen_count_ < base_level3_) {
            if (while_count > 50) {
                return false;
            }
            ++while_count;
            size_t random_free_row = rows.at(std::rand() % rows.size());
            vector<size_t> free_cols_in_free_row = GetFreeColsInRow(random_free_row, cols);
            if (!free_cols_in_free_row.size()) {
                return false;
            }
            size_t random_free_col = free_cols_in_free_row.at(std::rand() % free_cols_in_free_row.size());
            TrySetQueen(random_free_col, random_free_row);
            rows.erase(find(rows.begin(), rows.end(), random_free_row));
            cols.erase(find(cols.begin(), cols.end(), random_free_col));
        }
    } while (!Block4(cols, rows));
    return true;
}

bool Chessboard::Block4(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    SetBlock3Copy();
    return Block5(cols, rows);
}

bool Chessboard::Block5(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    vector<size_t> free_rows_copy = rows;
    vector<size_t> free_cols_copy = cols;
    bool has_decision;
    size_t beak_treak_count = 0;
    do {
        if (beak_treak_count > 10) {
            return false;
        }
        has_decision = true;
        ++beak_treak_count;
        rows = free_rows_copy;
        cols = free_cols_copy;
        SetBlock3Data();
        while (queen_count_ < size_) {
            vector<pair<size_t, size_t>> free_cols_row_id = GetFreeColsCountInRow(cols, rows);
            if (!free_cols_row_id.begin()->first) {
                has_decision = false;
                break;
            }
            size_t random_row_with_smallest_free_cols = GetRowsWithSmallestFreeCols(free_cols_row_id);; // тут я получил строку, теперь надо получить col с наименьшими потерями

            vector<size_t> free_cols_in_free_row = GetFreeColsInRow(random_row_with_smallest_free_cols, cols);

            size_t random_col_with_smallest_damage = GetRandomColWithSmallestDamage(random_row_with_smallest_free_cols, free_cols_in_free_row, cols, rows);
            TrySetQueen(random_col_with_smallest_damage, random_row_with_smallest_free_cols);
            rows.erase(find(rows.begin(), rows.end(), random_row_with_smallest_free_cols));
            cols.erase(find(cols.begin(), cols.end(), random_col_with_smallest_damage));
        }
    } while (!has_decision);
    return true;
}

size_t Chessboard::GetFreePos(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    size_t count = 0;
    for (size_t i = 0; i < cols.size(); ++i) {
        for (size_t j = 0; j < rows.size(); ++j) {
            if (!right_diagonal_[cols[i] + rows[j]] && !left_diagonal_[size_ + cols[i] - rows[j] - 1]) {
                ++count;
            }
        }
    }
    return count;
}

size_t Chessboard::GetRandomColWithSmallestDamage(size_t random_row_with_smallest_free_cols, std::vector<size_t> free_cols_in_free_row, std::vector<size_t>& cols, std::vector<size_t>& rows) {
    vector<pair<size_t, size_t>> damage_col;
    for (const size_t col : free_cols_in_free_row) {
        size_t count_befor_insert = GetFreePos(cols, rows);
        TrySetQueen(col, random_row_with_smallest_free_cols);
        size_t count_after_insert = GetFreePos(cols, rows);
        damage_col.push_back(make_pair(count_befor_insert - count_after_insert, col));
        EraseQueen(col, random_row_with_smallest_free_cols);
    }
    std::sort(damage_col.begin(), damage_col.end());
    vector<size_t> index_with_smallest_damage;
    index_with_smallest_damage.push_back(damage_col.begin()->second);
    for (size_t i = 1; i < damage_col.size(); ++i) {
        if (i > 2) {
            break;
        }
        if (damage_col.begin()->first == damage_col.at(i).first) {
            index_with_smallest_damage.push_back(damage_col.at(i).second);
        }
    }
    return damage_col.at(rand() % damage_col.size()).second;
}

size_t Chessboard::GetRowsWithSmallestFreeCols(std::vector<std::pair<size_t, size_t>> free_cols_row_id) {
    vector<size_t> rows_with_smallest_free_cols = { free_cols_row_id.begin()->second };
    for (size_t i = 1; i < free_cols_row_id.size(); ++i) {
        if (i > 2) {
            break;
        }
        if (free_cols_row_id.begin()->first == free_cols_row_id.at(i).first) {
            rows_with_smallest_free_cols.push_back(free_cols_row_id.at(i).second);
        }
    }
    return rows_with_smallest_free_cols.at(rand() % rows_with_smallest_free_cols.size());
}

std::vector<std::pair<size_t, size_t>> Chessboard::GetFreeColsCountInRow(std::vector<size_t>& cols, std::vector<size_t>& rows) {
    vector<pair<size_t, size_t>> result;
    for (const size_t row : rows) {
        result.push_back(make_pair(0, row));
        for (const size_t col : cols) {
            if (!right_diagonal_[col + row] && !left_diagonal_[size_ + col - row - 1]) {
                ++result.back().first;
            }
        }
    }
    std::sort(result.begin(), result.end());
    return result;
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