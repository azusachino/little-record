# Caddy server usage

## The api

caddy start

`curl localhost:2019/load -X POST -H "Content-Type: application/json" -d @caddy.json

```json
{
  "apps": {
    "http": {
      "servers": {
        "hello": {
          "listen": [":2015"],
          "routes": [
            {
              "handle": [
                {
                  "handler": "static_response",
                  "body": "Hello, world!"
                }
              ]
            }
          ]
        }
      }
    }
  }
}
```

`curl localhost:2015` for result

## With Caddyfile

```caddyfile
localhost {
    respond "Hello, world!"
}

localhost:2016 {
    respond "Goodbye, world!"
}
```

load the `Caddyfile` or `caddy reload`

```sh
curl localhost:2019/load \
    -X POST \
    -H "Content-Type: text/caddyfile" \
    --data-binary @Caddyfile
```

## static files

`--browse` to display file listing

`caddy file-server --listen :2015`

## Reverse proxy

`caddy reverse-proxy --form :2016 --to 127.0.0.1:9000`

or with Caddyfile

```caddyfile
:2016

reverse_proxy 127.0.0.1:9000
```

## https

```caddyfile
example.com

respond "hello, privacy!"
```
