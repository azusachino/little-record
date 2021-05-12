# Mysql

## MySQL：ERROR 1067 (42000): Invalid default value for 'end_time'

### 错误分析

表中的第一个 TIMESTAMP 列（如果未声明为 NULL 或显示 DEFAULT 或 ON UPDATE 子句）将自动分配 DEFAULT CURRENT_TIMESTAMP 和 ON UPDATE CURRENT_TIMESTAMP 属性

第一个之后的 TIMESTAMP 列（如果未声明为 NULL 或显示 DEFAULT 子句）将自动分配 DEFAULT '0000-00-00 00:00:00'（零时间戳），这不满足 sql_mode 中的 NO_ZERO_DATE 而报错。

> 注：sql_mode 有两种，一种是空值，一种是严格模式，会给出很多默认设置。在 MySQL5.7 之后默认使用严格模式。

NO_ZERO_DATE：若设置该值，MySQL 数据库不允许插入零日期，插入零日期会抛出错误而不是警告。

### 解决方式

- 方式一：先执行 select @@sql_mode,复制查询出来的值并将其中的 NO_ZERO_DATE 删除，然后执行 set sql_mode = '修改后的值'。此方法只在当前会话中生效。
- 方式二：先执行 select @@global.sql_mode,复制查询出来的值并将其中的 NO_ZERO_DATE 删除，然后执行 set global sql_mode = '修改后的值'。此方法在当前服务中生效，重新 MySQL 服务后失效。
- 方法三：在 mysql 的安装目录下，打开 my.ini 或 my.cnf 文件，新增  sql_mode = ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION，然后重启 mysql。此方法永久生效。
