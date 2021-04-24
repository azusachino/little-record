# ELK Related

## search in springboot

```java
    @Test
    void search() throws IOException {
        SearchRequest sr = new SearchRequest();
        SearchSourceBuilder searchSourceBuilder = new SearchSourceBuilder();
        BoolQueryBuilder boolQueryBuilder = QueryBuilders.boolQuery();
        // using `must`
        boolQueryBuilder.must(QueryBuilders.matchAllQuery());
        // filter only error message
        boolQueryBuilder.filter(QueryBuilders.matchQuery("message", "ERROR"));

        // under must using range with customized format
        boolQueryBuilder.must(QueryBuilders.rangeQuery("@timestamp").gte("prev").lte("next").format("yyyy-MM-dd HH:mm:ss SSS"));

        searchSourceBuilder.query(boolQueryBuilder);
        searchSourceBuilder.fetchSource(FETCH_FIELDS, Strings.EMPTY_ARRAY);
        // sort by naming sort field
        searchSourceBuilder.sort("@timestamp");
        // using from and size => simulate pagination
        searchSourceBuilder.from(1);
        searchSourceBuilder.size(10);

        sr.indices(INDEX);
        sr.source(searchSourceBuilder);

        SearchResponse searchResponse = restHighLevelClient.search(sr, RequestOptions.DEFAULT);
        if (searchResponse.getHits().getTotalHits().value > 0) {

            LOGGER.info("total count: {}", searchResponse.getHits().getTotalHits().value);

            for (SearchHit hit : searchResponse.getHits()) {
                System.out.println("Match: ");
                for (String fetchField : FETCH_FIELDS) {
                    System.out.println(" - " + fetchField + " " + hit.getSourceAsMap().get(fetchField));
                }
            }
        } else {
            System.out.println("No results matching the criteria.");
        }
        restHighLevelClient.close();
    }
```
