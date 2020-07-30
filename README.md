# Stable Roommate
This repository contains C++ codes for various functionalities related to the stable roommate problem.
This Project was co-authored by https://github.com/raghuraman2000.

## Requirements
g++ Compiler for C++.

## Solver
- Using the solver compile the solver with the command

         g++ Stable_Roommate_solver.cpp -std=c++11
- To execute the solver use the command

        ./a.out <name_of_input_file>.txt <name_of_output_file>.txt
- The input file must be like the one shown in Sample_test_case.txt and the output file must be empty
- If there exists a stable matching the solver fills the output file with a possible matching otherwise it says no matching is possible

## Checker
- Given a preference list and a solution the code check if the solution is indeed a valid solution
- Using the solver compile the solver with the command

         g++ Stable_Roommate_checker.cpp -std=c++11
- To execute the solver use the command

        ./a.out <name_of_preference_list_file>.txt <name_of_matching_file>.txt
        
## Test Generators
Its a sequence of codes which generate different kinds of input:-
- An input which guarantees O(n<sup>2</sup>) time for phase 1
- An input which guarantees O(n<sup>2</sup>) time for phase 2
- A random input with complete preference lists
- A random input with incomplete preference lists

## Time Complexity Analysis
A time complexity analysis was done with the random complete input set. Let N be the input set size. The y-axis denotes the N<sup>2</sup>logN and the x-axis denotes the time taken by the algorithm by a randomly generated input. The choice of the y-axis was based on the expected complexity as can be seen in the figure a complexity of N<sup>2</sup>logN is accomplished as the plot is linear with time.
<p align="center">
  <img width="460" height="300" src="https://github.com/Arnhav-Datar/StableRoommate/blob/master/stable.jpeg">
</p>
