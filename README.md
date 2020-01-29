# n-puzzle solver

This C++ program solves the n-queen problem using bit-patterns, recursion and threads.

## Usage

Compilation:

```bash
g++ nqueen-parallel.cpp -o nqueen-parallel.exe -pthread -std=c++11
```

Run:

```bash
./nqueen-parallel.exe <queens_n> <max_threads> <seq_threshold>
```
Where:


| parameter     | description                                             |
| ------------- | ------------------------------------------------------- |
| queens_n      | number of queens                                        |
| max_threads   | maximum number of threads                               |
| seq_threshold | the point where the program switches to sequential mode |

Example:

```bash
./nqueen-parallel.exe 15 8 7

The number of solutions to the 15-Queen problem is : 2279184
```

You can also use the provided bash script for running the program multiple times with a different number of queens:

```bash
./run-nqueen-parallel.sh <max_queens>
```

The script will run the program <max_queens> times from one to <max_queens> queens.
It will also print how much time it took for each iteration to run.
Example:

```bash
 ./run-nqueen-parallel.sh 18

The number of solutions to the 1-Queen problem is : 1

real    0m0.132s
user    0m0.000s
sys     0m0.031s

The number of solutions to the 2-Queen problem is : 0

real    0m0.141s
user    0m0.000s
sys     0m0.031s

The number of solutions to the 3-Queen problem is : 0

real    0m0.162s
user    0m0.016s
sys     0m0.063s

The number of solutions to the 4-Queen problem is : 2

real    0m0.161s
user    0m0.047s
sys     0m0.016s

The number of solutions to the 5-Queen problem is : 10

real    0m0.158s
user    0m0.000s
sys     0m0.047s

The number of solutions to the 6-Queen problem is : 4

real    0m0.161s
user    0m0.000s
sys     0m0.047s

The number of solutions to the 7-Queen problem is : 40

real    0m0.163s
user    0m0.000s
sys     0m0.047s

The number of solutions to the 8-Queen problem is : 92

real    0m0.125s
user    0m0.000s
sys     0m0.016s

The number of solutions to the 9-Queen problem is : 352

real    0m0.164s
user    0m0.000s
sys     0m0.016s

The number of solutions to the 10-Queen problem is : 724

real    0m0.147s
user    0m0.000s
sys     0m0.016s

The number of solutions to the 11-Queen problem is : 2680

real    0m0.184s
user    0m0.016s
sys     0m0.016s

The number of solutions to the 12-Queen problem is : 14200

real    0m0.158s
user    0m0.031s
sys     0m0.000s

The number of solutions to the 13-Queen problem is : 73712

real    0m0.262s
user    0m0.078s
sys     0m0.016s

The number of solutions to the 14-Queen problem is : 365596

real    0m0.765s
user    0m0.563s
sys     0m0.016s

The number of solutions to the 15-Queen problem is : 2279184

real    0m3.063s
user    0m2.828s
sys     0m0.031s

The number of solutions to the 16-Queen problem is : 14772512

real    0m19.324s
user    0m18.859s
sys     0m0.016s

The number of solutions to the 17-Queen problem is : 95815104

real    2m3.705s
user    2m1.125s
sys     0m0.203s

The number of solutions to the 18-Queen problem is : 666090624

real    14m30.682s
user    14m20.000s
sys     0m0.766s
```

## Author

Giorgos Argyrides (g.aryrides@outlook.com)