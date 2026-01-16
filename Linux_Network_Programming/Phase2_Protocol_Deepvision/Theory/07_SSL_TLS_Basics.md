# 🧠 SSL/TLS Basics (Transport Layer Security)
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is TLS? (Real Meaning)

**Standard Definition:**
"Secure Sockets Layer / Transport Layer Security. Encrypts data between client and server."

**Senior Definition:**
*   ✅ **"Authenticated Encryption Channel over TCP"**
*   ✅ **"Public Key Infrastructure (PKI) in Action"**

TLS provides **three guarantees**:
1. **Confidentiality** (Encryption) - Data can't be read by middlemen.
2. **Integrity** (Hashing) - Data can't be modified without detection.
3. **Authentication** (Certificates) - Server proves its identity.

**SSL is deprecated.** Modern systems use **TLS 1.2** or **TLS 1.3** (2018).

---

## 🔹 2. Reasonable Code Snippet (OpenSSL Basics)

Using OpenSSL library for TLS client.

```cpp
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

void tlsConnect(const char* hostname) {
    // 1. Initialize OpenSSL
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
    
    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);
    
    // 2. Create TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(443);
    inet_pton(AF_INET, hostname, &addr.sin_addr);
    
    connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    
    // 3. Wrap socket in TLS
    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sock);
    
    // 4. TLS Handshake (Blocking)
    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
    } else {
        std::cout << "TLS Handshake successful!\n";
        std::cout << "Cipher: " << SSL_get_cipher(ssl) << "\n";
        
        // 5. Send HTTP request over TLS
        const char* req = "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n";
        SSL_write(ssl, req, strlen(req));
    }
    
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
    SSL_CTX_free(ctx);
}
```

---

## 🔹 3. Deep Pros & Cons (TLS Handshake)

### ✅ Pro #1: Strong Security
*   **Benefit**: Even if someone captures all packets, they can't decrypt them (assuming modern ciphers like AES-GCM).
*   **Example**: Banks, Login pages, APIs.

### ❌ Con #1: Latency Overhead
*   **Issue**: Full TLS 1.2 handshake = **2 Round Trip Times (RTT)**.
    *   RTT 1: TCP handshake
    *   RTT 2: TLS ClientHello/ServerHello
    *   RTT 3: Certificate verification
*   **Impact**: 150ms+ added latency on high-latency networks.
*   **Solution**: TLS 1.3 reduces this to **1 RTT**.

### ❌ Con #2: CPU Cost
*   **Issue**: RSA/ECDSA signature verification is CPU-intensive.
*   **Impact**: TLS offloading to hardware (Nginx with SSL accelerators).

---

## 🔹 4. When to Use / Not Use

### ❌ Do NOT use TLS when:
*   **Extreme low latency** is required (e.g., HFT trading within same datacenter). Use IPsec or direct TCP.
*   **Multicast/Broadcast** is needed (TLS requires a 1-to-1 TCP connection).

### ✅ Acceptable use cases:
*   **HTTPS** (web traffic).
*   **FTPS, SMTPS, IMAPS** (secure file transfer, email).
*   **gRPC over TLS** (microservices).

---

## 🔹 5. Relevant Linux Tools

### 🛠️ openssl s_client
Debug TLS connections.
```bash
openssl s_client -connect google.com:443 -showcerts
# Shows:
# - Certificate chain
# - Cipher suite negotiated
# - TLS version
```

### 🛠️ openssl x509
Inspect certificates.
```bash
openssl x509 -in cert.pem -text -noout
# Shows:
# - Issuer (CA)
# - Validity (Not Before/Not After)
# - Subject (Domain name)
```

### 🛠️ wireshark / tshark
Decrypt TLS if you have the private key.
```bash
tshark -i any -Y "ssl.handshake.type == 1"
# Filter to see only ClientHello packets
```

---

## 🔹 6. Senior-Level Interview Answer

> "TLS provides encryption, integrity, and authentication over TCP. The handshake works via **asymmetric cryptography** (RSA/ECDSA for key exchange) and **symmetric cryptography** (AES for bulk data). A critical production issue is **certificate expiration**—automated renewal with Let's Encrypt is standard. TLS 1.3 improves on 1.2 by reducing handshake latency to 1-RTT and removing weak ciphers (RC4, SHA-1). In C++, I use OpenSSL or BoringSSL, always checking `SSL_get_verify_result()` to prevent man-in-the-middle attacks."

---

## 🔹 7. Interview Question Bank

### Q1. What is a Certificate Authority (CA)?
**Answer**: "A trusted third party that signs certificates. Your browser has a list of trusted CAs (root certificates). When a server presents a certificate signed by a trusted CA, the browser trusts it. Self-signed certificates are not trusted by default."

### Q2. What is the difference between RSA and ECDSA?
**Answer**: "Both are asymmetric algorithms for key exchange. **RSA** uses large prime numbers (2048-bit keys). **ECDSA** (Elliptic Curve) achieves the same security with smaller keys (256-bit), making it faster. Modern servers prefer ECDSA."

### Q3. What is Perfect Forward Secrecy (PFS)?
**Answer**: "It ensures that even if the server's private key is compromised later, past communications remain secure. Achieved using ephemeral key exchange (DHE or ECDHE). Without PFS, stealing the server key allows decrypting all past traffic."

### Q4. What is SNI (Server Name Indication)?
**Answer**: "An extension to TLS that allows the client to specify the hostname during the handshake. This enables virtual hosting on HTTPS (multiple SSL sites on one IP). Without SNI, the server can't know which certificate to present."

### Q5. Why is TLS 1.0/1.1 deprecated?
**Answer**: "They support weak ciphers (RC4, DES) vulnerable to attacks (BEAST, POODLE). TLS 1.2+ is mandatory (PCI-DSS compliance). TLS 1.3 removes all legacy cruft and is now the recommended version."
