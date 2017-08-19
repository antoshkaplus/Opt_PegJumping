

#ifndef __PEG_JUMPING_UTIL__
#define __PEG_JUMPING_UTIL__


#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <functional>

#include "ant/core/core.hpp"
#include "ant/grid.hpp"


using namespace std;
using namespace ant;
using namespace ant::grid;


// what i want now is:
// two scores. one is best move that icreases


extern Amount current_board_value;


using Move = TrailNode<Position>;
using MovePtr = shared_ptr<Move>;



vector<string> ForwardResult(const vector<MovePtr>& moves);
vector<string> BackwardResult(const vector<MovePtr>& moves);
void ReadProblem(istream& in_stream, vector<Amount>& peg_values, Grid<char>& board);
void OutputProblem(ostream& out_stream, vector<Amount>& peg_values, Grid<char>& board);



class PegJumping {
    Grid<char> board;
    Grid<char> connected;
    array<Grid<char>, 4> conn;
    array<Grid<char>, 4> amount;
    
    Grid<char> markage;
    
    char jumper_peg_value;
    
    
    vector<int> peg_values;
    
    // coming from board size
    int N;
    
    void ComputeConnected() {
        connected.fill(false);
        auto func = [&](const Position& p) {
            auto p_h = p + Indent{0, 1};
            auto p_v = p + Indent{1, 0};
            if (board[p] == '.') return;
            if (board.isInside(p_h) && board[p_h] != '.') {
                connected[p_h] = connected[p] = true;
            }
            if (board.isInside(p_v) && board[p_v] != '.') {
                connected[p_v] = connected[p] = true;
            }
        };
        Region({0, 0}, board.size()).ForEach(func);
    }
    
    Amount BoardScore_2() {
        for_each(conn.begin(), conn.end(), [](Grid<char>& g) {
            g.fill(false);
        });
        auto func = [&](const Position p) {
            if (board[p] == '.') return;
            char old_value = board[p];
            board[p] = '.';
            for (Direction d : kDirections) {
                JumpComponent(p, d);
            }
            board[p] = old_value;
        };
        Region(0, 0, N, N).ForEach(func);
        Amount score = 0;
        auto func_2 = [&](const Position p) {
            for (auto i = 0; i < 4; ++i) {
                if (conn[i][p]) {
                    score += 1;
                    break;
                }
            } 
        };
        Region(0, 0, N, N).ForEach(func_2);
        return score;
    }
    
    Amount BoardScore_3() {
        for_each(amount.begin(), amount.end(), [](Grid<char>& g) {
            g.fill(0);
        });
        for_each(conn.begin(), conn.end(), [](Grid<char>& g) {
            g.fill(false);
        });
        auto func = [&](const Position p) {
            if (board[p] == '.') return;
            char old_value = board[p];
            board[p] = '.';
            for (Direction d : kDirections) {
                JumpAmount(p, d);
            }
            board[p] = old_value;
        };
        Region(0, 0, N, N).ForEach(func);
        Amount max = 0;
        auto func_2 = [&](const Position p) {
            for (auto i = 0; i < 4; ++i) {
                if (amount[i][p] > max) {
                    max = amount[i][p];
                }
            } 
        };
        Region(0, 0, N, N).ForEach(func_2);
        return max;
    }
    
    Amount BoardScore() {
        ComputeConnected();
        Amount res = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                // need to consider corners too
                // should be named as unit vector
                for (auto k_0 = 0; k_0 < 4; ++k_0) {
                    for (auto k_1 = k_0+1; k_1 < 4; ++k_1) {
                        auto d_0 = kDirVector[k_0];
                        auto d_1 = kDirVector[k_1];
                        if (d_0 != d_1) continue;
                        Position p = {i, j};
                        if (board[p] != '.') continue;
                        auto p_0 = p + d_0;
                        auto p_1 = p + d_1;
                        if (board.isInside(p_0) && board.isInside(p_1) && 
                            board[p_0] != '.' && board[p_1] != '.' &&
                            connected[p_0] && connected[p_1]) {
                            
                            res += 1;
                        }
                    }
                }
            }
        }
        return res;
    }
    
    
    struct Score {
        Amount total_value = 0;
        Count count = 0;
        Amount board_value = 0;
        
        // we strike for more
        bool operator<(const Score& s) const {
            if (current_board_value < board_value || current_board_value < s.board_value) {
                return board_value < s.board_value;
            } else {
                return total_value*count < s.total_value*s.count;
            }
            //return (count == s.count && board_value < s.board_value) || total_value*count < s.total_value*s.count;
        }
    };
    
    
    void CheckMoveValidity(MovePtr m) {
        if (!m) return;
        while (m->previous) {
            //auto ind = m->value - m->previous->value;
            //assert(abs(ind.row + ind.col) == 2);
            m = m->previous;
        }
    }
    
    void CheckJumps(MovePtr m) {
        assert(m->previous);
    }
    
    void JumpComponent(const Position& pos, Direction dir) {
        auto peg_pos = pos + kDirVector[dir];
        if (!board.isInside(peg_pos) || board[peg_pos] == '.') {
            return;
        } 
        auto new_pos = peg_pos + kDirVector[dir];
        if (!board.isInside(new_pos) || board[new_pos] != '.') {
            return;   
        }
        if (conn[dir][peg_pos]) return;
        conn[dir][peg_pos] = true;
        char old_value = board[peg_pos];  
        board[peg_pos] = '.';
        for (Direction d : kDirections) {
            JumpComponent(new_pos, d);
        }
        board[peg_pos] = old_value;
    }
    
    
    // returns max amount found
    Amount JumpAmount(const Position& pos, Direction dir) {
        auto peg_pos = pos + kDirVector[dir];
        if (!board.isInside(peg_pos) || board[peg_pos] == '.') {
            return 0;
        } 
        auto new_pos = peg_pos + kDirVector[dir];
        if (!board.isInside(new_pos) || board[new_pos] != '.') {
            return 0;   
        }
        if (conn[dir][peg_pos]) return this->amount[dir][peg_pos];
        Amount m, max = 0;
        conn[dir][peg_pos] = true;
        char old_value = board[peg_pos];  
        board[peg_pos] = '.';
        for (Direction d : kDirections) {
             m = JumpAmount(new_pos, d) + 1; // +1 for previous element
             if (m > max) max = m;
        }
        this->amount[dir][peg_pos] = max;
        board[peg_pos] = old_value;
        return max;
    }

    
    
    // watch for bounds for this one need extra or check
    Score Jump(const Position& pos, Direction dir, MovePtr& m) {
        Score best_s;
        auto peg_pos = pos + kDirVector[dir];
        if (!board.isInside(peg_pos) || board[peg_pos] == '.') {
            board[pos] = jumper_peg_value;
            best_s.board_value = BoardScore_3();
            board[pos] = '.';
            return best_s;
        } 
        auto new_pos = peg_pos + kDirVector[dir];
        if (!board.isInside(new_pos) || board[new_pos] != '.') {
            board[pos] = jumper_peg_value;
            best_s.board_value = BoardScore_3();
            board[pos] = '.';
            return best_s;   
        }
        // best move
        m = MovePtr(new Move(new_pos, m));
        CheckMoveValidity(m);
        MovePtr best = m;
        char old_value = board[peg_pos];  
        board[peg_pos] = '.';
        for (Direction d : kDirections) {
            MovePtr cur = m;
            Score s = Jump(new_pos, d, cur);
            if (best_s < s) {
                best = cur;
                best_s = s;  
            }
        }
        m = best;
        best_s.count += 1;
        best_s.total_value += peg_values[old_value - '0'];
        board[peg_pos] = old_value;
        return best_s;
    }
    
    MovePtr BestJump() {
        MovePtr best_m, m;
        Score best_s, s;
        current_board_value = BoardScore_3();
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                // try to jump with this peg for a lot of times
                Position p{i, j};
                if (board[p] == '.') continue;
                char old_value = jumper_peg_value = board[p];
                board[p] = '.';
                for (Direction d : kDirections) {
                    //if (d == kDirUp || d == kDirLeft) continue;
                    m = MovePtr(new Move(p, nullptr));
                    s = Jump(p, d, m);
                    if (best_s < s && s.count > 0) {
                        best_s = s;
                        best_m = m;
                    }
                }
                board[p] = old_value;
            }
        }
        return best_m;
    }
        
    vector<MovePtr> res;
    
    
    vector<MovePtr> DoJumps() {
        MovePtr ptr;
        while ((ptr = BestJump())) {
            // need to remove some pegs
            CheckJumps(ptr);
            Jump(ptr);
            res.push_back(ptr);
        }
        return res;
    }
    
    // first element is starting move
    MovePtr ConstructMoveChain(const vector<Position>& ps) {
        MovePtr m;
        for (auto& p : ps) {
            m = MovePtr{new Move(p, m)};
        }
        return m;
    }
    
    
    
    // can return different ways of doing this shit
    void ExtendMove(MovePtr& m) {
        MarkPath(m);
        Position p = m->value;
        for (Direction d : kDirections) {
            Position peg = p + kDirVector[d];
            if (board[peg] == '.') continue;
            Position need_empty = peg + kDirVector[d];
            if (board[need_empty] == '.' || markage[need_empty]) continue; 
            Direction dd_0 = kDirTurn[d][0];
            Direction dd_1 = kDirTurn[d][1];
            Position p_0 = need_empty + kDirVector[dd_0];
            Position p_1 = need_empty + kDirVector[dd_1];
            if (markage[p_0] || markage[p_1]) continue; // unable to progress
            
            if (board[p_0] == '.' && board[p_1] != '.') {
                Jump(ConstructMoveChain({p_1, p_0})); 
            } else if (board[p_0] != '.' && board[p_1] != '.') {
                Jump(ConstructMoveChain({p_0, p_1}));
            } else {
                continue;
            }
            UnmarkPath(m);
            m = MovePtr(new Move(need_empty, m));
            return;
        }
        UnmarkPath(m);
    }
    
    void Jump(MovePtr m) {
        MovePtr cur = m;
        Position end = cur->value;
        while (cur->previous) {
            auto peg_position = Centroid(cur->value, cur->previous->value); 
            assert(board[peg_position] != '.');
            board[peg_position] = '.';
            cur = cur->previous;
        }
        board[end] = board[cur->value];
        board[cur->value] = '.';
    }

    
    
    void MarkPath(MovePtr m) {
        markage[m->value] = true;
        while (m->previous) {
            markage[Centroid(m->value, m->previous->value)] = true;
            markage[m->previous->value] = true;
            m = m->previous;
        }
    }
    
    void UnmarkPath(MovePtr m) {
        markage[m->value] = false;
        while (m->previous) {
            markage[Centroid(m->value, m->previous->value)] = false;
            markage[m->previous->value] = false;
            m = m->previous;
        }
    }
    
public:
    
    vector<string> getMoves(vector<int>& peg_values, Grid<char> board) {
        N = board.size().row;
        connected.resize(N, N);
        
        markage.resize(N, N);
        markage.fill(false);
        
        fill(conn.begin(), conn.end(), Grid<char>(N, N));
        fill(amount.begin(), amount.end(), Grid<char>(N, N));
        this->board = board;
        this->peg_values = peg_values;
        return BackwardResult(DoJumps());
    }
    
};



#endif


