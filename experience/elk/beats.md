# Beats

```yml
#禁用索引 ilm 策略检查，避免无用动作
setup.ilm.check_exists: false

#把Beats自身的日志记录调到最低级别降低
logging.level: error

#开启本地默认的端点缓存行为
queue.spool: ~

#启用端点的监控
monitoring:
  enabled: true
```

## filebeat.yml

```yml
#=========================== Filebeat inputs =============================
filebeat.inputs:
  - type: log
    enabled: false
    paths:
      - /var/log/*.log

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

#==================== Best Practice Configuration ==========================
setup.ilm.check_exists: false
logging.level: error
queue.spool: ~
monitoring:
  enabled: true
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
