# cAPI - A Lightweight C REST API Framework

A multithreaded HTTP REST API framework written in pure C, designed for building fast, efficient web services with minimal dependencies.

## ✨ Features

- **Multithreaded Request Handling** — Concurrent client connections using pthread
- **Route-Based Routing** — Easy path matching with support for multiple HTTP methods
- **HTTP Method Support** — GET, POST, DELETE, PATCH
- **Handler Callbacks** — Flexible handler architecture for business logic
- **JSON Responses** — Built-in cJSON integration for JSON serialization
- **SQLite Integration** — Database support out of the box
- **Request/Response Abstraction** — Clean struct-based request/response handling
- **HTTP Status Codes** — Proper HTTP responses with status codes
- **Concurrent Requests** — Thread-safe request handling

## 📋 Requirements

- GCC or Clang compiler
- POSIX-compliant OS (Linux, macOS, BSD)
- SQLite3 development libraries
- pthreads (usually included)

## 🚀 Installation

### Build from source:

```bash
git clone https://github.com/yourusername/cAPI.git
cd cAPI
make clean
make
```

### Run the server:

```bash
make run
# or
./server
```

The server will start on `http://localhost:8080`

## 📁 Project Structure

```
cAPI/
├── server.c              # Main server and request handler
├── common.h/c            # Common types and utilities
├── config.h/c            # Route configuration
├── handlers.h/c          # HTTP handlers
├── controllers.h/c       # Business logic controllers
├── db.h/c                # Database operations
├── cJSON.h/c             # JSON library
├── Makefile              # Build configuration
└── README.md             # This file
```

## 💻 Usage Example

### Define a Handler

```c
http_response get_jokes_handler(http_request* request) {
    char* json_data = get_jokes_from_db(NULL);
    
    http_response resp = {
        .status_code = 200,
        .body = json_data,
        .content_type = "application/json",
        .should_free = 1
    };
    
    return resp;
}
```

### Configure Routes

In `config.c`:

```c
struct handler jokes_handlers[] = {
    {GET, get_jokes_handler},
    {POST, post_jokes_handler}
};

struct route routes[NUM_ROUTES] = {
    {
        .path = "/api/jokes",
        .handlers = jokes_handlers,
        .handler_count = 2
    }
};
```

### Test the API

```bash
curl http://localhost:8080/api/jokes
curl -X POST http://localhost:8080/api/jokes
curl -X DELETE http://localhost:8080/api/jokes/1
```

## 🔧 API Structure

### Request Object

```c
typedef struct {
    enum HTTP_Method method;  /* GET, POST, DELETE, PATCH */
    char* path;               /* Request path */
    char* body;               /* Request body (for POST/PATCH) */
} http_request;
```

### Response Object

```c
typedef struct {
    int status_code;          /* HTTP status code (200, 404, 500, etc) */
    char* body;               /* Response body (usually JSON) */
    char* content_type;       /* Content-Type header */
    int should_free;          /* Whether to free body after sending */
} http_response;
```

### Handler Function Type

```c
typedef http_response (*handler_t)(http_request* request);
```

## 🛠️ Development

### Add a New Route

1. **Create a handler** in `controllers.c`:
```c
http_response get_users_handler(http_request* request) {
    char* body = malloc(256);
    snprintf(body, 256, "{\"users\": []}");
    
    return (http_response){
        .status_code = 200,
        .body = body,
        .content_type = "application/json",
        .should_free = 1
    };
}
```

2. **Add to config.c**:
```c
struct handler users_handlers[] = {
    {GET, get_users_handler}
};

/* Add to routes array */
struct route routes[] = {
    { .path = "/api/jokes", .handlers = jokes_handlers, .handler_count = 2 },
    { .path = "/api/users", .handlers = users_handlers, .handler_count = 1 }
};
```

3. **Update NUM_ROUTES** in `config.c`:
```c
#define NUM_ROUTES 2
```

4. **Recompile**:
```bash
make clean
make run
```

## 📊 Logging

The server logs all requests with:
- Client IP address
- HTTP method
- Request path
- Response status code
- Timestamp

Example log:
```
[Tue Sep  8 22:28:47 2026] 127.0.0.1 - "GET /api/jokes" - 200
```

## 🔐 Security Considerations

This framework is designed for learning and small projects. For production use:
- Add input validation
- Implement authentication (JWT, API keys)
- Add rate limiting
- Use HTTPS/TLS
- Add CORS support
- Sanitize database inputs
- Add comprehensive error handling

## 🐛 Troubleshooting

### "Bad file descriptor" error
- Make sure socket is not being closed in main thread before worker thread reads
- Check that `handle_request()` is not trying to read from closed socket

### Routes showing as "0x0" or corrupted
- Verify `struct route` definition is consistent across all headers
- Check that routes are declared as `extern` in headers
- Ensure routes are initialized globally in `config.c`

### Compiler errors with multiple includes
- Add include guards (`#ifndef`, `#define`, `#endif`) to all headers
- Or use `#pragma once` at the top of each header file

## 📝 License

cAPI is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

### Third-party Dependencies

**cJSON** - Provided under the MIT License:

```
Copyright (c) 2009-2017 Dave Gamble and cJSON contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

## 👨‍💻 Author

**Mohamed EL AFIA** - Initial framework design and implementation

## 🤝 Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📚 Resources

- [HTTP Methods](https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods)
- [JSON API Specification](https://jsonapi.org/)
- [SQLite Documentation](https://www.sqlite.org/docs.html)
- [POSIX Threads](https://pubs.opengroup.org/onlinepubs/9699919799/)

## 🎯 Roadmap

- [ ] URL parameters (`/api/jokes/:id`)
- [ ] Query string parsing
- [ ] Request body parsing (multipart/form-data)
- [ ] Middleware support
- [ ] Static file serving
- [ ] WebSocket support
- [ ] TLS/HTTPS support
- [ ] Built-in authentication

---

**Happy coding! 🚀**