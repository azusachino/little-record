# Using ILM

## Policy

```json
{
  "policy": {
    "phases": {
      "hot": {
        "actions": {
          "rollover": {
            "max_size": "30gb"
          }
        }
      },
      "delete": {
        "min_age": "30d",
        "actions": {
          "delete": {}
        }
      }
    }
  }
}
```

## Template

template name: ehr_prod_log_template

```json
{
  "index_patterns": ["ehr-prod-log-rollover-*"],
  "settings": {
    "number_of_shards": 1,
    "number_of_replicas": 1,
    "index.lifecycle.name": "ehr_prod_log_policy",
    "index.lifecycle.rollover_alias": "ehr-prod-log-rollover"
  }
}
```

## Index

Bootstrap `PUT ehr-prod-log-rollover-00000001`

```json
{
  "aliases": {
    "ehr-prod-log-rollover": {
      "is_write_index": true
    }
  }
}
```

## Configure Logstash

```conf
output {
  elasticsearch {
    hosts => ["**"]
    ilm_rollover_alias => "ehr-prod-log-rollover" //将会附在ilm_rollover_alias的值后面共同构成索引名，myindex-00001
    ilm_pattern => "00001" //使用的索引策略
    ilm_policy => "ehr_prod_log_policy" //使用的索引模版名称
    template_name => "ehr_prod_log_template"
  }
}
```

实战中，通过 logstash 将日志从 kafka 采集到 es，index => "ehr-prod-log-rollover"，这里使用别名，与之对应。

则在 es 中对应生成对应索引，ehr-prod-log-rollover-0000000n，而对外使用别名即可。

## Check Status

`GET ehr-prod-log-rollover-*/_ilm/explain`
