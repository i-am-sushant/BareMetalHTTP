#  BareMetlHTTP

A lightweight, no-frills HTTP server in C with just enough funk to be fun—and just enough structure to extend.

## What it does
- Listens on **port 8080**
- Serves files from the **`public/`** directory
- Returns **`404 Not Found`** when a file doesn’t exist
- Basic Content-Type handling:
  - `.css` → `text/css`
  - everything else → `text/html`
- Logs the raw client request to stdout (handy for debugging)

## Quick start (Linux)
### Build
```bash
gcc -O2 -Wall -Wextra -o web_server main.c
```

### Run
```bash
./web_server
```

### Add something to serve
```bash
mkdir -p public
printf '<!doctype html><h1>It works.</h1>\n' > public/index.html
```

### Open it
- Browser: `http://localhost:8080/index.html`
- Or with curl:
```bash
curl -i http://localhost:8080/index.html
```

## Project layout
```text
.
├── main.c
└── public/
    └── index.html
```

## Notes (the “tiny server” fine print)
- Single-process, sequential request handling
- Minimal HTTP parsing and header handling
- No HTTPS, no compression, no fancy MIME table (yet)

## Make it yours
Want to level it up? Typical next steps:
- Add more MIME types (js/png/svg/etc.)
- Default `/` → `/index.html`
- Safer path handling (avoid `../` traversal)
- Keep-alive / better header parsing

---
