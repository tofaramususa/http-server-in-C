# http-server-in-C  
A non-blocking mini-server implemented in C  

This program acts as a server that:  
- Listens for client connections on a specified port on `127.0.0.1`.
- Facilitates communication between connected clients, allowing them to broadcast messages to each other.  

## Features and Functionality  

### **Startup Behavior**
- The program accepts a single argument: the port to bind the server to.
- If no argument is provided:
  - It outputs the error message: `Wrong number of arguments\n` to `stderr`.
  - Exits with status code `1`.
- If a system call fails before the server starts accepting connections:
  - Outputs `Fatal error\n` to `stderr`.
  - Exits with status code `1`.
- If memory allocation fails at any point:
  - Outputs `Fatal error\n` to `stderr`.
  - Exits with status code `1`.

### **Client Management**
- **Non-blocking Behavior**: The program operates in a non-blocking manner. However, if a client delays reading messages, the server must not disconnect them.  
- **Client Identification**:  
  - Each client is assigned a unique ID starting from `0`.  
  - The ID increments sequentially for each new connection.  

#### **Connection Events**
- When a client connects:  
  - Broadcasts the message: `server: client %d just arrived\n` to all connected clients.  
- When a client disconnects:  
  - Broadcasts the message: `server: client %d just left\n` to all connected clients.  

### **Message Handling**
- Clients can send messages consisting of printable characters.  
  - The server does not validate the message content.  
- Messages may contain multiple `\n`.  
- Upon receiving a message:  
  - The server prepends `client %d: ` to each line of the message.  
  - Broadcasts the formatted message to all other connected clients.

### **Constraints**
- **No #define Preprocessor**: The program must not use `#define` preprocessor directives.  
- **Bound to Localhost**: The server listens exclusively on `127.0.0.1`.  
- **File Descriptor (fd) Behavior**:  
  - File descriptors are configured to block `recv` or `send` calls unless `select` has been invoked.  

### **Best Practices**
- No unnecessary buffering; messages must be sent as quickly as possible.  
- Avoid memory or file descriptor leaks.

- **Testing Tips**:  
  - Use `nc` (netcat) to test the server.  
  - Use `fcntl(fd, F_SETFL, O_NONBLOCK)` for non-blocking I/O. However, rely on `select` and avoid manually checking for `EAGAIN`.
