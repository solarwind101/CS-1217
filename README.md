## Floyd-Warshall Algorithm (Multithreading)

**Author:** Suraj Sharma  
**Course:** CS 1217 - Operating System  
**Assignment:** 4  

---

### Description
Implements the Floyd-Warshall Algorithm using multithreading.

---

### Compilation
```bash
gcc A4.c -o A4
./A4

## Multi-threading

In this assignment, we are required to implement a multi-threaded version of the Floyd-Warshall **All-Pairs-Shortest-Path** algorithm. Since multi-threading is commonly used in graph algorithms, our task is to design our own approach and provide pseudocode to explain our implementation.

---

### Single-Threaded Version

Here’s the single-threaded version of the algorithm. The single-threaded version of the algorithm follows a **three-level nested loop structure**. The outermost loop iterates over an intermediate node `k`, considering paths that pass through it. The two inner loops iterate over all node pairs `(i, j)`, checking whether using `k` as an intermediate node offers a shorter path. The `dist[i][j]` matrix keeps track of the shortest known distances between node pairs so far, and each iteration updates these values.  

**A key observation** here is that in each iteration, we only need to read from the \(k^{th}\) row and column while updating the same `dist` matrix, without requiring additional copies.

```pseudo
for k = 1 to n do
    for i = 1 to n do
        for j = 1 to n do
            if (dist[i][k] + dist[k][j] < dist[i][j])
                dist[i][j] = dist[i][k] + dist[k][j]
        end for
    end for
end for
```

---

### Multi-Threaded Version

We modify this algorithm to take advantage of **multi-threading**. The outer `k` loop remains unchanged, but instead of sequentially iterating over `i`, we create **n threads**, where each thread is responsible for computing the updates for a specific row `i`. Each thread independently executes the inner `j` loop, computing updates in parallel.

Since multiple threads access the `dist` matrix simultaneously, we may need to handle **race conditions** correctly if they arise.  

At the end of every iteration of the `k` loop, we need to **wait for all the threads to finish**. Only then can we start another iteration of the `k` loop. Therefore, we need to **join the threads** at the end of every iteration of the `k` loop.  

---

### Input Format

The graph structure is given as follows:  

- The first line contains two integers, **N** and **M**, where:  
  - \(N\) = number of nodes  
  - \(M\) = number of undirected edges  

- The next **M** lines each contain three integers: \(u_i, v_i, w_i\), representing an undirected edge between nodes \(u_i\) and \(v_i\) with weight \(w_i\).  

---

### Constraints

- \(N \in [1, 100]\)  
- All edge weights are positive.  

---

### Output Format

Print the final `dist` matrix. If node `j` is not reachable from node `i`, print **INF** as the distance between `i` and `j`.  

---

### Example

**Input**
```
4 4
1 2 1
2 3 1
3 4 1
4 1 1
```

**Output**
```
0 1 2 1
1 0 1 2
2 1 0 1
1 2 1 0
```
