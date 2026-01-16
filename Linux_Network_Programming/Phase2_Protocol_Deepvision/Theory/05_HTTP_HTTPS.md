# 🧠 HTTP / HTTPS (Application Layer Protocol)
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is HTTP? (Real Meaning)

**Standard Definition:**
"HyperText Transfer Protocol. A request-response protocol for web communication."

**Senior Definition:**
*   ✅ **"Stateless, Text-Based RPC over TCP"**
*   ✅ **"The Protocol Tax on Every Business App"**

HTTP is fundamentally a **Remote Procedure Call** mechanism. `GET /api/user` is just calling a function on a remote server. The "Stateless" nature means each request is independent - the server doesn't remember you (hence cookies/sessions).

**HTTPS = HTTP + TLS.** It's not a different protocol; it's HTTP wrapped in SSL/TLS encryption.

---

## 🔹 2. Reasonable Code Snippet (HTTP Request Parsing)

Manual parsing is critical for understanding.

```cpp
#include <iostream>
#include <sstream>
#include <map>

struct HTTPRequest {
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
};

HTTPRequest parseRequest(const std::string& raw) {
    HTTPRequest req;
    std::istringstream stream(raw);
    
    // Parse Request Line: GET /index.html HTTP/1.1
    stream >> req.method >> req.path >> req.version;
    
    std::string line;
    std::getline(stream, line); // Consume newline
    
    // Parse Headers
    while (std::getline(stream, line) && line != "\r") {
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 2); // Skip ": "
            req.headers[key] = value;
        }
    }
    
    return req;
}
```

---

## 🔹 3. Deep Pros & Cons

### ✅ Pro #1: Simplicity
*   **Benefit**: Human-readable text. You can debug with `telnet` or `nc`.
*   **Example**: `GET / HTTP/1.1\r\nHost: example.com\r\n\r\n`

### ❌ Con #1: Head-of-Line Blocking (HTTP/1.1)
*   **Issue**: One slow request blocks the entire TCP connection.
*   **Impact**: Browsers open 6 parallel connections to work around this.
*   **Solution**: HTTP/2 (multiplexing) or HTTP/3 (QUIC).

### ❌ Con #2: Overhead
*   **Issue**: Headers are resent on EVERY request. A simple API call has ~400 bytes of headers for 10 bytes of data.
*   **Solution**: HTTP/2 HPACK compression.

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use HTTP when:
*   **Real-time bidirectional** communication is needed (Use WebSocket or gRPC streaming).
*   **Low latency** is critical (TCP handshake + TLS handshake = 2 RTTs minimum).

### ✅ Acceptable use cases:
*   REST APIs (CRUD operations).
*   Web serving (static files, dynamic pages).
*   Webhooks (asynchronous notifications).

---

## 🔹 5. Relevant Linux Tools

### 🛠️ curl
The Swiss Army knife for HTTP.
```bash
curl -v https://example.com
# -v shows request/response headers
# -I shows only headers (HEAD request)
# -X POST -d '{"key":"value"}' for POST requests
```

### 🛠️ openssl s_client
Debug HTTPS handshake.
```bash
openssl s_client -connect example.com:443 -showcerts
# Shows certificate chain and TLS version
```

### 🛠️ tcpdump (Protocol Analysis)
```bash
tcpdump -i any -A port 80
# -A shows ASCII payload (see HTTP headers)
```

---

## 🔹 6. Senior-Level Interview Answer

> "HTTP is a stateless, text-based protocol running over TCP (or UDP for HTTP/3). The key performance issue is **Head-of-Line Blocking** in HTTP/1.1, which HTTP/2 solves with multiplexing. HTTPS is HTTP over TLS (port 443 vs 80). The TLS handshake adds latency (full handshake = 2 RTTs), but TLS 1.3 reduces this to 1 RTT. For production C++ servers, I parse HTTP manually or use libraries like `cpp-httplib` or Boost.Beast. I always validate Content-Length to prevent buffer overflows."

---

## 🔹 7. Interview Question Bank

### Q1. What is the difference between HTTP/1.1 and HTTP/2?
**Answer**: "HTTP/1.1 is text-based, one request per connection (or pipelined, which is broken). HTTP/2 is **binary**, supports **multiplexing** (many streams over one TCP connection), **server push**, and **header compression (HPACK)**. This eliminates Head-of-Line Blocking at the application layer."

### Q2. Why does HTTPS handshake take 2 RTTs?
**Answer**: "1st RTT: TCP SYN/SYN-ACK/ACK (3-way handshake). 2nd RTT: TLS ClientHello/ServerHello + Certificate. TLS 1.3 optimizes this to 1 RTT by combining some steps. TLS 1.3 also supports 0-RTT resumption for repeat connections."

### Q3. What is the `Host` header and why is it mandatory?
**Answer**: "HTTP/1.1 requires the `Host` header to support **virtual hosting** (multiple domains on one IP). Without it, the server can't distinguish `example.com` from `another.com` if both point to the same IP. Nginx/Apache use this header to route requests to the correct virtual host."

### Q4. How do you handle partial HTTP requests?
**Answer**: "HTTP over TCP means you might receive headers in chunks. I read until I see `\r\n\r\n` (end of headers). Then I parse `Content-Length` to know how many body bytes to read. I buffer partial data in a per-connection state machine."

### Q5. What is a `301` vs `302` redirect?
**Answer**: "`301` is **Permanent** - browsers cache it. `302` is **Temporary** - browsers don't cache it. Using `302` when you mean `301` can hurt SEO (search engines won't update their index)."
