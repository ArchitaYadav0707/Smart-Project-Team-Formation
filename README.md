# Smart Project Team Formation Platform

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

A terminal-based C application that models a student database and project team formation system. It applies core Data Structures and Algorithms (DSA) to solve the real-world problem of forming optimal, skill-complete, and budget-feasible academic project teams.

## 🚀 Overview

This platform implements a 5-step pipeline using advanced data structures:
1. **Storage (AVL Tree):** Stores student profiles by roll number, guaranteeing `O(log n)` insertion and search times.
2. **Ranking (Max Heap):** Ranks and extracts the highest-scoring candidates in `O(1)` time.
3. **Skill Validation (Backtracking):** Validates team combinations to ensure the final group possesses the exact required skills (Python, ML, SQL).
4. **Budget Optimization (0/1 Knapsack DP & Branch and Bound):** Optimizes the team selection to maximize compatibility scores without exceeding a specific "effort" budget (measured in hours).
5. **Communication Synergy (Dijkstra's & Kruskal's Algorithms):** Calculates the shortest communication paths between students and builds a Minimum Spanning Tree (MST) to minimize internal team communication friction.

## 🛠️ Data Structures & Algorithms Used
* **Module I:** AVL Tree, Max Heap, Heap Sort
* **Module II:** Adjacency List, Dijkstra's Algorithm, Kruskal's Algorithm, Union-Find (Disjoint Set)
* **Module III:** Backtracking (State-Space Tree pruning)
* **Module IV:** Dynamic Programming (0/1 Knapsack), Branch and Bound

## 📋 Prerequisites
* GCC Compiler (or any standard C compiler like Clang or MinGW for Windows)

## ⚙️ Compilation & Execution
To compile and run the platform, open your terminal in the project directory and run the following commands:

```bash
# Compile the C file
gcc smart_team.c -o smart_team

# Run the executable (Linux/macOS)
./smart_team

# Run the executable (Windows)
smart_team.exe
```

## 🖥️ Menu Options
Upon running the program, you will be presented with an interactive CLI menu containing the following operations:

### Database Operations (AVL Tree)
* `1` - **Add student profile:** Insert a new student with their skills and effort requirements.
* `2` - **Search student:** Find a student by their roll number in `O(log n)` time.
* `3` - **Update student profile:** Modify a student's skills or effort hours.
* `4` - **Delete student profile:** Remove a student and automatically rebalance the AVL tree.
* `5` - **Display all student profiles:** Perform an inorder traversal to show the database.

### Ranking & Team Selection (Heaps)
* `6` - **Select top project team:** Greedily pick the highest-scoring students using a Max Heap.
* `7` - **Display ranked leaderboard:** View all candidates ranked by score using Heap Sort.

### Constraints & Optimization (Backtracking & DP)
* `8` - **Evaluate Exact Skill Coverage:** Use Backtracking to find a combination that covers all required skills (Python, ML, SQL).
* `9` - **Optimize Effort Budget:** Use 0/1 Knapsack DP to find the highest-scoring team that fits within a strict effort budget.
* `10` - **Estimate Branch & Bound Upper Bound:** Calculate an optimistic score bound for partial team selections.

### Network & Synergy (Graphs)
* `11` - **Add Communication Edge:** Create a weighted link (representing communication friction) between two students.
* `12` - **Calculate Communication Paths:** Use Dijkstra's Algorithm to find the shortest paths from a specific student.
* `13` - **Build Team Synergy Network:** Use Kruskal's Algorithm to find the Minimum Spanning Tree (MST) connecting the team.

* `0` - **Exit**

## 👨‍💻 Author
* **Archita Yadav** - *B.Tech CSE-AIML*
