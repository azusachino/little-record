# 算法

## 数据结构与算法总览

### 精通一个领域 (semantic tree)

- chunk it up (chapter, part)
- deliberately practicing (练五遍)
- feedback (crack questions)

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

### 递归

画出递归树 (找出重复部分 -> 做缓存)

### 主定理

![master](../resources/algorithm/master.png)

## 数组, 链表, 跳表

升维 -- 空间换时间

- array
  - prepend O(1)
  - append O(1)
  - lookup O(1)
  - insert O(n)
  - delete O(n)
- linkedlist
  - prepend O(1)
  - append O(1)
  - lookup O(n)
  - insert O(1)
  - delete O(1)
- 跳表 (skip list)
  - lookup O(logn)
  - insert O(logn)
  
### 1.实战解析: 移动零

1. 5-10分钟: 读题和思考
2. 有思路: 自己开始做和写代码; 不然, 马上看题解
3. 默写背诵, 熟练

## 栈和队列

- Stack (FILO); 添加, 删除皆为 O(1), 查询O(n)
- Queue (FIFO); 添加, 删除皆为 O(1), 查询O(n)
- Deque 添加, 删除皆为 O(1), 查询O(n)

### Stack

```java
Stack<Integer> stack = new Stack<>();
stack.push(1);
stack.push(2);
stack.search(1);
stack.pop();
stack.peek();
```

### Queue

```java
Queue<String> queue = new LinkedList<>();
queue.size();
queue.add("1");
queue.offer("one");
queue.remove();
queue.poll();
queue.element();
queue.peek();
```

### Deque

```java
Deque<Integer> deque = new LinkedList<>();
    deque.addFirst(1);
    deque.addLast(2);
    System.out.println(deque);

    int a = deque.getFirst();
    int b = deque.peekLast();

    while (deque.size() > 0) {
         System.out.println(deque.pollLast());
    }
```

### PriorityQueue

1. 插入操作: O(1)
2. 取出操作: O(logN) -- 按照元素的优先级取出
3. 底层具体实现的数据结构较为多样和复杂: heap, BST, treap
