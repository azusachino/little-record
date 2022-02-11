# docker-compose

## `docker-compose stop`

1. firstly, send `SIGTERM` => kill 9555
2. after 10s (default), send `SIGKILL` => kill -9 9555

`docker-compose -t 30 iris_service`
