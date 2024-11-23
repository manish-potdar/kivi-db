# **Kivi-DB**  

**Kivi-DB** is a lightweight distributed key-value store developed as part of **CS744: Design and Engineering of Computing Systems**. It supports basic CRUD operations via a simple RESP-like text-based protocol.

---

## **Features**  
- **Distributed Design:** Supports concurrent client connections, with data residing on multiple nodes.
- **CRUD Operations:**  
  - Set, get, update, and delete key-value pairs.  
- **Lightweight Protocol:** Easy-to-use text-based command system.  

---

## **Supported Commands**  
1. **SET**: Add a new key-value pair.  
   - Example: `set key value \r\n`  
2. **GET**: Retrieve the value for a given key.  
   - Example: `get key \r\n`  
3. **UPDATE**: Modify the value of an existing key.  
   - Example: `update key value \r\n`  
4. **DELETE**: Remove a key-value pair.  
   - Example: `delete key \r\n`  

Refer to [protocol.md](./docs/protocol.md) for detailed responses and error handling.

---

## **How to Run**  
1. **Clone the repository:**  
   ```
   git clone https://github.com/mAasim1026/kivi-db.git
   cd kivi-db
   ```
2. **Build the project:**  
   ```
   make
   ```
3. **Start the server:**  
   ```
   ./bin/server
   ```
4. **Connect a client** using any TCP client like `telnet` or write your own. 
    ```
    telnet 127.0.0.1 8010
    ```

---

## **Project Details**  
**Course:** CS744 - Design and Engineering of Computing Systems  
**Instructor:** Prof. Purushottam Kulkarni  
**Authors:** Mohammad Aasim, Aditya Upadhye

---
