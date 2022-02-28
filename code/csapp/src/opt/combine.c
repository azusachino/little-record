#include "opt/vec.h"

#define INDENT 0
#define OP +
// #define INDENT 1
// #define OP *

void combine1(vec_ptr v, data_t *dest)
{
    long i;
    *dest = INDENT;

    for (i = 0; i < vec_length(v); i++)
    {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

// 清除循环中的不必要计算
void combine2(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);

    *dest = INDENT;

    for (i = 0; i < length; i++)
    {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

// 尝试使用 raw data
void combine3(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    *dest = INDENT;

    for (i = 0; i < length; i++)
    {
        *dest = *dest OP data[i];
    }
}
// combine3 部分汇编代码
// Inner loop of combine3. data_t = double, OP = *
// dest in %rbx, data+i in %rdx, data+length in %rax
// .L17:                           loop:
//   vmovsd (%rbx), %xmm0             Read product from dest
//   vmulsd (%rdx), %xmm0, %xmm0      Multiply product by data[i]
//   vmovsd %xmm0, (%rbx)             Store product at dest
//   addq $8, %rbx                    Increment data+i
//   cmpq %rax, %rdx                  Compare to data+length
//   jne   .L17                       if !=, goto loop

// 消除多余的内存读写 *dest
void combine4(vec_ptr v, data_t *dest)
{
    long i;
    long length = vec_length(v);
    data_t *data = get_vec_start(v);
    data_t acc = INDENT;

    for (i = 0; i < length; i++)
    {
        acc = acc OP data[i];
    }
    *dest = acc;
}

// combine4 部分汇编代码
// Inner loop of combine3. data_t = double, OP = *
// acc in %xmm0, data+i in %rdx, data+length in %rax
// .L25:                           loop:
//   vmulsd (%rdx), %xmm0, %xmm0     Multiply acc by data[i]
//   addq $8, %rbx                   Increment data+i
//   cmpq %rax, %rdx                 Compare to data+length
//   jne   .L25                      if !=, goto loop