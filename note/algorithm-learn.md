# 算法

## 数据结构与算法总览

### 精通一个领域 (semantic tree)

- chunk it up
- deliberate practicing (练五遍)
- feedback

### 1.数据结构

- 一维数据结构
  - 基础: Array, LinkedList
  - 高级: Stack, Queue, Dequeue, Set, Map
- 二维数据结构
  - 基础: Tree, Graph
  - 高级: binary search tree(red-black, AVL), heap, disjoint set, Trie 字典树
- 特殊数据结构
  - 位运算Bitwise, 步隆过滤器(Bloom Filter)
  - LRU Cache

### 2.算法

- if-else, switch -- branch
- for,while loop -- iteration
- 递归Recursion(Divide & Conquer, Backtrace)

---

- Search: 深度优先搜索 Depth First Search, 广度优先搜索 Breadth First Search, A*, etc
- 动态规划 Dynamic Programming
- 二分查找 Binary Search
- 贪心 Greedy
- 数学 Math, 几何 Geometry

### 3.切题四件套

- Clarification
- Possible Solutions
  - compare (time/space)
  - optimal (enhance)
- Coding
- Test cases

### 其他建议

- 善用Google
- Iterm2 & zsh
- Vscode + leetcode plugin
- IDE的快捷方式 熟练
- 自顶向下的编程方式

## 复杂度分析

### 空间复杂度

### 时间复杂度

O(1): Constant Complexity 常数复杂度

```java
int n = 100;
pring("your input is " + n);
```

O(log n): Logaithmic Complexity 对数复杂度

```java
for (int i = 1; i <= n; i= i*2) {
    System.out.println("your input is: " + i)
}
```

O(n): Linear Complexity 线性时间复杂度

```java
for (int i =1; i<= n; i++) {
    System.out.println("your input is: " + i);
}
```

O(n^2): 2 square Complexity 平方

```java
for (int i =1; i<= n; i++) {
    for (int j = 1; j <=n; j++) {
        System.out.println("your input is: " + i +j);
    }
}
```

O(n^3): 3 square Complexity 立方

```java
for (int i =1; i<= n; i++) {
    for (int j = 1; j <=n; j++) {
        for (int a = 1; a <=n; a++) {
            System.out.println("your input is: " + i+j+a);
        }
    }
}
```

O(2^n): Exponential Growth 指数 (递归)

```java
int fib(int n) {
  if (n <= 2) return n;
  return fib(n-1) + fib(n-2);
}
```

O(n!): Factorial 阶乘

```java
for (int i =1; i<= n; i++) {
    System.out.println("your input is: " + i);
}
```

### 递归

画出递归树

### 主定理

![master](../resources/algorithm/master.png)
