/*
 * Google Code Jam
 * Round 1A - Problem 1
========================================================================
 Problem

A tree is a connected graph with no cycles.

A rooted tree is a tree in which one special vertex is called the root.
If there is an edge between X and Y in a rooted tree, we say that Y is a
child of X if X is closer to the root than Y (in other words, the
shortest path from the root to X is shorter than the shortest path from
the root to Y).

A full binary tree is a rooted tree where every node has either exactly
2 children or 0 children.

You are given a tree G with N nodes (numbered from 1 to N). You are
allowed to delete some of the nodes. When a node is deleted, the edges
connected to the deleted node are also deleted. Your task is to delete
as few nodes as possible so that the remaining nodes form a full binary
tree for some choice of the root from the remaining nodes.
Input

The first line of the input gives the number of test cases, T. T test
cases follow. The first line of each test case contains a single integer
N, the number of nodes in the tree. The following N-1 lines each one
will contain two space-separated integers: Xi Yi, indicating that G
contains an undirected edge between Xi and Yi.
Output

For each test case, output one line containing "Case #x: y", where x is
the test case number (starting from 1) and y is the minimum number of
nodes to delete from G to make a full binary tree.
Limits

1 ≤ T ≤ 100.
1 ≤ Xi, Yi ≤ N
Each test case will form a valid connected tree.
Small dataset

2 ≤ N ≤ 15.
Large dataset

2 ≤ N ≤ 1000.
Sample

Input
    
Output
 

3
3
2 1
1 3
7
4 5
4 2
1 2
3 1
6 4
3 7
4
1 2
2 3
3 4

    

Case #1: 0
Case #2: 2
Case #3: 1

In the first case, G is already a full binary tree (if we consider node
1 as the root), so we don't need to do anything.

In the second case, we may delete nodes 3 and 7; then 2 can be the root
of a full binary tree.

In the third case, we may delete node 1; then 3 will become the root of
a full binary tree (we could also have deleted node 4; then we could
have made 2 the root).

========================================================================
 *
 * James Sullivan <sullivan.james.f@gmail.com>
 * 12/14
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {
    int index;
    int num_edges;
    struct node *edges[3];
} node;

typedef struct bnode {
    int index;
    struct bnode *left;
    struct bnode *right;
} bnode;

typedef struct btree {
    bnode *root;
} btree;

node *nodes[1000];

/* Returns the degree of the given vertex. */
static inline int degree(int x)
{
    return nodes[x]->num_edges;
}

/* Returns the size of the tree rooted at root. */
int size(node *root, node *parent)
{
    int i, ret = 1;
    if(root->num_edges > 1)
        for(i = 0; i < root->num_edges; i++)
            if(!(root->edges[i] == parent))
                ret += size(root->edges[i], root);
    return ret;
}

/* Returns the minimum number of vertices that must be removed to
 * make the tree rooted at i a full tree.
 */
int trim(node *root, node *parent)
{
    int ret;

    if(root->num_edges == 1) {
        /* No need to remove a singular leaf */
        ret = 0;
    } else if(root->num_edges == 2) {
        /* Trim the entire child tree, so this becomes a leaf */
        if(parent == root->edges[0])
            ret = size(root->edges[1], root);
        else
            ret = size(root->edges[0], root);
    } else {
        /* Otherwise recurse on the left and right children */
        if(parent == root->edges[0])
            ret = trim(root->edges[1], root) + trim(root->edges[2], root);
        else if(parent == root->edges[1])
            ret = trim(root->edges[0], root) + trim(root->edges[2], root);
        else if(parent == root->edges[2])
            ret = trim(root->edges[0], root) + trim(root->edges[1], root);
    }

    return ret;
}

/* Returns the minimum number of vertices that must be removed
 * to make a full tree, or -1 on failure. 
 */
int solve(int n)
{
    int this, ret = -1;
    int i, j = 0, candidates[1000];
    if(n < 0 || n > 1000)
        goto out;

    /* Get a list of removal candidates (those with 2 edges) */
    for(i = 0; i < n-1; i++)
        if(degree(i) == 2) candidates[j++] = i;

    ret = n-1;
    for(i = 0; i < j; i++) {
        /* Find out the number of trims needed for a j-rooted tree */
        this = trim(nodes[candidates[i]]->edges[0], nodes[candidates[i]]) + 
                trim(nodes[candidates[i]]->edges[1], nodes[candidates[i]]);
        if(this < ret)
            ret = this;
    }
out:
    return ret;
}

int main(int argc, char **argv)
{
    int num_tests, test = 0, round;
    int ret = 1;
    int n, i, j;

    scanf("%d",&num_tests);
    if(num_tests < 1 || num_tests > 100) {
        printf("Illegal number of tests (1 <= T <= 100)\n");
        goto out;
    }

    while(test++ < num_tests) {
        /* Number of nodes */
        scanf("%d",&n);
        if(n < 2 || n > 1000) {
            printf("Illegal number of nodes (2 <= N <= 1000)\n");
            continue;
        }

        /* Allocate the nodes */
        for(i = 0; i < n; i++) {
            nodes[i] = malloc(sizeof(node));
            if(!nodes[i]) {
                perror("Malloc");
                for(j = 0; j < i; j++)
                    free(nodes[j]);
                continue;
            }
            nodes[i]->index     = i;
            nodes[i]->num_edges = 0;
        }

        /* N-1 edges (index-index pairs) */
        for(i = 0; i < n-1; i++) {
            int fst, snd;
            scanf("%d %d", &fst, &snd);
            if(fst < 1 || fst > n) {
                printf("1 <= Xi <= N\n");
                continue; 
            }
            if(snd < 1 || snd > n) {
                printf("1 <= Yi <= N\n");
                continue; 
            }
            nodes[fst-1]->edges[nodes[fst-1]->num_edges++] = nodes[snd-1];
            nodes[snd-1]->edges[nodes[snd-1]->num_edges++] = nodes[fst-1];
        }

        round = solve(n);
        printf("Case #%d: ", test);
        if(round >= 0)
            printf("%d\n",round);
        else
            printf("Error\n");

        for(i = 0; i < n; i++) {
            free(nodes[i]);
            nodes[i] = NULL;
        }
    }
    
    ret = 0;
out:
    return ret;
}

