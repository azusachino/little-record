# ELK EXP

## Search in SpringBoot

```java
    public static final String[] FETCH_FIELDS = ["@timestamp", "some_field"];

    @Test
    void search() throws IOException {
        SearchRequest sr = new SearchRequest();
        SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
        BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();
        // using `must`
        boolQueryBuilder.must(QueryBuilders.matchAllQuery());
        // filter only error message
        boolQueryBuilder.filter(QueryBuilders.matchQuery("message", "ERROR"));

        // under must using range with customized format
        boolQueryBuilder.must(QueryBuilders.rangeQuery("@timestamp").gte("prev").lte("next").format("yyyy-MM-dd HH:mm:ss SSS"));

        searchSourceBuilder.query(boolQueryBuilder);
        searchSourceBuilder.fetchSource(FETCH_FIELDS, Strings.EMPTY_ARRAY);
        // sort by naming sort field
        searchSourceBuilder.sort("@timestamp");
        // using from and size => simulate pagination
        searchSourceBuilder.from(1);
        searchSourceBuilder.size(10);

        sr.indices(INDEX);
        sr.source(searchSourceBuilder);

        SearchResponse searchResponse = restHighLevelClient.search(sr, RequestOptions.DEFAULT);
        if (searchResponse.getHits().getTotalHits().value > 0) {

            LOGGER.info("total count: {}", searchResponse.getHits().getTotalHits().value);

            for (SearchHit hit : searchResponse.getHits()) {
                System.out.println("Match: ");
                for (String fetchField : FETCH_FIELDS) {
                    System.out.println(" - " + fetchField + " " + hit.getSourceAsMap().get(fetchField));
                }
            }
        } else {
            System.out.println("No results matching the criteria.");
        }
        restHighLevelClient.close();
    }
```

## ES DOCKER 权限相关问题

es 内部的文件夹需要特殊权限，或者以`user: root`模式启动

`chown -R 1000:1000 folder`

## 向 ES 添加 template

1. 首先可以访问 `ES:9200/_cat/templates` 查看 ES 中的模板列表
2. 通过 `ES:9200/_template/xxx` 获取特定的配置
3. 下载 `logstash`，并在其基础上进行修改
4. 通过 `PUT ES:9200/_template/yourTemplateName` 添加自定义的 template

## logstash 配置模板输出到 elasticsearch

首先需要定义执行任务的 pipeline

```yml
# config/pipelines.yml
- pipeline.id: spring-boot
  config.string: |
    input { 
      beats {
        port => "5044"
      }
    }
    filter {
      grok {
        match => [
          "message", "(?<timestamp>\d{4}-\d{2}-\d{2}\s\d{2}:\d{2}:\d{2}.\d{3}) \[%{NOTSPACE:thread}\] %{NOTSPACE:level} %{NOTSPACE:class} - %{GREEDYDATA:msg}"
        ]
      }
      # 如果匹配失败，则不进行采集
      if "_grokparsefailure" in [tags] {
        drop {}
      }
      # 指定记录的时间戳字段
      date {
        match => [ "timestamp", "yyyy-MM-dd HH:mm:ss.SSS", "ISO8601" ]
        target => "@timestamp"
      }
      mutate {
        remove_field => [ "timestamp", "message" ]
      }  
    }
    output {           
      elasticsearch {
        hosts => ["http://elasticsearch:9200"]
        # recommended for indexing time series datasets (such as logs, metrics, and events)
        # data_stream => "true"
        index => "spring-%{+YYYY.MM.dd}"
        template_name => "spring-boot"
        template_overwrite => true
        template => "/usr/share/logstash/template/spring-boot.json"
      }
    }
```

然后是具体的模板配置，介绍几个重要的知识点：

注意：**ES 版本不同，配置文件也有相应的变化**

- 通过配置`dynamic_templates`，可以动态匹配 mappings.properties 之外的属性；否则出现多余字段
- `analyzer`和`search_analyzer`使用不同的分词器达到更高的效率

```json
{
  "order": 0,
  "version": 60001,
  "index_patterns": ["spring-*"],
  "settings": {
    "index": {
      "number_of_shards": "1",
      "number_of_replicas": "0",
      "refresh_interval": "60s",
      "lifecycle": {
        "name": "spring-policy",
        "rollover_alias": "spring"
      },
      "translog": {
        "durability": "async",
        "sync_interval": "60s"
      }
    }
  },
  "mappings": {
    "dynamic_templates": [
      {
        "message_field": {
          "path_match": "message",
          "mapping": {
            "norms": false,
            "type": "text"
          },
          "match_mapping_type": "string"
        }
      },
      {
        "string_fields": {
          "mapping": {
            "norms": false,
            "type": "keyword"
          },
          "match_mapping_type": "string",
          "match": "*"
        }
      }
    ],
    "properties": {
      "@timestamp": {
        "type": "date"
      },
      "@version": {
        "type": "keyword"
      },
      "agent": {
        "properties": {
          "ephemeral_id": {
            "type": "keyword"
          },
          "hostname": {
            "type": "keyword"
          },
          "id": {
            "type": "keyword"
          },
          "name": {
            "type": "keyword"
          },
          "type": {
            "type": "keyword"
          },
          "version": {
            "type": "keyword"
          }
        }
      },
      "class": {
        "type": "keyword"
      },
      "container": {
        "properties": {
          "id": {
            "type": "keyword"
          }
        }
      },
      "ecs": {
        "properties": {
          "version": {
            "type": "keyword"
          }
        }
      },
      "fields": {
        "properties": {
          "model": {
            "type": "keyword"
          }
        }
      },
      "host": {
        "properties": {
          "architecture": {
            "type": "keyword"
          },
          "containerized": {
            "type": "boolean"
          },
          "hostname": {
            "type": "keyword"
          },
          "id": {
            "type": "keyword"
          },
          "ip": {
            "type": "keyword"
          },
          "mac": {
            "type": "keyword"
          },
          "name": {
            "type": "keyword"
          },
          "os": {
            "properties": {
              "codename": {
                "type": "keyword"
              },
              "family": {
                "type": "keyword"
              },
              "kernel": {
                "type": "keyword"
              },
              "name": {
                "type": "keyword"
              },
              "platform": {
                "type": "keyword"
              },
              "type": {
                "type": "keyword"
              },
              "version": {
                "type": "keyword"
              }
            }
          }
        }
      },
      "input": {
        "properties": {
          "type": {
            "type": "keyword"
          }
        }
      },
      "level": {
        "type": "keyword"
      },
      "log": {
        "properties": {
          "file": {
            "properties": {
              "path": {
                "type": "keyword"
              }
            }
          },
          "offset": {
            "type": "long"
          }
        }
      },
      "msg": {
        "type": "text",
        "analyzer": "ik_max_word",
        "search_analyzer": "ik_smart",
        "fields": {
          "keyword": {
            "type": "keyword",
            "ignore_above": 256
          }
        }
      },
      "tags": {
        "type": "keyword"
      },
      "thread": {
        "type": "keyword"
      }
    }
  },
  "aliases": {}
}
```

## 安装插件

注意：**插件版本必须与 ES 版本一致**，例如[elasticsearch-analysis-ik](https://github.com/medcl/elasticsearch-analysis-ik)。以下是安装的两种方式：

1. 通过执行`bin/elasticsearch-plugin install [plugin_name]`，安装完成后需要重启 ES (在线安装)
2. 下载完成之后，解压到`${ES_FOLDER}/plugins`即可 (离线安装)

## 性能调优

### 硬件方面

1. 内存： 64G 最理想，16-32 也可以，8G 内存可能适得其反
2. CPU：需要 2 到 8 核
3. 硬盘：SSD 最好，避免 NAS
4. 网络：避免跨多个数据中心的集群

### 索引优化

1. 批量提交
2. 增加 refresh 时间间隔 `index.refresh_interval`
3. 调整索引缓存配置 `indices.memory.index_buffer_size: 10%`, `indices.memory.min_index_buffer_size: 48mb`
4. 调整 translog 配置 `index.translog.sync_interval: 5s`, `index.translog.flush_threshold_size: 512mb`
5. 注意 `_id` 字段的使用
6. 注意 `_all` 和 `_source` 字段的使用
7. 合理配置`index`的属性
8. 减少副本数量

### 操作系统

1. 较大的文件描述符，例如 64000
2. 设置 MMAP `sysctl -w vm.max_map_count=655300` 或者设置 `/etc/sysctl.conf`

### JVM

1. 不要调整 JVM 参数
2. 不要修改垃圾收集器
3. 给 Lucene 留下一半的内存空间
4. 不要超过 32G
5. 避免使用过大内存
6. 最好禁用系统 swapping

### ES 参数

1. 禁止调整线程池
2. 设置集群和节点名称
3. 设置路径
4. 设置 minimum_master_nodes

```sh
ES_JAVA_OPTS 配置ES JVM heap内存限制
cluster.name   集群名称
node.name 节点名称
node.master 节点角色配置，true表示可以成为主节点，false不能成为主节点
node.data   节点角色配置，true表示可以成为数据节点，false不能成为数据节点
node.ingest 节点角色配置，true表示可以成为协调节点，false不能成为协调节点（简单解释，请以官方为准）
node.attr.rack  节点服务器所在的机柜信息，可能在数据分布中起到指导作用
discovery.zen.ping.unicast.hosts 配置自动发现IP列表
discovery.zen.minimum_master_nodes 防止脑裂，这个参数控制的是，一个节点需要看到的具有master节点资格的最小数量，然后才能在集群中做操作。官方的推荐值是(N/2)+1，其中N是具有master资格的节点的数量（我们的情况是3，因此这个参数设置为2，但对于只有2个节点的情况，设置为2就有些问题了，一个节点DOWN掉后，你肯定连不上2台服务器了，这点需要注意）。
gateway.recover_after_nodes 控制集群在达到多少个节点之后才会开始数据恢复,通过这个设置可以避免集群自动相互发现的初期,shard分片不全的问题,假如es集群内一共有5个节点,就可以设置为5,那么这个集群必须有5个节点启动后才会开始数据分片,如果设置为3,就有可能另外两个节点没存储数据分片
network.host 绑定服务的IP地址
transport.tcp.port 内部通信端口
http.port 对外服务端口
path.data 数据存放目录
bootstrap.memory_lock 锁住内存，确保ES不使用swap
bootstrap.system_call_filter        系统调用过滤器，建议禁用该项检查，因为很多检查项需要Linux 3.5以上的内核，否则会报错。
```

## 部署 ES

1.内核参数优化

```sh
vim /etc/sysctl.conf

vm.max_map_count = 655300
vm.swappiness = 1
```

2.执行 docker

```sh
#!/bin/bash
# 删除已退出的同名容器
docker ps -a | grep es_master |egrep "Exited|Created" | awk '{print $1}'|xargs -i% docker rm -f % 2>/dev/null
# 启动
docker run --name es_master \
        -d --net=host \
        --restart=always \
        --privileged=true \
        --ulimit nofile=655350 \
        --ulimit memlock=-1 \
        --memory=12G \
        --memory-swap=-1 \
        --cpuset-cpus='31-34' \
        --volume /data:/data \
        --volume /etc/localtime:/etc/localtime \
        -e TERM=dumb \
        -e ES_JAVA_OPTS="-Xms8g -Xmx8g" \
        -e cluster.name="iyunwei" \
        -e node.name="MASTER-100" \
        -e node.master=true \
        -e node.data=false \
        -e node.ingest=false \
        -e node.attr.rack="0402-K03" \
        -e discovery.zen.ping.unicast.hosts="192.168.2.100:9301,192.168.2.101:9301,192.168.2.102:9301,192.168.2.100:9300,192.168.2.102:9300,192.168.2.103:9300,192.168.3.100:9300,192.168.3.101:9300,192.168.3.102:9300,192.168.3.103:9300" \
        -e discovery.zen.minimum_master_nodes=2 \
        -e gateway.recover_after_nodes=5 \
        -e network.host=0.0.0.0 \
        -e transport.tcp.port=9301 \
        -e http.port=9201 \
        -e path.data="/data/iyunwei/master" \
        -e path.logs=/data/elastic/logs \
        -e bootstrap.memory_lock=true \
        -e bootstrap.system_call_filter=false \
        -e indices.fielddata.cache.size="25%" \
```

## es sort

with special field name for `_score` to sort by score, and `_doc` to sort by index order.

## ik

基于 ik 分词器，增加 filter

```json
{
  "settings": {
    "index.number_of_shards": 3,
    "index.max_result_window": 20000000,
    "number_of_replicas": 1,
    "index.refresh_interval": "60s",
    "index.highlight.max_analyzed_offset": "10000",
    "analysis": {
      "analyzer": {
        "my_analyzer": {
          "tokenizer": "ik_max_word",
          "char_filter": ["camel_case_filter", "special_character_filter"]
        }
      },
      "char_filter": {
        "camel_case_filter": {
          "type": "pattern_replace",
          "pattern": "(?<=\\p{Lower})(?=\\p{Upper})",
          "replacement": " "
        },
        "special_character_filter": {
          "type": "pattern_replace",
          "pattern": "(?:\\p{Punct})",
          "replacement": " "
        }
      }
    }
  },
  "mappings": {
    "record": {
      "_all": { "enabled": false },
      "dynamic": true,
      "date_detection": true,
      "properties": {
        "id": {
          "type": "keyword",
          "index": true
        },
        "record": {
          "type": "text",
          "index": "true",
          "analyzer": "my_analyzer",
          "search_analyzer": "ik_smart",
          "fields": {
            "exact": {
              "type": "keyword"
            }
          },
          "fielddata": true
        }
      }
    }
  }
}
```

## setup ilm sample

```http
PUT _ilm/policy/one-minute-rollover
{
  "policy": {
    "phases": {
      "hot": {
        "min_age": "0ms",
        "actions": {
          "rollover": {
            "max_size": "10mb",
            "max_age": "1m"
          },
          "set_priority": {
            "priority": 100
          }
        }
      },
      "delete": {
        "min_age": "1h",
        "actions": {
          "delete": {
            "delete_searchable_snapshot": true
          }
        }
      }
    }
  }
}

PUT _template/iris-template
{
  "index_patterns": ["iris-*"],
  "settings": {
    "number_of_shards": 1,
    "number_of_replicas": 0,
    "index.lifecycle.name": "one-minute-rollover",
    "index.lifecycle.rollover_alias": "iris"
  }
}

PUT iris-00000001
{
  "aliases": {
    "iris": {
      "is_write_index": true
    }
  }
}

POST iris/_doc
{
    "message":"hello world"
}

GET iris-*/_ilm/explain
```

**result**:

```sh
green  open   iris-000001                   EsqnOHQFTXyQBkobkqUpxQ   1   0         21            0        5kb            5kb
green  open   iris-000002                     OEp7t9ymTKOdrnQoUCrnEw   1   0         51            0      5.9kb          5.9kb
```

### using ILM with Logstash

```conf
output {
  elasticsearch {
    hosts => ["localhost:9200"]
    ilm_rollover_alias => "iris" # 将会附在ilm_rollover_alias的值后面共同构成索引名，myindex-00001; using iris as query alias
    ilm_pattern => "000001" # 使用的索引策略
    ilm_policy => "one-minute-rollover" # 使用的索引模版名称
    template_name => "iris-template"
  }
}
```

### force rollover

When updated mapping, and we need apply with the new mapping, we can force rollover.  
In case of search mechanism don't take effect with new format of data.

Creates a new index for a data stream or index alias.

```sh
POST <index> | <alias>/_rollover
```

## beats usage

```yml
#把Beats自身的日志记录调到最低级别降低
logging.level: error
logging.to_stderr: true

#开启本地默认的端点缓存行为
queue.disk:
  max_size: 10GB

#启用端点的监控
monitoring:
  enabled: false
```

### filebeat.yml

```yml
#=========================== Filebeat inputs =============================
filebeat.inputs:
  - type: log
    enabled: false
    paths:
      - /var/log/*.log
    ignore_older: 24h # 不采集24h内无更新的日志
    close_removed: true # 当文件被删除时，filebeat关闭文件的读取处理
    close_renamed: true # 当文件重命名或者rotated，关闭句柄
    clean_removed: true # 当文件在磁盘中找不到时，清理注册表中的文件信息
    # multiline.type: pattern # 匹配多行规则
    # multiline.pattern: '^\d{4}-\d{2}-\d{2}' # 通过日期匹配真实的日志，堆栈信息返回false
    # multiline.negate: true
    # multiline.match: after
    # exclude_lines: ["DEBUG"]
    fields:
      log_topic: scv-server
      host_ip: 172.31.103.161
#============================= Filebeat modules ===============================
filebeat.config.modules:
  path: ${path.config}/modules.d/*.yml
  reload.enabled: true
  reload.period: 60s

#-------------------------- Elasticsearch output ------------------------------
output.elasticsearch:
  hosts: ["${INT_ES_SRV}"]
  password: ${BEATS_WRITER_PW}
  username: ${BEATS_WRITER_USERNAME}

#================================ Processors =====================================
processors:
  - add_host_metadata:
      netinfo.enabled: true
      cache.ttl: 5m
  - add_cloud_metadata: ~
  - add_docker_metadata: ~
  - add_kubernetes_metadata: ~
  - add_fields:
      target: ""
      fields:
        service.name: "Joint Lab"
        service.id: "es-qq"
```

### metricbeat.yml

```yml
# =========================== Modules configuration ============================
metricbeat.config.modules:
  path: ${path.config}/modules.d/*.yml
  reload.enabled: true
  reload.period: 10s

#-------------------------- Elasticsearch output ------------------------------
output.elasticsearch:
  hosts: ["${INT_ES_SRV}"]
  password: ${BEATS_WRITER_PW}
  username: ${BEATS_WRITER_USERNAME}

#================================ Processors =====================================
processors:
  - add_host_metadata:
      netinfo.enabled: true
      cache.ttl: 5m
  - add_cloud_metadata: ~
  - add_docker_metadata: ~
  - add_kubernetes_metadata: ~
  - add_fields:
      target: ""
      fields:
        service.name: "Joint Lab"
        service.id: "es-qq"

#==================== Best Practice Configuration ==========================
setup.ilm.check_exists: false
logging.level: error
queue.spool: ~
monitoring:
  enabled: true
```
