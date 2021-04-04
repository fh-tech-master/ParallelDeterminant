# Parallel Determinant
This is a university project.

## General 
We decided to implement **Laplace's expansion theorem** as it can compute the determinant for matrizes with dimension > 3. 
Laplace's theorem has a complexity of **O(n!)** therefore we can not test our solution with very big dimensions, unless we wait very long.
One alternative would have been to use the Gaussian elimination algorithm which has better complexity, approximately **0(n^3)**.

## Test Data
We calculate the determinant for a **12-dimensional** matrix.

## Speed Sequential Solution
Average Speed of 10 runs with the Test Data: **4905.72ms** or **4.91s**.

## Speed Naive Parallel Solution 
Average Speed of 10 runs with the Test Data: **5101.58ms** or **5.1s**.

After implementing our parallel solution via OpenMP's Tasks and playing around with it a bit it quickly became clear that the naive parallel solution
is slower than the sequential solution even. Creating a task for every Sub-Matrix
leads to **Over-Subscription** which in turn makes our solution slower(just like in ParallelSort).

## Thresholds
To solve the problem of **Over-Subscription** we decided to set thresholds. In our case we want to start executing sequentially as soon as we hit a certain dimension of the matrix.
As the termination condition of our recursion is reaching two dimensions (as we can calculate the determinant easily with **a * d - c * b**) the **smallest threshold** 
that would make any difference is **3**.
As we test with 12 dimensions the **largest threshold** making sense (to still compute anything in parallel) would be **11**.

## Speed with Thresholds
We took the average of 10 executions for a 12-dimensional matrix for the thresholds from 3-11 and wrote the results to a csv. We also compared the speed for each threshold
with the speed of the sequential solution (also average of 10 executions for 12-dimensional).
This allowed us to create the following graph:

![executionTime](executionTimeparallelThresholds.png "executionTime")

As we can see the fastest execution was reached by setting the threshold to 4.

![executionTime](speedupParallelThresholds.png "speedup")

Again we see that the speedup at threshold 4 is best after that it rapidly declines but seems to rise slightly at 11.
