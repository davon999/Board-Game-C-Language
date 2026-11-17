# Board Game in C

A board game simulation written in C: players sign up, fight in a knockout tournament built on binary trees, and the winners are sent on missions across a map with obstacles, where the shortest path is found with the **A\*** algorithm.

The project is built around classic data structures, all implemented from scratch: queue, stack, binary search tree, tournament tree and min-heap.

---

## How it works

The simulation runs in three stages.

### 1. Players, map and missions

- **Players** are read from a `.csv` file and added to a **queue** in the order they are read. Each one gets a unique `id`, starting from 0.
- **The map** is read from a text file and split into three matrices: cell values (free/obstacle), horizontal movement costs and vertical movement costs.
- **Missions** (start point → end point) are read from a `.csv` file and pushed onto a **stack**.

A player has:

| Field | Type | Notes |
|---|---|---|
| `prenume_nume` | `char *` | dynamically allocated, only as much as needed |
| `energie` | `float` | |
| `abilitate` | `enum` | `NEUTRALITATE`, `REINCERCARE`, `RENASTERE` |
| `id` | `int` | assigned in reading order |

### 2. The tournament

1. Players are dequeued and inserted into a **BST**, ordered by energy.
2. The BST is converted into a **tournament tree**: leaves are players, and internal nodes are fights (`><`) between the players below them.
3. Fights are resolved from the leaves up to the root. The player with more energy wins (the left one on a tie), and their energy becomes:

   ```
   E_winner = max(E1, E2) + |E1 - E2|
   ```

4. At the end, the top **k** players by remaining energy are selected, where **k** is the largest power of 2 such that `k ≤ n` (n = number of players).

**Example** with three players with energies J0 = 15, J1 = 5, J2 = 17:

```
       BST                        Tournament tree

     J0 (15)                          J1J0J2
     /     \                          /     \
 J1 (5)   J2 (17)                  J1J0      J2
                                  /    \
                                J1      J0
```

- J1 (5) vs J0 (15) → **J0** wins, with 15 + 10 = **25**
- J0 (25) vs J2 (17) → **J0** wins, with 25 + 8 = **33**

Ranking (k = 2): `J0 33.00`, `J2 17.00`

### 3. Missions on the map

Each winner, in descending order of energy, receives the next mission popped from the stack. The path from start to destination is found with **A\***. Every step costs the player the movement cost between the two cells; if their energy drops to 0 or below, the mission fails.

Example output:

```
J0 - 21.00: [0, 0] -- [1, 0] -- [1, 1] -- [0, 1] -- [0, 2]
J2: Nu s-a putut efectua misiunea
```

(The second line means "the mission could not be completed".)

---

## Data structures and algorithms

### Queue (singly linked list)

I keep both `front` and `rear` pointers, so inserting at the back and removing from the front both take constant time.

| Operation | Complexity |
|---|---|
| `enqueue` (add to the back) | O(1) |
| `dequeue` (remove from the front) | O(1) |
| traversal | O(n) |

### Stack (singly linked list)

A single `top` pointer to the top of the stack. Insertion and removal always happen at the top (LIFO), so the last mission read is the first one popped.

| Operation | Complexity |
|---|---|
| `push` (add to the top) | O(1) |
| `pop` (remove from the top) | O(1) |

### Binary search tree

Players are inserted recursively with energy as the key: lower energy → left subtree, greater or equal energy → right subtree.

| Operation | Complexity |
|---|---|
| insertion | O(h), h = tree height (O(log n) on average, O(n) if the tree degenerates) |
| post-order traversal | O(n) |

### Tournament tree

The BST → tournament tree conversion is done recursively, bottom-up. For each subtree there are four cases:

1. **full subtree**: the left leaf and the parent are joined by a fight node, which is then joined by another fight node with the right leaf;
2. **left child only**: the leaf and the parent are joined by a fight node;
3. **right child only**: the parent and the leaf are joined by a fight node;
4. **children that are already fight nodes**: the same rules apply, with fight nodes treated like player nodes.

Each node gets a label made of the ids of the players involved (e.g. `J1J0J2`), which can be printed in post-order.

| Operation | Complexity |
|---|---|
| conversion | O(n), n = number of nodes in the BST |

### Simulating the fights and selecting the top k

`fight_and_collect` traverses the tree once: at each fight node it decides the winner, updates their energy and stores the loser in an array. The overall winner stays at the root and is added to the array as well. The array is then sorted by energy in descending order with `qsort`, and the top k players are the first k entries.

| Step | Complexity |
|---|---|
| traversal and fights | O(n) |
| sorting with `qsort` | O(n log n) |
| selecting the top k | O(k) |
| **total** | **O(n log n)** time, O(n) space |

**Why this approach:** the tree is traversed only once, and every player ends up in the array exactly once, with their energy after their last fight. Since k is the largest power of 2 with `k ≤ n`, we always have `k > n/2`, so at least half of the players have to be selected anyway. A partial selection (for example with a heap of size k) would cost O(n log k) = O(n log n), the same, so a full sort is the simplest solution with no loss in efficiency.

### Min-heap (priority queue)

Used by A\*. Always keeps the node with the lowest `f` value at the top.

| Operation | Complexity |
|---|---|
| insertion (`heap_push`) | O(log n) |
| extract minimum (`heap_pop`) | O(log n) |
| access minimum | O(1) |

### The A\* algorithm

Finds the shortest path on the map from the source to the destination of each mission. The cost function:

```
f(n) = g(n) + min_cost * h(n)
h(n) = |n.x - d.x| + |n.y - d.y|      (Manhattan distance)
```

- `g(n)`: the actual cost of the path from the source to `n`
- `h(n)`: the Manhattan distance from `n` to the destination `d`
- `min_cost`: the smallest cost in the cost matrices

At each step I pop the node with the lowest `f` from the heap, explore its (at most) 4 reachable neighbours and update their cost if a shorter path was found. A neighbour is pushed onto the heap only if the player still has energy left (`energy - g > 0`). The path is rebuilt at the end from the `prev_x` / `prev_y` matrices.

**Why `min_cost * h(n)`:** every step on the map costs at least `min_cost`, so `min_cost * h(n)` never exceeds the real remaining cost to the destination. A heuristic that never overestimates (an admissible heuristic) guarantees that A\* finds the optimal path.

| | Complexity |
|---|---|
| time | O(E log V), where V = number of reachable cells and E = number of edges (E ≤ 4V), so O(V log V) |
| space | O(V), for the `g`, `visited`, `prev_x`, `prev_y` matrices and the heap |

---

## Map representation

The map is stored in three dynamically allocated matrices:

| Matrix | Size | Contents |
|---|---|---|
| cell values | `n_lin × n_col` | 0 = free, 1 = obstacle |
| row costs | `n_lin × (n_col - 1)` | horizontal movement costs |
| column costs | `(n_lin - 1) × n_col` | vertical movement costs |

The input file alternates two kinds of lines: lines with cell values interleaved with horizontal costs, and lines with vertical costs. It is read line by line with `strtok`.

```
0  12  0  1  0
6      3     5
0  2   0  1  1
12     7     4
0  4   1  1  0
```

For the example above, the map has 3 × 3 cells: the line `0 12 0 1 0` means cell (0,0) = 0, cost 12 to (0,1), cell (0,1) = 0, cost 1 to (0,2), cell (0,2) = 0.

---

## Input format

**Players** (`.csv`):

```
PRENUME NUME;ENERGIE;ABILITATE
Mirel Alexandru;15;NEUTRALITATE
Gabi Manea;5;RENASTERE
Viorel-Bogdan Protop;17;REINCERCARE
```

**Missions** (`.csv`), coordinates in the map matrix:

```
Xi;Yi;Xf;Yf
2;0;0;0
0;0;0;2
```

---

## Project structure

```
.
├── src/
│   ├── biblioteca.h   # struct and function declarations
│   ├── biblioteca.c   # implementation of all functions
│   └── main.c         # main program flow
├── Makefile
└── README.md
```

## Build

```bash
make
```

## Memory management

All dynamically allocated memory (players, names, queue/stack/tree nodes, map matrices, the heap) is freed after use, through a dedicated function for each structure. You can check this with:

```bash
valgrind --leak-check=full ./<executable>
```

---

## Author

**Poturlu David-Andrei**
