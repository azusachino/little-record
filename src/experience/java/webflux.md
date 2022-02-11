# Webflux & Netty

## Fire and Forget Mechanism

**If your fire-and-forget is already async returning Mono/Flux**:

```java
public Flux<Data> search(SearchRequest request)
{
    return searchService.search(request)
                        .collectList()
                        .doOnNext(data -> doThisAsync(data).subscribe())  // add error logging here or inside doThisAsync
                        .flatMapMany(Flux::fromIterable);
}

public Mono<Void> doThisAsync(List<Data> data) {
    //do some async/non-blocking processing here like calling WebClient
}
```

**If your fire-and-forget does blocking I/O**:

```java
public Flux<Data> search(SearchRequest request)
{
    return searchService.search(request)
                        .collectList()
                        .doOnNext(data -> Mono.fromRunnable(() -> doThisAsync(data))
                                              .subscribeOn(Schedulers.elastic())  // delegate to proper thread to not block main flow
                                              .subscribe())  // add error logging here or inside doThisAsync
                        .flatMapMany(Flux::fromIterable);
}

public void doThisAsync(List<Data> data) {
    //do some blocking I/O on calling thread
}
```
