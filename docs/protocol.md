# **Kivi-DB Protocol Specification**

## **Overview**
Kivi-DB is a distributed key-value store. Commands are text-based and terminated with `\r\n`. Responses indicate success, error, or key data.

---

## **Commands and Responses**

### **SET Command**
- **Syntax:**  
  `set <key> <value> \r\n`

- **Responses:**  
  1. **Success:** `DONE\r\n`  
  2. **Error:**  
     - Invalid syntax: `Error: Invalid command.\r\n`  
     - Key Already Exists: `Error: Key <key> already exists.\r\n`

---

### **GET Command**
- **Syntax:**  
  `get <key> \r\n`

- **Responses:**  
  1. **Success:** `<value>\r\n`  
  2. **Error:**  
     - Invalid syntax: `Error: Invalid command.\r\n`  
     - Non-existant key: `Key not found.\r\n`

---

### **UPDATE Command**
- **Syntax:**  
  `update <key> <value> \r\n`

- **Responses:**  
  1. **Success:** `Updated successfully.\r\n`  
  2. **Error:**  
     - Invalid syntax: `Error: Invalid command.\r\n`  
     - Non-existant key: `Key not found.\r\n`

---

### **DELETE Command**
- **Syntax:**  
  `delete <key> \r\n`

- **Responses:**  
  1. **Success:** `DELETED <key>\r\n`  
  2. **Error:**  
     - Invalid syntax: `Error: Invalid command.\r\n`  
     - Non-existant key: `Error: Key not found.\r\n`

---

## **Notes**
- Keys and values are case-sensitive.
- Ensure all commands end with `\r\n`.  
