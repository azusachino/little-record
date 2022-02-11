# 编程范式

1. 泛型编程
2. 函数式编程
3. 修饰器编程
4. 面向对象编程
5. 基于原型的编程
6. Go 的委托模式
7. 逻辑编程范式

屏蔽掉数据和操作数据的细节，让算法更为通用，让编程者更多地关注算法的结构，而不是在算法中处理不同的数据类型。

## C 语言的特性

1. C 语言是一个静态弱类型语言，在使用变量时需要声明变量类型，但是类型间可以有隐式转换；
2. 不同的变量类型可以用结构体（struct）组合在一起，以此来声明新的数据类型；
3. C 语言可以用  `typedef`  关键字来定义类型的别名，以此来达到变量类型的抽象；
4. C 语言是一个有结构化程序设计、具有变量作用域以及递归功能的过程式语言；
5. C 语言传递参数一般是以值传递，也可以传递指针；
6. 通过指针，C 语言可以容易地对内存进行低级控制，然而这引入了非常大的编程复杂度；
7. 编译预处理让 C 语言的编译更具有弹性，比如跨平台。

```c
void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
void swap(void* x, void* y, size_t size)
{
    char tmp[size];
    memcpy(tmp, y, size);
    memcpy(y, x, size);
    memcpy(x, tmp, size);
}
```

- **函数接口中增加了一个`size`参数**。因为，用了  `void*`  后，类型被“抽象”掉了，编译器不能通过类型得到类型的尺寸了，所以，需要我们手动地加上一个类型长度的标识。
- **函数的实现中使用了`memcpy()`函数**。还是因为类型被“抽象”掉了，所以不能用赋值表达式了，很有可能传进来的参数类型还是一个结构体，因此，为了要交换这些复杂类型的值，我们只能使用内存复制的方法了。
- **函数的实现中使用了一个`temp[size]`数组**。这就是交换数据时需要用的 buffer，用 buffer 来做临时的空间存储。

## C++解决 C 语言中的各种问题和不方便

- 用引用来解决指针的问题。
- 用 namespace 来解决名字空间冲突的问题。
- 通过 try-catch 来解决检查返回值编程的问题。
- 用 class 来解决对象的创建、复制、销毁的问题，从而可以达到在结构体嵌套时可以深度复制的内存安全问题。
- 通过重载操作符来达到操作上的泛型。
- 通过模板 template 和虚函数的多态以及运行时识别来达到更高层次的泛型和多态。
- 用 RAII、智能指针的方式，解决了 C 语言中因为需要释放资源而出现的那些非常 ugly 也很容易出错的代码的问题。
- 用 STL 解决了 C 语言中算法和数据结构的 N 多种坑。

## C++

- 通过类中的构造、析构、拷贝构造，重载赋值操作符，标准化（隐藏）了类型的内存分配、释放和复制的操作。
- 通过重载操作符，可以标准化类型的比较等操作。
- 通过 iostream，标准化了类型的输入输出控制。
- 通过模板技术（包括模板的特化），来为不同的类型生成类型专属的代码。
- 通过迭代器来标准化数据容器的遍历操作。
- 通过面向对象的接口依赖（虚函数技术），来标准化了特定类型在特定算法上的操作。
- 通过函数式（函数对象），来标准化对于不同类型的特定操作。

## 函数式编程

对于函数式编程来说，它只关心定义输入数据和输出数据相关的关系，数学表达式里面其实是在做一种映射（mapping），输入的数据和输出的数据关系是什么样的，是用函数来定义的。

- **first class function（头等函数）** ：这个技术可以让你的函数就像变量一样来使用。也就是说，你的函数可以像变量一样被创建、修改，并当成变量一样传递、返回，或是在函数中嵌套函数。
- **tail recursion optimization（尾递归优化）** ： 我们知道递归的害处，那就是如果递归很深的话，stack 受不了，并会导致性能大幅度下降。因此，我们使用尾递归优化技术——每次递归时都会重用 stack，这样能够提升性能。当然，这需要语言或编译器的支持。Python 就不支持。
- **map & reduce** ：这个技术不用多说了，函数式编程最常见的技术就是对一个集合做 Map 和 Reduce 操作。这比起过程式的语言来说，在代码上要更容易阅读。（传统过程式的语言需要使用 for/while 循环，然后在各种变量中把数据倒过来倒过去的）这个很像 C++ STL 中 foreach、find_if、count_if 等函数的玩法。
- **pipeline（管道）**：这个技术的意思是，将函数实例成一个一个的 action，然后将一组 action 放到一个数组或是列表中，再把数据传给这个 action list，数据就像一个 pipeline 一样顺序地被各个函数所操作，最终得到我们想要的结果。
- **recursing（递归）** ：递归最大的好处就简化代码，它可以把一个复杂的问题用很简单的代码描述出来。注意：递归的精髓是描述问题，而这正是函数式编程的精髓。
- **currying（柯里化）** ：将一个函数的多个参数分解成多个函数， 然后将函数多层封装起来，每层函数都返回一个函数去接收下一个参数，这可以简化函数的多个参数。在 C++ 中，这很像 STL 中的 bind1st 或是 bind2nd。
- **higher order function（高阶函数）**：所谓高阶函数就是函数当参数，把传入的函数做一个封装，然后返回这个封装函数。现象上就是函数传进传出，就像面向对象对象满天飞一样。这个技术用来做 Decorator 很不错。

## 用于解耦的技术

- **State Machine**
  - 状态定义
  - 状态变迁条件
  - 状态的 action
- **DSL – Domain Specific Language**
  - HTML, SQL , Unix Shell Script，AWK，正则表达式……
- **编程范式**
  - 面向对象：委托、策略、桥接、修饰、IoC/DIP、MVC……
  - 函数式编程：修饰、管道、拼装
  - 逻辑推导式编程：Prolog

---

An algorithm can be regarded as consisting of a logic component, which specifies the knowledge to be used in solving problems, and a control component, which determines the problem-solving strategies by means of which that knowledge is used. The logic component determines the meaning of the algorithm whereas the control component only affects its efficiency. The efficiency of an algorithm can often be improved by improving the control component without changing the logic of the algorithm. We argue that computer programs would be more often correct and more easily improved and modified if their logic and control aspects were identified and separated in the program text.

- **Logic 部分才是真正有意义的（What）**
- **Control 部分只是影响 Logic 部分的效率（How）**
