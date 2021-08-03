# Index Lifecycle Management

To automate rollover and management of a data stream with ILM:

1. Create a lifecycle policy that defines the appropriate phases and actions.
2. Create an index template to create the data stream and apply the ILM policy and the indices settings and mappings configurations for the backing indices.
3. Verify indices are moving through the lifecycle phases as expected.

## Create a lifecycle policy

A lifecycle policy specifies the phases in the index lifecycle and the actions to perform in each phase. A lifecycle can have up to five phases: `hot`, `warm`, `cold`, `frozen`, and `delete`.

```http
PUT _ilm/policy/timeseries_policy
{
  "policy": {
    "phases": {
      "hot": {
        "actions": {
          "rollover": {
            "max_primary_shard_size": "50GB",
            "max_age": "30d"
          }
        }
      },
      "delete": {
        "min_age": "90d",
        "actions": {
          "delete": {}
        }
      }
    }
  }
}
```

- The `min_age` defaults to 0ms, so new indices enter the `hot` phase immediately.
- Trigger the `rollover` action when either of the conditions are met.
- Move the index into the `delete` phase 90 days after `rollover`.
- Trigger the `delete` action when the index enters the delete phase.

## Create an index template to create the data stream and apply the lifecycle policy

```http
PUT _index_template/timeseries_template
{
  "index_patterns": ["timeseries"],
  "data_stream": { },
  "template": {
    "settings": {
      "number_of_shards": 1,
      "number_of_replicas": 1,
      "index.lifecycle.name": "timeseries_policy"
    }
  }
}
```

## Create the data stream

the following request creates the timeseries data stream and the first generation backing index called .ds-timeseries-2099.03.08-000001.

```http
POST timeseries/_doc
{
  "message": "logged the request",
  "@timestamp": "1591890611"
}
```

## Check lifecycle progress

```http
GET .ds-timeseries-*/_ilm/explain

{
  "indices": {
    ".ds-timeseries-2099.03.07-000001": {
      "index": ".ds-timeseries-2099.03.07-000001",
      "managed": true,
      "policy": "timeseries_policy",
      "lifecycle_date_millis": 1538475653281,
      "age": "30s",
      "phase": "hot",
      "phase_time_millis": 1538475653317,
      "action": "rollover",
      "action_time_millis": 1538475653317,
      "step": "check-rollover-ready",
      "step_time_millis": 1538475653317,
      "phase_execution": {
        "policy": "timeseries_policy",
        "phase_definition": {
          "min_age": "0ms",
          "actions": {
            "rollover": {
              "max_primary_shard_size": "50gb",
              "max_age": "30d"
            }
          }
        },
        "version": 1,
        "modified_date_in_millis": 1539609701576
      }
    }
  }
}
```

## Create an index template to apply the lifecycle policy

```http
PUT _index_template/timeseries_template
{
  "index_patterns": ["timeseries-*"],
  "template": {
    "settings": {
      "number_of_shards": 1,
      "number_of_replicas": 1,
      "index.lifecycle.name": "timeseries_policy",
      "index.lifecycle.rollover_alias": "timeseries"
    }
  }
}
```

## Bootstrap the initial time series index with a write index alias

```http
PUT timeseries-000001
{
  "aliases": {
    "timeseries": {
      "is_write_index": true
    }
  }
}
```
