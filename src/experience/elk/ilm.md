# Index Lifecycle Management

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

## using with Logstash

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

## force rollover

When updated mapping, and we need apply with the new mapping, we can force rollover.  
In case of search mechanism don't take effect with new format of data.

Creates a new index for a data stream or index alias.

```sh
POST <index> | <alias>/_rollover
```
