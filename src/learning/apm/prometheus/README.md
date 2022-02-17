# Prometheus

## prometheus with thanos

![arch](https://cdn.buttercms.com/MgmtWUMeQOyJomANiEin)

### Thanos sidecar

- The main component that runs along Prometheus
- Reads and archives data on the object store
- Manages Prometheus’s configuration and lifecycle
- Injects external labels into the Prometheus configuration to distinguish each Prometheus instance
- Can run queries on Prometheus servers’ PromQL interfaces
- Listens in on Thanos gRPC protocol and translates queries between gRPC and REST

### Thanos store

- Implements the Store API on top of historical data in an object storage bucket
- Acts primarily as an API gateway and therefore does not need significant amounts of local disk space
- Joins a Thanos cluster on startup and advertises the data it can access
- Keeps a small amount of information about all remote blocks on a local disk in sync with the bucket
- This data is generally safe to delete across restarts at the cost of increased startup times

### Thanos query

- Listens in on HTTP and translates queries to Thanos gRPC format
- Aggregates the query result from different sources, and can read data from Sidecar and Store
- In HA setup, Thanos Query even de-duplicates the result

### Thanos compact

- Applies the compaction procedure of the Prometheus 2.0 storage engine to block data in object storage
- Generally not concurrent with safe semantics and must be deployed as a singleton against a bucket
- Responsible for down sampling data: 5 minute down sampling after 40 hours and 1 hour down sampling after 10 days

### Thanos ruler

Thanos Ruler basically does the same thing as the querier but for Prometheus’ rules. The only difference is that it can communicate with Thanos components.
