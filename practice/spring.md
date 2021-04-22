# Spring 相关实践

## `@Cacheable`

- cacheNames/value ：用来指定缓存组件的名字
- key ：缓存数据时使用的 key，可以用它来指定。默认是使用方法参数的值。（这个 key 你可以使用 spEL 表达式来编写）
- keyGenerator ：key 的生成器。 key 和 keyGenerator 二选一使用
- cacheManager ：可以用来指定缓存管理器。从哪个缓存管理器里面获取缓存。
- condition ：可以用来指定符合条件的情况下才缓存
- unless ：否定缓存。当 unless 指定的条件为 true ，方法的返回值就不会被缓存。当然你也可以获取到结果进行判断。（通过 #result 获取方法结果）
- sync ：是否使用异步模式。
