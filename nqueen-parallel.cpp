/*
 * Author: Giorgos Argyrides - g.argyrides@outlook.com
 *
 * This program solves the n-queen problem by using bit-patterns, recursion and threads.
 *
 */
#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>
#include <chrono>

typedef int chessboard;

// global variables
int n;              // number of queens (N)
chessboard all;     // contains N 1 bits, representing number of columns
int threadsNumber;  // number of threads
int threshold;      // threshold for switching to sequential mode


// Class representing a state of the n-queen chess board by using bit patterns
class ChessState {
public:
    ChessState() = default;

    ChessState(int ld_, int cols_, int rd_) : ld(ld_), cols(cols_), rd(rd_) {} // Constructor

    chessboard get_ld() {
        return ld;
    }

    chessboard get_cols() {
        return cols;
    }

    chessboard get_rd() {
        return rd;
    }

private:
    chessboard ld;      // bit pattern containing  1 for each position attacked along a left going diagonal
    chessboard cols;    // bit pattern containing 1 for each column that is already occupied
    chessboard rd;      // bit pattern containing 1 for each position attacked along a right going diagonal
};


// Class representing a thread-safe queue.
template<typename T>
class Queue {
private:
    std::queue<T> data_queue;
    mutable std::mutex m;
    std::condition_variable cv;

public:
    Queue() = default; // default Constructor

    // Pushes the value in the queue
    void push(T value) {
        std::lock_guard<std::mutex> lock(m);
        data_queue.push(value);
        cv.notify_one();
    }

/*
 * Pops the front value of the queue. Waits until queue is not empty or until timeout (in the case where the problem is
 * solved and no more items will be put in the queue). Returns true if an item is successfully popped.
 */
    bool pop(T &value) {
        std::unique_lock<std::mutex> lock(m);

        if (cv.wait_for(lock, std::chrono::milliseconds(100), [this] { return !data_queue.empty(); })) {
            value = data_queue.front();
            data_queue.pop();
            return true;
        } else
            return false;
    }
};


/* Finds the number of solutions to the N-queen problem
 * ld: a bit pattern containing ones for positions under attack along left diagonals for this row
 * cols: a bit pattern containing ones for columns that are already accupied
 * rd: a bit pattern containing ones for positions under attack along right diagonals for this row
 * all: a bit pattern where the first N bits are set to one, where N is the number of queens
 *
 * ld, cols, and rd contain sufficient info about the current state of the board.
 * (ld | cols | rd) is a bit pattern containing ones in all positions under attack for this row
 */
unsigned long seq_nqueen(chessboard ld, chessboard cols, chessboard rd) {
    unsigned long sol = 0;

    if (cols == all) // A solution is found
        return 1;

    chessboard pos = ~(ld | cols | rd) & all; // Possible positions for the queen on the current row
    chessboard next;
    while (pos !=
           0) {                  // Iterate over all possible positions and solve the (N-1)-queen in each case
        next = pos & (-pos);     // next possible position
        pos -= next;             // update the possible position
        sol += seq_nqueen((ld | next) << 1, cols | next, (rd | next) >> 1); // recursive call for the 'next' position
    }
    return sol;
}


/*
 * Unfolds the sub-problems (all possible positions for the (N-1)-queen) and puts them in a queue if N is less than
 * a given threshold. Otherwise calls the seq_queen function to solve the sub-problems
 */
unsigned long par_nqueen(Queue<ChessState> &qqueue, chessboard ld, chessboard cols, chessboard rd) {
    unsigned long sol = 0;

    if (cols == all) // A solution is found
        return 1;

    chessboard pos = ~(ld | cols | rd) & all; // Possible positions for the queen on the current row
    chessboard next;
    int count = 0;
    int i = cols;

    // counts the number of ones in cols which indicates how many queens are already placed on the board
    while (i) {
        count += i & 1;
        i = i >> 1;
    }

    while (pos != 0) {          // Iterate over all possible positions and solve the (N-1)-queen in each case
        next = pos & (-pos);    // next possible position
        pos -= next;
        if (count < n - threshold) { // if less than n - threshold queens are placed then push the (N-1)-queen in the
                                    // queue
            ChessState cs = ChessState((ld | next) << 1, cols | next, (rd | next) >> 1);
            qqueue.push(cs);
        } else // switch to sequential mode
            sol += seq_nqueen((ld | next) << 1, cols | next,
                              (rd | next) >> 1);
    }
    return sol;
}


/*
 * Pops the n-queen sub-problems from the queue and calls a function to solve each popped sub-problem.
 */
unsigned long nqueen_solver(Queue<ChessState> &qqueue) {
    unsigned long sol = 0;
    ChessState val;
    while (qqueue.pop(val)) {
        sol += par_nqueen(std::ref(qqueue), val.get_ld(), val.get_cols(), val.get_rd());
    }

    return sol;
}


/*
 * Main function:
 * Takes three arguments: 1) Number of Queens
 *                        2) The maximum number of threads
 *                        3) The point where the program switches to sequential mode
 * Creates a queue for storing the unfolded sub-problems and pushes in the initial problem.
 * Calls "maximum number of threads" threads to solve the problem in parallel,prints the total number of solutions.
 */
int main(int argc, char **argv) {

    if (argc < 4) {
        std::cout << "You have to provide : \n 1) Number of Queens \n 2) The maximum number of threads \n 3) The point "
                     "where the program switches to sequential mode" << std::endl;
        return 0;
    }
    unsigned long solutions = 0;
    n = std::stoi(argv[1]);
    threadsNumber = std::stoi(argv[2]);
    threshold = std::stoi(argv[3]);
    all = (1 << n) - 1; // set N bits on, representing number of columns
    Queue<ChessState> qqueue;
    std::vector<std::future<unsigned long>> futures((unsigned) threadsNumber);

    ChessState cs = ChessState(0, 0, 0);
    qqueue.push(cs); // Push the initial board state (where zero queens are placed) in the queue

    // Calls "maximum number of threads" threads to solve the problem in parallel
    for (int i = 0; i < threadsNumber; i++)
        futures[i] = std::async(&nqueen_solver, std::ref(qqueue));

    //  sums the number of solutions returned from each thread
    for (int i = 0; i < threadsNumber; i++)
        solutions += futures[i].get();

    std::cout << "The number of solutions to the " << n << "-Queen problem is : " << solutions << std::endl;

    return 0;
}