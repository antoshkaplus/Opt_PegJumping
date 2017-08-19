
#include "util.hpp"


Amount current_board_value;

// first element first
vector<string> ForwardResult(const vector<MovePtr>& moves) {
    vector<string> res;
    for (auto j : moves) {
        string dirs;
        MovePtr m = j;
        Position start = m->value;
        while (m->previous) {
            auto dp = (m->previous->value - m->value);
            char D;
            if (dp.row == 0) {
                if (dp.col > 0) {
                    D = 'R';
                } 
                if (dp.col < 0) {
                    D = 'L';
                }
            } 
            if (dp.col == 0) {
                if (dp.row > 0) {
                    D = 'D';
                } 
                if (dp.row < 0) {
                    D = 'U';
                }
            }
            dirs = dirs + D;
            // could use if statement
            m = m->previous;
        }
        res.push_back(to_string(start.row) + " " 
                      + to_string(start.col) + " "
                      + dirs);
    }
    return res;
}

// last element first
vector<string> BackwardResult(const vector<MovePtr>& moves) {
    vector<string> res;
    for (auto j : moves) {
        
        string dirs;
        Position start;
        MovePtr m = j;
        while (m->previous) {
            auto dp = (m->value - m->previous->value);
            char D;
            if (dp.row == 0) {
                if (dp.col > 0) {
                    D = 'R';
                } 
                if (dp.col < 0) {
                    D = 'L';
                }
            } 
            if (dp.col == 0) {
                if (dp.row > 0) {
                    D = 'D';
                } 
                if (dp.row < 0) {
                    D = 'U';
                }
            }
            dirs = D + dirs;
            m = m->previous;
        }
        start = m->value;
        res.push_back(to_string(start.row) + " " 
                      + to_string(start.col) + " "
                      + dirs);
    }
    return res;
}


void ReadProblem(istream& in_stream, vector<Amount>& peg_values, Grid<char>& board) {
    Count M;
    in_stream >> M;
    peg_values.resize(M);
    for (int i = 0; i < M; ++i) {
        in_stream >> peg_values[i];
    }
    Count N;
    in_stream >> N;
    board.resize(N, N);
    string b;
    auto b_it = board.begin();
    for (Index i = 0; i < N; ++i) { 
        in_stream >> b;
        copy_n(b.begin(), N, b_it);
        b_it += N;
    }
}

void OutputProblem(ostream& out_stream, vector<Amount>& peg_values, Grid<char>& board) {
    out_stream << peg_values.size() << endl;
    for (auto p : peg_values) {
        out_stream << p << " ";
    }
    out_stream << endl;
    Count N = board.size().row;
    out_stream << N << endl;
    for (Index i = 0; i < N; ++i) {
        for (Index j = 0; j < N; ++j) {
            out_stream << board(i, j);
        }
        out_stream << endl;
    }
}
