# SpringBoot

Micrometer+Prometheus+Grafana

## Prometheus.yml

```yml
global:
  # How frequently to scrape targets by default.
  [ scrape_interval: <duration> | default = 1m ]

  # How long until a scrape request times out.
  [ scrape_timeout: <duration> | default = 10s ]

  # How frequently to evaluate rules.
  [ evaluation_interval: <duration> | default = 1m ]

  # The labels to add to any time series or alerts when communicating with
  # external systems (federation, remote storage, Alertmanager).
  external_labels:
    [ <labelname>: <labelvalue> ... ]

  # File to which PromQL queries are logged.
  # Reloading the configuration will reopen the file.
  [ query_log_file: <string> ]

# Rule files specifies a list of globs. Rules and alerts are read from
# all matching files.
rule_files:
  [ - <filepath_glob> ... ]

# A list of scrape configurations.
scrape_configs:
  [ - <scrape_config> ... ]

# Alerting specifies settings related to the Alertmanager.
alerting:
  alert_relabel_configs:
    [ - <relabel_config> ... ]
  alertmanagers:
    [ - <alertmanager_config> ... ]

# Settings related to the remote write feature.
remote_write:
  [ - <remote_write> ... ]

# Settings related to the remote read feature.
remote_read:
  [ - <remote_read> ... ]
```

## AlertManager

```yml
global:
  resolve_timeout: 5m
  smtp_smarthost: ""
  smtp_from: "xxx@a.com"
  smtp_auth_username: ""
  smtp_auth_password: "xxx"
route:
  group_by: ["alertname"]
  group_wait: 10s
  group_interval: 10s
  repeat_interval: 1h
  reveiver: "mail-receiver"
receivers:
  - name: "mail-receiver"
    email_configs:
      - to: "xxx@a.comc"
  - name: "webhook"
    webhook_configs:
      - url: http://localhost:8060/dingtalk/webhook1/send
        send_resolve: true
inhibit_rules:
  - source_match:
      severity: "critical"
    target_math:
      severity: "warning"
    equal: ["alertname", "dev", "instance"]
```

## AlertRule

```yml
groups:
  - name: demo-alert-rule
    rules:
      - alert: DemoJobDown
        expr: sum(up{job="springboot-demo"}) == 0
        for: 1m
        labels:
          severity: critical
```
