# Beats

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

## filebeat.yml

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

## metricbeat.yml

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
