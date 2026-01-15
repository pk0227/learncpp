# 🧠 Proxy Design Pattern
> **Deep, Practical, Senior-Level Understanding**  
> *(For 7+ years C/C++ Interview & Real-World Usage)*

## 🔹 1. What is Proxy? (Real Meaning)

**Standard Definition:**
"Provide a surrogate or placeholder for another object to control access to it."

**Senior Definition:**
*   ✅ **"The Gatekeeper / Interceptor"**
*   ✅ **"Indirection with Benefits"**

A Proxy looks like the object, smells like the object, but **is not** the object. It intercepts the call and decides *if, when, and how* to forward it.

---

## 🔹 2. Proxy Types & Implementation

### A. Virtual Proxy (Lazy Loading)
Don't load a 500MB video until the user hits play.

```cpp
void play() {
    if (!realObject) {
        realObject = std::make_unique<Video>("file.mp4"); // Lazy Init
    }
    realObject->play();
}
```

### B. Protection Proxy (Access Control)
Only Admin can delete.

```cpp
void deleteItem(int id) {
    if (user.isAdmin()) realService->deleteItem(id);
    else throw AccessDenied();
}
```

### C. Smart Reference Proxy (C++ Smart Pointers)
`std::shared_ptr` is a proxy.

---

## 🔹 3. Pros — Deep Explanation

### ✅ Pro #1: Performance (Virtual Proxy)
*   **The Benefit**: Draconian reduction in startup time and memory footprint. You don't pay for what you don't use.

### ✅ Pro #2: Security
*   **The Benefit**: The RealService can focus purely on business logic. The Proxy handles the "dirty work" of AuthN/AuthZ.

### ✅ Pro #3: Lifecycle Management
*   **The Benefit**: `shared_ptr` manages the death of an object. The client doesn't need to know *when* to delete it.

---

## 🔹 4. Cons — Deep Explanation

### ❌ Con #1: Latency
*   **The Issue**: Every request goes through an extra hop. For Remote Proxies (RPC), this adds network limits.
*   **Impact**: A function call that looks local might take 500ms (Network call). This violates the "Principle of Least Astonishment".

### ❌ Con #2: Complexity
*   **The Issue**: Writing wrappers manually is boring and error-prone.
*   **Impact**: Often solved via generic templates (`std::shared_ptr`), but custom logic requires manual coding.

---

## 🔹 5. When to Use / Not Use

### ❌ Do NOT use Proxy when:
*   The only goal is to hide a constructor (Use Factory).
*   The "Real Object" is lightweight and cheap to create (Lazy loading an integer is silly).

### ✅ Acceptable use cases:
*   **Heavy Assets**: High-res images, DB connections.
*   **Remote Objects**: Implementing RPC / CORBA / gRPC clients.
*   **Access Control**: Adding permission layers.

---

## 🔹 6. Senior-Level Interview Answer

> "I often treat **Smart Pointers** (`unique_ptr`, `shared_ptr`) as the most common Proxies in C++. Beyond that, I use **Virtual Proxies** for lazy loading in UI apps to keep startup fast. The main pitfall is that a Proxy can hide the latency of a call—making a network request look like a local function call can lead to performance bugs if developers aren't aware."

---

## 🔹 7. Interview Question Bank

### Q1. Proxy vs Decorator?
**Answer**: "**Decorator** adds *behavior* (functional features). **Proxy** controls *access* (lifecycle, security, location). The Proxy generally doesn't change the *result*."

### Q2. How is std::shared_ptr a Proxy?
**Answer**: "It overloads `operator->` and `operator*` to masquerade as a raw pointer, but intercepts these calls (or rather, the destruction) to manage reference counting."

### Q3. What is a "Ghost" object?
**Answer**: "A specific type of Proxy used in ORMs. It's an object that contains only the database ID. When any other field is accessed, the Proxy 'materializes' the full data from the DB."
