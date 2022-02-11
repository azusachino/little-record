# Algorithms

- [Part1](https://www.coursera.org/learn/algorithms-part1)
- [Part2](https://www.coursera.org/learn/algorithms-part2)

## Data Types

### Union Find

**Steps to developing a usable algorithm**:

- Model the problem
- Find an algorithm to solve it
- Fast enough? Fits in memory?
- If not, figure out why
- Find a way to address the problem
- Iterate util satisfied

```java
public class UF {
    UF(int n);
    void union(int p, int q);
    boolean connected(int p, int q);
    int find(int p);
    int count();
}
```

#### Quick Find (eager approach)

```java
public class QuickFindUF {
    private int[] id;

    public QuickFindUF(int n) {
        id = new int[n];
        for (int i =0; i < n; i++) {
            id[i] = i;
        }
    }

    public boolean connected(int p, int q) {
        return id[p] == id[q];
    }

    public void union(int p, int q) {
        int pid = id[p];
        int qid = id[q];
        for (int i = 0; i <id.length; i++) {
            // change all entries with id[p] to id[q]
            if (id[i] == pid) {
                id[i] = qid;
            }
        }
    }
}
```

#### Quick Union (lazy approach)

**Data structure**:

- Integer array `id[]` of size n
- Interpretation: `id[i]` is parent of i
- Root of i is `id[id[id[i]]]`

**Find**: Check if p and q have the same root

**Union**: To merge components containing p and q, set the id of p's root to the id of q's root

```java
public class QuickUnionUF {
    private int[] id;

    public QuickUnionUF(int n) {
        id = new int[n];
        for (int i =0; i < n; i++) {
            id[i] = i;
        }
    }

    private int root(int i) {
        while (i != id[i]) {
            i = id[i];
        }
        return i;
    }

    public boolean connected(int p, int q) {
        return root(p) == root(q);
    }

    public void union(int p, int q) {
        int i = root(p);
        int j = root(q);
        id[i] = j;
    }
}
```

#### Improvement

**weighting**:

- modify quick-union to avoid all trees
- keep track of size of each tree
- balance by linking root of smaller tree to root of larger tree

```java
public void union(int p, int q) {
    int i = root(p);
    int j = root(q);
    if (i == j) {
        return;
    }
    if (sz[i] < sz[j]) {
        id[i] = j;
        sz[j] += sz[i];
    } else {
        id[j] = i;
        sz[i] += sz[j];
    }
}
```

**path compression**:

- quick union with path compression

```java
private int root(int i) {
    while (i != id[i]) {
        id[i] = id[id[i]];
        i = id[i];
    }
    return i;
}
```

### Analysis of Algorithms

- Predict performance
- Compare algorithms
- Provide guarantees
- Understand theoretical basis
- Primary practical reason: avoid performance bugs

### Queue & Stack

Using Linkedlist or Array

## Sorting

- Insertion Sort
- Selection Sort
- Shell Sort
- Merge Sort
- Quick Sort

## Searching

## Graphs

## String

## Advanced
