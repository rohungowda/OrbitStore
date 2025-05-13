# OrbitStore  

**OrbitStore** is an ongoing project to develop a NoSQL distributed key-value store using **C++**. The system is designed to handle large-scale data efficiently and reliably with the following key features:  

## Features  
- **Consistent Hashing Partitioning**  
  Ensures data distribution across nodes for scalability and fault tolerance.  
- **LSM Tree Data Structures**  
  Provides fast and efficient search, insert, and delete operations.  
- **Disk Storage, Memory Buffer Management, and Arena Managment**  
  Combines persistent storage with in-memory caching for optimized performance.  
- **Asynchronous TCP Connections**  
  Uses the Boost.Asio library for efficient non-blocking communication.  
- **Rumor-Mongering Gossip Protocol**  
  Facilitates node registration, status updates, and leaving mechanisms.  
- **Load Balancing with UDP**  
  Monitors node health and routes requests to the appropriate nodes, with rerouting support if needed.  

## Project Status  
This project is currently under active development, and its design and implementation are evolving.  

## Inspiration
This project takes inspiration from the DynamoDB paper and how Cassandra is built. This projects' goal is to take a deep dive into these sytems and learn how to build them.
