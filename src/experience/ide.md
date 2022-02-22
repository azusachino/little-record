# IDE Related

## IDEA httpClient

### Sample

```http
GET http://localhost:8080/users ### HttpMethod+URL
Accept: application/json ### All necessary Headers
Content-Type: text/plain

{
    "ss": "ss" ### body content
}

```

### Use Response handler scripts

- The `client` object stores the session metadata, which can be modified inside the script. The client state is preserved until you close IntelliJ IDEA. Every variable saved in client.global as variable_name is accessible to subsequent HTTP requests as {{variable_name}}.
- `response` holds information about the received response: its content type, status, response body, and so on.

```http
### 响应的后置处理
GET localhost:8080/{{a}}

> {%
client.global.set("a","b") // client的全局变量设置之后，通过{{a}}访问

client.test("Request executed successfully", function() {
        client.assert(response.status === 200, "Response status is not 200");
    });
 %}
```

### upload file

```http
POST http://172.31.103.161:29530/api/v1/logging/collect/file
Content-Type: multipart/form-data; boundary=WebAppBoundary

--WebAppBoundary
Content-Disposition: form-data; name="file"; filename="log5.txt"

< C:\Users\ycpang\Desktop\log5.txt
--WebAppBoundary--
```

## IDE 中 GIT 相关操作执行慢

删除掉 bin 目录中的 `runnerw.exe` 和 `runnerw64.exe` 即可。

## VSCode Usage Problem

### Please update includePath?

After long time searching, figured out the problem was `configurationProvider` wrong. Remove this or use `ms-vscode.cpp-tools` instead.
