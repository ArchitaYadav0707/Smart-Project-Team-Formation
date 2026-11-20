#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STUDENTS 500
#define NAME_LEN 50
#define SKILLS 3
#define INF 999999

/*
   Smart Project Team Formation Platform - Modules I, II, III, IV
   1. AVL Tree, Max Heap, Heap Sort (Module I)
   2. Adjacency List, Dijkstra's, Kruskal's (Module II)
   3. Backtracking (Module III)
   4. 0/1 Knapsack, Resource Allocation DP, Branch & Bound (Module IV)
*/

// --- CORE DATA STRUCTURES ---

typedef struct Student
{
    int roll;
    char name[NAME_LEN];
    int python;
    int ml;
    int sql;
    int effort; // Added for Module IV (Budget)
    int height;
    struct Student *left;
    struct Student *right;
} Student;

typedef struct
{
    int roll;
    char name[NAME_LEN];
    float score;
    int effort;
    int python;
    int ml;
    int sql;
} Candidate;

Student *root = NULL;

// --- MODULE II: GRAPH DATA STRUCTURES ---

typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
} AdjListNode;

typedef struct
{
    AdjListNode *head;
} AdjList;

typedef struct
{
    int V;
    AdjList *array;
} Graph;

typedef struct
{
    int src, dest, weight;
} Edge;

typedef struct
{
    int parent;
    int rank;
} Subset;

// Graph global for interactive connection building
Graph *teamGraph = NULL;
Edge teamEdges[MAX_STUDENTS];
int edgeCount = 0;

/* ---------- Input and score utilities ---------- */

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int height(Student *node)
{
    return node ? node->height : 0;
}

int getBalance(Student *node)
{
    return node ? height(node->left) - height(node->right) : 0;
}

float compatibility(int python, int ml, int sql)
{
    return (python + ml + sql) / 3.0f;
}

void readName(char name[])
{
    printf("Enter student name (no spaces): ");
    scanf("%49s", name);
}

int readScore(const char *skill)
{
    int value;
    while (1)
    {
        printf("%s score (0-100): ", skill);
        if (scanf("%d", &value) == 1 && value >= 0 && value <= 100)
        {
            return value;
        }
        printf("Invalid input. Enter a number from 0 to 100.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }
    }
}

int readNonNegativeInt(const char *message)
{
    int value;
    while (1)
    {
        printf("%s", message);
        if (scanf("%d", &value) == 1 && value >= 0)
        {
            return value;
        }
        printf("Please enter 0 or a positive integer.\n");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }
    }
}

/* ---------- AVL Tree ---------- */

Student *rightRotate(Student *y)
{
    Student *x = y->left;
    Student *subtree = x->right;
    x->right = y;
    y->left = subtree;
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));
    return x;
}

Student *leftRotate(Student *x)
{
    Student *y = x->right;
    Student *subtree = y->left;
    y->left = x;
    x->right = subtree;
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));
    return y;
}

Student *searchStudent(Student *node, int roll)
{
    if (node == NULL || node->roll == roll)
        return node;
    if (roll < node->roll)
        return searchStudent(node->left, roll);
    return searchStudent(node->right, roll);
}

Student *insertStudent(Student *node, int roll, const char name[], int python, int ml, int sql, int effort)
{
    if (node == NULL)
    {
        Student *newNode = malloc(sizeof(Student));
        if (newNode == NULL)
        {
            printf("Memory allocation failed.\n");
            exit(EXIT_FAILURE);
        }
        newNode->roll = roll;
        strcpy(newNode->name, name);
        newNode->python = python;
        newNode->ml = ml;
        newNode->sql = sql;
        newNode->effort = effort;
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (roll < node->roll)
        node->left = insertStudent(node->left, roll, name, python, ml, sql, effort);
    else if (roll > node->roll)
        node->right = insertStudent(node->right, roll, name, python, ml, sql, effort);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && roll < node->left->roll)
        return rightRotate(node);
    if (balance < -1 && roll > node->right->roll)
        return leftRotate(node);
    if (balance > 1 && roll > node->left->roll)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && roll < node->right->roll)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Student *minimumNode(Student *node)
{
    Student *current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

Student *deleteStudent(Student *node, int roll)
{
    if (node == NULL)
        return NULL;

    if (roll < node->roll)
    {
        node->left = deleteStudent(node->left, roll);
    }
    else if (roll > node->roll)
    {
        node->right = deleteStudent(node->right, roll);
    }
    else
    {
        if (node->left == NULL || node->right == NULL)
        {
            Student *child = node->left ? node->left : node->right;
            free(node);
            return child;
        }
        else
        {
            Student *successor = minimumNode(node->right);
            node->roll = successor->roll;
            strcpy(node->name, successor->name);
            node->python = successor->python;
            node->ml = successor->ml;
            node->sql = successor->sql;
            node->effort = successor->effort;
            node->right = deleteStudent(node->right, successor->roll);
        }
    }

    if (node == NULL)
        return node;
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

/* ---------- Display and candidate mapping ---------- */

void printStudent(Student *s)
{
    printf("%-8d %-18s %-7d %-7d %-7d %-8d %8.2f\n",
           s->roll, s->name, s->python, s->ml, s->sql, s->effort,
           compatibility(s->python, s->ml, s->sql));
}

void displayStudents(Student *node)
{
    if (node == NULL)
        return;
    displayStudents(node->left);
    printStudent(node);
    displayStudents(node->right);
}

int countStudents(Student *node)
{
    if (node == NULL)
        return 0;
    return 1 + countStudents(node->left) + countStudents(node->right);
}

void collectCandidates(Student *node, Candidate candidates[], int *index)
{
    if (node == NULL)
        return;
    collectCandidates(node->left, candidates, index);

    candidates[*index].roll = node->roll;
    strcpy(candidates[*index].name, node->name);
    candidates[*index].score = compatibility(node->python, node->ml, node->sql);
    candidates[*index].effort = node->effort;
    candidates[*index].python = node->python;
    candidates[*index].ml = node->ml;
    candidates[*index].sql = node->sql;
    (*index)++;

    collectCandidates(node->right, candidates, index);
}

void swapCandidate(Candidate *a, Candidate *b)
{
    Candidate temp = *a;
    *a = *b;
    *b = temp;
}

/* ---------- Max Heap & Heap Sort ---------- */

void heapifyMax(Candidate arr[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left].score > arr[largest].score)
        largest = left;
    if (right < n && arr[right].score > arr[largest].score)
        largest = right;

    if (largest != i)
    {
        swapCandidate(&arr[i], &arr[largest]);
        heapifyMax(arr, n, largest);
    }
}

void buildMaxHeap(Candidate arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
        heapifyMax(arr, n, i);
}

void heapSortDescending(Candidate arr[], int n)
{
    buildMaxHeap(arr, n);
    for (int i = n - 1; i > 0; i--)
    {
        swapCandidate(&arr[0], &arr[i]);
        heapifyMax(arr, i, 0);
    }
    for (int i = 0; i < n / 2; i++)
        swapCandidate(&arr[i], &arr[n - 1 - i]);
}

/* ---------- MODULE II: GRAPH ALGORITHMS ---------- */

Graph *createGraph(int V)
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList *)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;
    return graph;
}

void addEdge(Graph *graph, int src, int dest, int weight)
{
    AdjListNode *newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = (AdjListNode *)malloc(sizeof(AdjListNode));
    newNode->dest = src;
    newNode->weight = weight;
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

int minDistance(int dist[], bool sptSet[], int V)
{
    int min = INF, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
    return min_index;
}

void dijkstra(Graph *graph, int src, Candidate candidates[])
{
    int V = graph->V;
    int dist[V];
    bool sptSet[V];

    for (int i = 0; i < V; i++)
    {
        dist[i] = INF;
        sptSet[i] = false;
    }
    dist[src] = 0;

    for (int count = 0; count < V - 1; count++)
    {
        int u = minDistance(dist, sptSet, V);
        sptSet[u] = true;

        AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
            if (!sptSet[v] && dist[u] != INF && dist[u] + pCrawl->weight < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
            }
            pCrawl = pCrawl->next;
        }
    }
    printf("\n--- Dijkstra's Shortest Communication Paths from %s ---\n", candidates[src].name);
    for (int i = 0; i < V; i++)
    {
        if (dist[i] == INF)
            printf("To %s: Unreachable\n", candidates[i].name);
        else
            printf("To %s: Cost %d\n", candidates[i].name, dist[i]);
    }
}

int find(Subset subsets[], int i)
{
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

void Union(Subset subsets[], int x, int y)
{
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);
    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else
    {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

int compareEdges(const void *a, const void *b)
{
    Edge *a1 = (Edge *)a;
    Edge *b1 = (Edge *)b;
    return a1->weight > b1->weight;
}

void kruskalMST(Edge edges[], int V, int E, Candidate candidates[])
{
    Edge result[V];
    int e = 0, i = 0;
    qsort(edges, E, sizeof(edges[0]), compareEdges);

    Subset *subsets = (Subset *)malloc(V * sizeof(Subset));
    for (int v = 0; v < V; ++v)
    {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    while (e < V - 1 && i < E)
    {
        Edge next_edge = edges[i++];
        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        if (x != y)
        {
            result[e++] = next_edge;
            Union(subsets, x, y);
        }
    }
    printf("\n--- Kruskal's MST (Smoothest Team Synergy) ---\n");
    int totalFriction = 0;
    for (i = 0; i < e; ++i)
    {
        printf("Link: %s -- %s == Cost: %d\n", candidates[result[i].src].name, candidates[result[i].dest].name, result[i].weight);
        totalFriction += result[i].weight;
    }
    printf("Minimum Total Communication Friction: %d\n", totalFriction);
    free(subsets);
}

/* ---------- MODULE III: BACKTRACKING ---------- */

bool bestTeamFound = false;

void backtrackSkills(Candidate candidates[], int n, int teamSize, int index, int currentTeam[], int currentSize, int hasPy, int hasML, int hasSQL)
{
    if (currentSize == teamSize)
    {
        if (hasPy && hasML && hasSQL)
        {
            printf("Valid Team: ");
            for (int i = 0; i < teamSize; i++)
                printf("%s ", candidates[currentTeam[i]].name);
            printf("- Covers all skills (Python, ML, SQL).\n");
            bestTeamFound = true;
        }
        return;
    }
    if (index >= n || bestTeamFound)
        return;

    currentTeam[currentSize] = index;
    backtrackSkills(candidates, n, teamSize, index + 1, currentTeam, currentSize + 1,
                    hasPy | (candidates[index].python > 0),
                    hasML | (candidates[index].ml > 0),
                    hasSQL | (candidates[index].sql > 0));

    backtrackSkills(candidates, n, teamSize, index + 1, currentTeam, currentSize, hasPy, hasML, hasSQL);
}

/* ---------- MODULE IV: DP & BRANCH AND BOUND ---------- */

void knapsackDP(Candidate candidates[], int n, int budget)
{
    float dp[n + 1][budget + 1];

    for (int i = 0; i <= n; i++)
    {
        for (int w = 0; w <= budget; w++)
        {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (candidates[i - 1].effort <= w)
            {
                float included = candidates[i - 1].score + dp[i - 1][w - candidates[i - 1].effort];
                dp[i][w] = (included > dp[i - 1][w]) ? included : dp[i - 1][w];
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    printf("\n--- 0/1 Knapsack Effort Optimization ---\n");
    printf("Maximum Score achievable within %d hours: %.2f\n", budget, dp[n][budget]);

    float res = dp[n][budget];
    int w = budget;
    printf("Selected Candidates:\n");
    for (int i = n; i > 0 && res > 0; i--)
    {
        if (res == dp[i - 1][w])
            continue;
        else
        {
            printf("- %s (Effort: %d hours, Score: %.2f)\n", candidates[i - 1].name, candidates[i - 1].effort, candidates[i - 1].score);
            res = res - candidates[i - 1].score;
            w = w - candidates[i - 1].effort;
        }
    }
}

float calculateUpperBound(float currentScore, int currentEffort, int remainingBudget, Candidate candidates[], int nextIdx, int n)
{
    float bound = currentScore;
    int effortLeft = remainingBudget;
    int i = nextIdx;
    while (i < n && candidates[i].effort <= effortLeft)
    {
        effortLeft -= candidates[i].effort;
        bound += candidates[i].score;
        i++;
    }
    if (i < n && effortLeft > 0)
    {
        bound += candidates[i].score * ((float)effortLeft / candidates[i].effort);
    }
    return bound;
}

/* ---------- MENU OPERATIONS ---------- */

void addStudent(void)
{
    int roll = readNonNegativeInt("Enter roll number: ");
    if (searchStudent(root, roll) != NULL)
    {
        printf("This roll number already exists.\n");
        return;
    }
    char name[NAME_LEN];
    readName(name);
    int python = readScore("Python");
    int ml = readScore("Machine Learning");
    int sql = readScore("SQL");
    int effort = readNonNegativeInt("Enter effort required (hours): ");

    root = insertStudent(root, roll, name, python, ml, sql, effort);
    printf("Student added successfully to the AVL Tree.\n");
}

void findStudent(void)
{
    int roll = readNonNegativeInt("Enter roll number to search: ");
    Student *s = searchStudent(root, roll);
    if (s == NULL)
    {
        printf("Student not found.\n");
        return;
    }
    printf("\nStudent found:\n");
    printf("Roll number : %d\n", s->roll);
    printf("Name        : %s\n", s->name);
    printf("Python      : %d\n", s->python);
    printf("Machine Learning: %d\n", s->ml);
    printf("SQL         : %d\n", s->sql);
    printf("Effort (hrs): %d\n", s->effort);
    printf("Compatibility: %.2f/100\n", compatibility(s->python, s->ml, s->sql));
}

void updateStudent(void)
{
    int roll = readNonNegativeInt("Enter roll number to update: ");
    Student *s = searchStudent(root, roll);
    if (s == NULL)
    {
        printf("Student not found.\n");
        return;
    }
    printf("Enter updated details.\n");
    readName(s->name);
    s->python = readScore("Python");
    s->ml = readScore("Machine Learning");
    s->sql = readScore("SQL");
    s->effort = readNonNegativeInt("Enter effort required (hours): ");
    printf("Student profile updated successfully.\n");
}

void showAllStudents(void)
{
    if (root == NULL)
    {
        printf("No student profiles available.\n");
        return;
    }
    printf("\nSTUDENT DATABASE (AVL TREE - INORDER)\n");
    printf("%-8s %-18s %-7s %-7s %-7s %-8s %8s\n", "Roll No", "Name", "Python", "ML", "SQL", "Effort", "Score");
    printf("----------------------------------------------------------------------\n");
    displayStudents(root);
}

void showLeaderboard(void)
{
    int n = countStudents(root);
    if (n == 0)
    {
        printf("No students available for ranking.\n");
        return;
    }
    Candidate candidates[MAX_STUDENTS];
    int index = 0;
    collectCandidates(root, candidates, &index);
    heapSortDescending(candidates, n);

    printf("\nCOMPATIBILITY LEADERBOARD\n");
    printf("%-6s %-8s %-18s %10s\n", "Rank", "Roll No", "Name", "Score / 100");
    printf("------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%-6d %-8d %-18s %10.2f\n", i + 1, candidates[i].roll, candidates[i].name, candidates[i].score);
    }
}

void selectTeam(void)
{
    int n = countStudents(root);
    if (n == 0)
    {
        printf("No students available for team selection.\n");
        return;
    }
    int teamSize = readNonNegativeInt("Enter required team size: ");
    if (teamSize > n)
    {
        printf("Only %d student(s) are available.\n", n);
        return;
    }
    Candidate candidates[MAX_STUDENTS];
    int index = 0;
    collectCandidates(root, candidates, &index);
    buildMaxHeap(candidates, n);

    printf("\nSELECTED PROJECT TEAM (MAX HEAP)\n");
    printf("%-6s %-8s %-18s %10s\n", "Rank", "Roll No", "Name", "Score / 100");
    printf("------------------------------------------------\n");
    for (int i = 0; i < teamSize; i++)
    {
        Candidate top = candidates[0];
        printf("%-6d %-8d %-18s %10.2f\n", i + 1, top.roll, top.name, top.score);
        candidates[0] = candidates[n - 1];
        n--;
        if (n > 0)
            heapifyMax(candidates, n, 0);
    }
}

/* ---------- Main menu ---------- */

int main(void)
{
    int choice;

    do
    {
        printf("\n========================================\n");
        printf(" SMART PROJECT TEAM FORMATION PLATFORM\n");
        printf("          MODULES I, II, III, IV\n");
        printf("========================================\n");
        printf("1. Add student profile\n");
        printf("2. Search student by roll number\n");
        printf("3. Update student profile\n");
        printf("4. Delete student profile\n");
        printf("5. Display all student profiles\n");
        printf("6. Select top project team (Max Heap)\n");
        printf("7. Display ranked leaderboard (Heap Sort)\n");
        printf("8. Evaluate Exact Skill Coverage (Backtracking)\n");
        printf("9. Optimize Effort Budget (0/1 Knapsack DP)\n");
        printf("10. Estimate Branch & Bound Upper Bound\n");
        printf("11. Add Communication Edge (Graph)\n");
        printf("12. Calculate Communication Paths (Dijkstra)\n");
        printf("13. Build Team Synergy Network (Kruskal MST)\n");
        printf("0. Exit\n");
        printf("----------------------------------------\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid menu input. Please enter a number.\n");
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
            {
            }
            continue;
        }

        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            findStudent();
            break;
        case 3:
            updateStudent();
            break;
        case 4:
        {
            int roll = readNonNegativeInt("Enter roll number to delete: ");
            if (searchStudent(root, roll) == NULL)
                printf("Student not found.\n");
            else
            {
                root = deleteStudent(root, roll);
                printf("Student deleted successfully.\n");
            }
            break;
        }
        case 5:
            showAllStudents();
            break;
        case 6:
            selectTeam();
            break;
        case 7:
            showLeaderboard();
            break;

        case 8:
        {
            int n = countStudents(root);
            if (n == 0)
            {
                printf("No students available.\n");
                break;
            }
            Candidate candidates[MAX_STUDENTS];
            int index = 0;
            collectCandidates(root, candidates, &index);
            int teamSize = readNonNegativeInt("Enter team size to verify skills for: ");
            bestTeamFound = false;
            int currentTeam[MAX_STUDENTS];
            backtrackSkills(candidates, n, teamSize, 0, currentTeam, 0, 0, 0, 0);
            if (!bestTeamFound)
                printf("No combination covers all required skills.\n");
            break;
        }

        case 9:
        {
            int n = countStudents(root);
            if (n == 0)
            {
                printf("No students available.\n");
                break;
            }
            Candidate candidates[MAX_STUDENTS];
            int index = 0;
            collectCandidates(root, candidates, &index);
            int budget = readNonNegativeInt("Enter maximum effort budget (hours): ");
            knapsackDP(candidates, n, budget);
            break;
        }

        case 10:
        {
            int n = countStudents(root);
            if (n == 0)
            {
                printf("No students available.\n");
                break;
            }
            Candidate candidates[MAX_STUDENTS];
            int index = 0;
            collectCandidates(root, candidates, &index);
            heapSortDescending(candidates, n); // Sort by score for greedy bounding

            float currentScore = 0;
            int currentEffort = 0;
            int budget = readNonNegativeInt("Enter total budget (hours) to estimate: ");
            float bound = calculateUpperBound(currentScore, currentEffort, budget, candidates, 0, n);
            printf("Optimistic Upper Bound for empty team: %.2f points\n", bound);
            break;
        }

        case 11:
        {
            int n = countStudents(root);
            if (n < 2)
            {
                printf("Need at least 2 students to form connections.\n");
                break;
            }
            Candidate candidates[MAX_STUDENTS];
            int index = 0;
            collectCandidates(root, candidates, &index);

            printf("Available Students (Index : Name)\n");
            for (int i = 0; i < n; i++)
                printf("%d : %s\n", i, candidates[i].name);

            int u = readNonNegativeInt("Enter Index of first student (0-based): ");
            int v = readNonNegativeInt("Enter Index of second student (0-based): ");
            int weight = readNonNegativeInt("Enter communication cost/friction: ");

            teamEdges[edgeCount].src = u;
            teamEdges[edgeCount].dest = v;
            teamEdges[edgeCount].weight = weight;
            edgeCount++;
            printf("Connection added.\n");
            break;
        }

        case 12:
        {
            int n = countStudents(root);
            if (n == 0 || edgeCount == 0)
            {
                printf("Add students and edges first.\n");
                break;
            }
            Candidate candidates[MAX_STUDENTS];
            int index = 0;
            collectCandidates(root, candidates, &index);

            if (teamGraph)
            {
                free(teamGraph->array);
                free(teamGraph);
            }
            teamGraph = createGraph(n);
            for (int i = 0; i < edgeCount; i++)
                addEdge(teamGraph, teamEdges[i].src, teamEdges[i].dest, teamEdges[i].weight);

            int src = readNonNegativeInt("Enter source student index (0-based): ");
            if (src >= n)
            {
                printf("Invalid index.\n");
                break;
            }
            dijkstra(teamGraph, src, candidates);
            break;
        }

        case 13:
        {
            int n = countStudents(root);
            if (n == 0 || edgeCount == 0)
            {
                printf("Add students and edges first.\n");
                break;
            }
            Candidate candidates[MAX_STUDENTS];
            int index = 0;
            collectCandidates(root, candidates, &index);
            kruskalMST(teamEdges, n, edgeCount, candidates);
            break;
        }

        case 0:
            printf("Exiting Platform. Thank you!\n");
            break;
        default:
            printf("Invalid choice. Select a listed option.\n");
        }

    } while (choice != 0);

    while (root != NULL)
        root = deleteStudent(root, root->roll);
    if (teamGraph)
    {
        free(teamGraph->array);
        free(teamGraph);
    }
    return 0;
}