//
//  solver.hpp
//  PegJumping
//
//  Created by Anton Logunov on 6/29/15.
//
//

#ifndef PegJumping_solver_hpp
#define PegJumping_solver_hpp

#include "board.hpp"

class Solver {
public:
    
    vector<string> Solve(Grid<char>& g) {
        board.Init(g);
        DoJumps();
        return board.Result();
    }
    
private:

    void DoJumps() {
        MovePtr ptr;
        while ((ptr = board.LongestJump())) {
            board.Jump(ptr);
        }
    }
    
    
    Board board;
};


#endif
