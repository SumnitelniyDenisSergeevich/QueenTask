#pragma once
#include <vector>
#include <string>

class Chessboard {
public:
    Chessboard(const size_t chessboard_size);
    std::string Decision(const std::pair<int, int> mandatoryQueenCoordinates);
    std::string Block5Decision(const std::pair<int, int> mandatoryQueenCoordinates);
private:
    std::vector<int> map_;
    bool has_decision_ = false;
    size_t queen_count_;
    std::vector<bool> right_diagonal_, left_diagonal_;
    size_t size_;
    size_t base_level2_;
    size_t base_level3_;
    //input copy
    std::vector<int> start_one_map_;
    size_t start_one_queen_count_;
    std::vector<bool> start_one_right_diagonal_, start_one_left_diagonal_;
    //end input copy

    //block1 copy
    std::vector<int> block1_map_;
    size_t block1_queen_count_;
    std::vector<bool> block1_right_diagonal_, block1_left_diagonal_;
    //end block1 copy

    //block3 copy
    std::vector<int> block3_map_;
    size_t block3_queen_count_;
    std::vector<bool> block3_right_diagonal_, block3_left_diagonal_;
    //end block3 copy
    

    std::string PrintMap();
    void Block1();
    bool Block2(std::vector<size_t>& cols, std::vector<size_t>& rows);
    bool Block3(std::vector<size_t>& cols, std::vector<size_t>& rows);
    bool Block4(std::vector<size_t>& cols, std::vector<size_t>& rows);
    bool Block5(std::vector<size_t>& cols, std::vector<size_t>& rows);
    void Block5Only();
    bool TrySetQueen(const size_t col, const size_t row);
    void EraseQueen(const size_t col, const size_t row);
    size_t GetFreePos(const std::vector<size_t>& cols, const std::vector<size_t>& rows) const;
    void SetInputCopy();
    void SetBlock1Copy();
    void SetBlock3Copy();
    void SetInputData();
    void SetBlock1Data();
    void SetBlock3Data();
    std::vector<size_t> GetFreeColsInRow(const size_t row, const std::vector<size_t>& cols) const;
    void SetFreeRowsCols(std::vector<size_t>& cols, std::vector<size_t>& rows) const;
    std::vector<std::pair<size_t, size_t>> GetFreeColsCountInRow(const std::vector<size_t>& cols, const std::vector<size_t>& rows) const;
    size_t GetRowsWithSmallestFreeCols(const std::vector<std::pair<size_t, size_t>> free_cols_row_id) const;
    size_t GetRandomColWithSmallestDamage(const size_t random_row_with_smallest_free_cols, const std::vector<size_t> free_cols_in_free_row, const std::vector<size_t>& cols, const std::vector<size_t>& rows);
};