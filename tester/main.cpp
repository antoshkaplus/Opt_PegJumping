//
//  main.cpp
//  CirclesSeparation
//
//  Created by Anton Logunov on 6/7/13.
//  Copyright (c) 2013 Anton Logunov. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <map>
#include <functional>

#include "util.hpp"
#include "solver.hpp"

using namespace std;
using namespace ant;

int main(int argc, const char * argv[]) {
    ifstream pr_stream("./../temp/input.txt");
    vector<Amount> peg_values;
    Grid<char> board;
    ReadProblem(pr_stream, peg_values, board);
//    PegJumping solver;
//    auto moves = solver.getMoves(peg_values, board);
    
    Solver s;
    auto moves = s.Solve(board);

    cout << moves.size() << endl;
    for (auto m : moves) {
        cout << m << endl;
    }
    cout.flush();
    return 0;
}






