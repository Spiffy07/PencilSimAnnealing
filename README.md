# PencilSimAnnealing
This program was designed to simulate assigning workers (Dealer) to a assignment (Table). The role of assigning was refered to as the "Pencil". 
Each Dealer has a list of games that they are qualified to deal and each table will have exactly one game assigned.

In this C++ program, Simulated annealing was used to iterate numerous cycles to attempt to find the optimal assignments based on the available pool of dealers to the list of tables.
the following concepts were implemented in this project:
- Simulated annealing
- Multi threading using the <async> library (std::thread was originally used but <async> had better performance)
- Benchmarking using custom timer classes and <ofstream> to output data to be used with 3rd party assessment tools
- and more

The goal of this program was to use the concept of performant C++ code to simulate a common, but important, task while exploring new C++ concepts and keeping optimal performance.
The original working prototype began as a single thread performing Simulated Annealing to finish one optimal outcome.
This took around 23 seconds on average.

After refactoring the use of <vector>s in favor of std::array containers, The number of heap-allocations were drastically reduced.
I also added multi-threading to generate 8 threads to compute 8 different optimal outputs concurrently. 
This program currently outputs 8 possible optimals (though probably not best due to the nature of simulated annealing) in around than 1.4 seconds after pre-compiler optimizations.
