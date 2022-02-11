# ELK Related

## search in springboot

```java
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

## 文件夹分组

es 内部的文件夹需要特殊权限，或者以`user: root`模式启动

`chown -R 1000:1000 folder`

## ES 添加 template

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

注意：**插件版本必须与 ES 版本一致**，例如[elasticsearch-analysis-ik](https://github.com/medcl/elasticsearch-analysis-ik)。

1. 通过执行`bin/elasticsearch-plugin install [plugin_name]`，安装完成后需要重启 ES (在线安装)
2. 下载完成之后，解压到`${ES_FOLDER}/plugins`即可 (离线安装)
