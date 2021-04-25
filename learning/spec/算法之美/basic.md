# 数据结构与算法之美

建立时间复杂度、空间复杂度意识，写出高质量的代码，能够设计基础架构，提升编程技能，训练逻辑思维，积攒人生经验，以此获得工作回报，实现你的价值，完善你的人生。

![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/aa81002a-8e4f-48f8-91b9-2e77dab03a40/Untitled.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/aa81002a-8e4f-48f8-91b9-2e77dab03a40/Untitled.png)

1. 边学边练，适度刷题
2. 多问、多思考、多互动
3. 打怪升级学习法
4. 知识需要沉淀，不要想试图一下子掌握所有

## 复杂度分析

1. 只关注循环执行次数最多的一段代码
2. 加法法则：总复杂度等于量级最大的那段代码的复杂度
3. 乘法法则：嵌套代码的复杂度等于嵌套内外代码复杂度的乘积

![https://s3-us-west-2.amazonaws.com/secure.notion-static.com/16ee7adc-cc19-498b-9f89-e4bcb4a7b431/Untitled.png](https://s3-us-west-2.amazonaws.com/secure.notion-static.com/16ee7adc-cc19-498b-9f89-e4bcb4a7b431/Untitled.png)

- 最好情况时间复杂度就是，在最理想的情况下，执行这段代码的时间复杂度
- 最坏情况时间复杂度就是，在最糟糕的情况下，执行这段代码的时间复杂度
- 平均时间复杂度
- 均摊时间复杂度

## 数组

数组（Array）是一种线性表数据结构。它用一组连续的内存空间，来存储一组具有相同类型的数据。

数组下标为何从 0 开始？

在 c++中，auto p = arr; 此时 p 获取的是 arr[0]的引用，即第一个位置的引用
a[k]\_address = base_addr + k \* type_size;
C 语言中的数组越界会造成不可预知的问题

## 链表

链表与指针：

1. 理解指针或引用的含义
2. 警惕指针丢失和内存泄漏
3. 利用哨兵简化实现难度
4. 重点留意边界条件处理
5. 举例画图，辅助思考
6. 多练多写

---

- 单链表反转
- 链表中环的检测
- 两个有序的链表合并
- 删除链表中倒数第 n 个节点
- 求链表的中间节点

## 排序算法

稳定排序算法可以保持金额相同的两个对象，在排序之后的相对顺序不变。
冒泡排序是原地排序、稳定排序。最好是 O(n)，最坏是 O(n^2)。
插入排序是原地排序、稳定排序。最好是 O(n)，最坏是 O(n^2)。
选择排序是原地排序、不稳定排序。最好和最坏是 O(n^2)。

## 线性排序

## 二分查找

二分查找针对的是一个有序的数据集合，查找思想有点类似分治思想。每次都通过跟区间的中间元素对比，将待查找的区间缩小为之前的一半，直到找到要查找的元素，或者区间被缩小为 0。

```java
public int bsearch(int[] a, int n, int value) {
  int low = 0;
  int high = n - 1;

  while (low <= high) {
    int mid = (low + high) / 2;
    if (a[mid] == value) {
      return mid;
    } else if (a[mid] < value) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return -1;
}
```

## Skip List
