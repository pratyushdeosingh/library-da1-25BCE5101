# DA1 - Library Book Tracking System (C)

## Assignment Details

- **Course:** Structured and Object Oriented Programming  
- **Faculty:** Dr. Dinakaran M  
- **Student:** Pratyush Deo Singh  
- **Register No:** 25BCE5101  
- **Assignment:** DA1 (C Mini Project)  
- **Assigned Question:** Q4 - Library Book Tracking System

## Project Overview

This is a menu-driven C program to manage library books with permanent file storage.  
It supports adding books, issuing/returning books, searching by ID/keyword, and listing all or issued books.

The project is compiled to WebAssembly for browser hosting using GitHub Pages.

## Repository and Live Demo

- **GitHub Repository:**  
  `https://github.com/pratyushdeosingh/library-da1-25BCE5101`
- **Live Demo (GitHub Pages):**  
  `https://pratyushdeosingh.github.io/library-da1-25BCE5101/`

## Files in This Repository

- `library.c` - main C source code
- `da1_library.html` - WebAssembly frontend page
- `da1_library.js` - Emscripten-generated JS glue
- `da1_library.wasm` - compiled WebAssembly binary
- `index.html` - redirect entry point for GitHub Pages
- `documentation.txt` - detailed assignment documentation
- `library.dat` - runtime binary data file (created/updated during execution)

## Features Implemented

- Add new book records (`BookID`, `Title`, `Author`, status defaults to `Available`)
- Issue books to students by register number
- Return issued books
- Search by exact `BookID`
- Search by title keyword (case-insensitive)
- Display all books in formatted table
- Display only currently issued books
- Save/load records using binary file handling (`library.dat`)
- Validation for duplicate IDs, invalid inputs, and invalid operations

## C Concepts Used

- `struct` (`Book`)
- User-defined modular functions (17 total)
- `switch-case` menu flow
- Array-based record storage (`books[MAX_BOOKS]`)
- Binary file handling (`fopen`, `fread`, `fwrite`, `fclose`)
- Input sanitation with `clearInputBuffer()`
- String operations (`strcpy`, `strncpy`, `strlen`, `strstr`, `tolower`)

## How to Run Locally (C Compiler)

```bash
gcc library.c -o library
./library
```

On Windows (MinGW):

```bash
gcc library.c -o library.exe
library.exe
```

## WebAssembly Build (Reference)

```bash
emcc library.c -o da1_library.html -s WASM=1 -s EXIT_RUNTIME=1 -s INVOKE_RUN=1 -s FORCE_FILESYSTEM=1
```

## GitHub Pages Note

`index.html` redirects automatically to `da1_library.html` so the project loads correctly from the root URL.

## Status

This project is complete, deployed, and ready for DA submission.
