# BareMetalHTTP (PocketWebd 🌐)

A lightweight HTTP web server written from scratch in C using POSIX socket APIs. Built to understand low-level networking and how web servers like Apache/Nginx work under the hood.

---

## ✨ Features

- **TCP Socket Server** — Listens on port `8080`
- **Static File Serving** — Serves files from the `public/` directory  
- **HTTP/1.1 Response** — Proper status codes and headers
- **MIME Type Detection** — `.css` → `text/css`, default → `text/html`
- **404 Handling** — Returns proper error response for missing files
- **Request Logging** — Prints raw HTTP requests to stdout for debugging

---

## 🚀 Quick Start

### Build
```bash
gcc -O2 -Wall -Wextra -o web_server main.c
```

### Run
```bash
./web_server
```

### Test
```bash
# Using browser
open http://localhost:8080/index.html

# Using curl
curl -i http://localhost:8080/index.html
```

---

## 📁 Project Structure

```
web_server/
├── main.c              # Server source code (all the magic happens here)
├── main                # Compiled binary
├── public/             # Document root (static files directory)
│   ├── index.html      # Status page with modern glassmorphism UI
│   └── styles.css      # CSS with animations and responsive design
└── README.md           # This file
```

---

## 🔧 How It Works

### The Socket Programming Flow

```
┌─────────────────────────────────────────────────────────────┐
│  1. CREATE    socket()     Create communication endpoint    │
│  2. BIND      bind()       Associate with IP:PORT           │
│  3. LISTEN    listen()     Mark as passive, set backlog     │
│  4. ACCEPT    accept()     Wait for incoming connection     │
│  5. READ      read()       Receive HTTP request             │
│  6. WRITE     write()      Send HTTP response               │
│  7. CLOSE     close()      Terminate connection             │
│       ↻ Loop back to step 4 for next client                 │
└─────────────────────────────────────────────────────────────┘
```

### Key Concepts

| Concept | Value | Explanation |
|---------|-------|-------------|
| `AF_INET` | IPv4 | Address family for IPv4 internet protocols |
| `SOCK_STREAM` | TCP | Reliable, ordered, connection-oriented stream |
| `INADDR_ANY` | 0.0.0.0 | Listen on all available network interfaces |
| `htons()` | Host→Network | Converts port to network byte order (big-endian) |
| Backlog | 3 | Max pending connections in OS queue |

### HTTP Request/Response

**Parsing incoming request:**
```c
// "GET /index.html HTTP/1.1" → extracts method, path, version
sscanf(buffer, "%s %s %s", method, path, version);
```

**Sending response:**
```
HTTP/1.1 200 OK
Content-Type: text/html

<file contents here>
```

---

## 🧠 Technical Deep-Dive (Interview Ready)

### Why use `htons()`?
Network protocols use **big-endian** byte order, but your CPU might use little-endian. `htons()` (Host TO Network Short) ensures the 16-bit port number is in the correct byte order for network transmission.

### What's `struct sockaddr_in`?
```c
struct sockaddr_in {
    sa_family_t    sin_family;  // AF_INET (IPv4)
    in_port_t      sin_port;    // Port in network byte order
    struct in_addr sin_addr;    // IP address (INADDR_ANY = 0.0.0.0)
};
```

### Why cast to `(struct sockaddr *)`?
`bind()` is generic and accepts different address types. The cast provides type compatibility — it's C's way of achieving polymorphism.

### Why is this a "blocking" server?
`accept()` and `read()` block execution until data arrives. This means **one client at a time** — the server can't handle concurrent requests. Production servers use:
- Multi-threading (`pthread`)
- Process forking (`fork()`)
- Async I/O (`select()`, `poll()`, `epoll`)

### Security Consideration
⚠️ **Path Traversal Vulnerability**: Requests like `GET /../../../etc/passwd` could escape the document root. Production servers sanitize paths to prevent directory traversal attacks.

---

## 🛠️ System Calls Used

| Function | Header | Purpose |
|----------|--------|---------|
| `socket()` | `<sys/socket.h>` | Create socket file descriptor |
| `bind()` | `<sys/socket.h>` | Bind socket to address |
| `listen()` | `<sys/socket.h>` | Mark socket as passive |
| `accept()` | `<sys/socket.h>` | Accept incoming connection |
| `read()` | `<unistd.h>` | Read data from socket |
| `write()` | `<unistd.h>` | Write data to socket |
| `close()` | `<unistd.h>` | Close file descriptor |
| `htons()` | `<netinet/in.h>` | Host to network byte order |

---

## 📈 Future Improvements

- [ ] Add more MIME types (`.js`, `.png`, `.svg`, `.json`)
- [ ] Default `/` → `/index.html` routing
- [ ] Path sanitization (prevent `../` traversal)
- [ ] Multi-threaded request handling
- [ ] HTTP Keep-Alive support
- [ ] Command-line arguments for port/directory
- [ ] Proper HTTP header parsing

---

## 📖 Learning Resources

- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
- [Linux Socket Programming](https://man7.org/linux/man-pages/man2/socket.2.html)
- [HTTP/1.1 Specification (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616)

---

## 📝 License

MIT License — Feel free to use, modify, and learn from this project.

---

*Built with ❤️ to understand the fundamentals of web servers*
