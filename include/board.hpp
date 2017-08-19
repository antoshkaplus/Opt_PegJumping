//
//  board.hpp
//  PegJumping
//
//  Created by Anton Logunov on 6/29/15.
//
//

#ifndef PegJumping_board_hpp
#define PegJumping_board_hpp

#include "util.hpp"


struct Board {

    void Init(Grid<char>& b) {
        N = b.size().row;
        g = b;
        count.resize(b.size());
        moves.resize(b.size());
        jumps.clear();
    }

    Size Size() const {
        return g.size();
    }
    
    bool IsEmpty(const Position& p) const {
        return g[p] == '.';
    }
    
    void BackwardJump(MovePtr m) {
        MovePtr cur = m;
        Position end = cur->value;
        while (cur->previous) {
            auto peg_position = Centroid(cur->value, cur->previous->value); 
            assert(g[peg_position] != '.');
            g[peg_position] = '.';
            cur = cur->previous;
        }
        g[end] = g[cur->value];
        g[cur->value] = '.';
    }
    
    void ForwardJump(MovePtr m) {
        MovePtr cur = m;
        Position end = cur->value;
        while (cur->previous) {
            auto peg_position = Centroid(cur->value, cur->previous->value); 
            assert(g[peg_position] != '.');
            g[peg_position] = '.';
            cur = cur->previous;
        }
        g[cur->value] = g[end];
        g[end] = '.';
    }
    
    // returns max amount found
    void LongestJump(const Position& pos, Direction dir) {
        auto peg_pos = pos + kDirVector[dir];
        if (!g.isInside(peg_pos) || IsEmpty(peg_pos)) {
            return;
        } 
        auto new_pos = peg_pos + kDirVector[dir];
        if (!g.isInside(new_pos) || !IsEmpty(new_pos)) {
            return;   
        }
        if (count[new_pos][dir] > 0) return;
        MovePtr p_max;
        Count c_max = 0;;
        char old_value = g[peg_pos];  
        g[peg_pos] = '.';
        for (Direction d : kDirections) {
            LongestJump(new_pos, d); // +1 for previous element
            if (!p_max || count[new_pos][d] > c_max) {
                p_max = moves[new_pos][d];
                c_max = count[new_pos][d];
            }
        }
        count[new_pos][dir] = c_max;
        p_max = MovePtr(new Move(new_pos, p_max));
        moves[new_pos][dir] = p_max;
        g[peg_pos] = old_value;
    }
    
    void Jump(MovePtr p) {
        ForwardJump(p);
    }
    
    // need to prepare count probably...
    MovePtr LongestJump() {
        MovePtr best_m, m;
        Count c_max = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                // try to jump with this peg for a lot of times
                Position p{i, j};
                if (IsEmpty(p)) continue;
                char old_value = g[p];
                g[p] = '.';
                for (Direction d : kDirections) {
                    LongestJump(p, d);
                    if (count[p][d] > c_max) {
                        best_m = MovePtr(new Move(p, moves[p][d]));
                    }
                }
                g[p] = old_value;
            }
        }
        return best_m;
    }
    
    vector<string> Result() {
        return ForwardResult(jumps);
    }
    
private:
    Count N;
    Grid<char> g;
    // maybe count already found at this direction for 
    // current position
    Grid<array<Count, kDirCount>> count;
    Grid<array<MovePtr, kDirCount>> moves;
    // don't really want user to care about it
    vector<MovePtr> jumps;

};



#endif
