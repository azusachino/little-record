# Mybatis-plus

## 逻辑删除

### 逻辑删除实现

```java
@TableLogic
private Boolean deleted;
```

```yml
mybatis-plus:
  global-config:
    db-config:
      logic-not-delete-value: 0
      logic-delete-value: 1
      logic-delete-field: deleted
```

## 自动填充

元对象字段填充控制器抽象类，实现公共字段自动写入

`com.baomidou.mybatisplus.core.handlers.MetaObjectHandler`

```java
  /**
     * 插入元对象字段填充（用于插入时对公共字段的填充）
     *
     * @param metaObject 元对象
     */
    void insertFill(MetaObject metaObject);

    /**
     * 更新元对象字段填充（用于更新时对公共字段的填充）
     *
     * @param metaObject 元对象
     */
    void updateFill(MetaObject metaObject);
```

## 乐观锁

适合多读少写的场景

```java
    @Version
    private String version;
```

## 多租户

多租户定义：多租户技术或称多重租赁技术，简称SaaS，是一种软件架构技术，是实现如何在多用户环境下（此处的多用户一般是面向企业用户）共用相同的系统或程序组件，并且可确保各用户间数据的隔离性。简单讲：在一台服务器上运行单个应用实例，它为多个租户（客户）提供服务。从定义中我们可以理解：多租户是一种架构，目的是为了让多用户环境下使用同一套程序，且保证用户间数据隔离。那么重点就很浅显易懂了，多租户的重点就是同一套程序下实现多用户数据的隔离。

### 三种方案

1. 独立数据库: 隔离级别最高，安全性最好，但成本较高。
2. 共享数据库，独立 Schema
3. 共享数据库，共享 Schema，共享数据表

```java
    @Bean
    public PaginationInterceptor paginationInterceptor() {
        PaginationInterceptor interceptor = new PaginationInterceptor();

        List<ISqlParser> sqlParserList = new ArrayList<>();

        TenantSqlParser tenantSqlParser = new TenantSqlParser();
        tenantSqlParser.setTenantHandler(new MyTenantHandler());

        sqlParserList.add(tenantSqlParser);

        interceptor.setSqlParserList(sqlParserList);
        interceptor.setSqlParserFilter(new MySqlParserFilter());
        return interceptor;
    }
```

## 动态表名

```java
    @Bean
    public PaginationInterceptor paginationInterceptor() {
        PaginationInterceptor interceptor = new PaginationInterceptor();

        List<ISqlParser> sqlParserList = new ArrayList<>();

        DynamicTableNameParser dynamicTableNameParser = new DynamicTableNameParser();
        dynamicTableNameParser.setTableNameHandlerMap(Map.of());

        TenantSqlParser tenantSqlParser = new TenantSqlParser();
        tenantSqlParser.setTenantHandler(new MyTenantHandler());

        sqlParserList.add(tenantSqlParser);
        sqlParserList.add(dynamicTableNameParser);

        interceptor.setSqlParserList(sqlParserList);
        interceptor.setSqlParserFilter(new MySqlParserFilter());
        return interceptor;
    }
```

## sql注入器

1. 创建定义方法的类
2. 创建注入器
3. 在mapper中加入自定义方法

---

```java
public class Truncate extends AbstractMethod {
    /**
     * 注入自定义 MappedStatement
     *
     * @param mapperClass mapper 接口
     * @param modelClass  mapper 泛型
     * @param tableInfo   数据库表反射信息
     * @return MappedStatement
     */
    @Override
    public MappedStatement injectMappedStatement(Class<?> mapperClass, Class<?> modelClass, TableInfo tableInfo) {

        String sql = "";
        String method = "Truncate";

        SqlSource sqlSource= languageDriver.createSqlSource(configuration, sql, modelClass);
        return addDeleteMappedStatement(mapperClass, method, sqlSource);
    }
}

@Component
public class MySqlInjector extends DefaultSqlInjector {

    @Override
    public List<AbstractMethod> getMethodList(Class<?> mapperClass) {
        List<AbstractMethod> abstractMethods = super.getMethodList(mapperClass);
        abstractMethods.add(new Truncate());
        abstractMethods.add(new InsertBatchSomeColumn(t ->
                t.isLogicDelete() && t.isVersion()
        ));
        return abstractMethods;
    }
}
```
