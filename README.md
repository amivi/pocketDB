# pocketDB
- A lightweight key-value data-store backed by Log Structured Merge (LSM) Tree.
- This implementation of LSM tree aims at providing low cost indexing for a file system with high rates of insertion and retrieval.

# Project Structure
pocketDB is divided into three main modules :
1. ### Memory Component
     - #### get API
       - ##### getKey
       - ##### getInRange
       - ##### getAll
     - #### put API
2. ### Disk Component
     - #### get API
       - ##### getKey
       - ##### getInRange
       - ##### getAll
     - #### flush API
3. ### Engine Manager
 
# How to Contribute  
- This project is currently being developed in C programming language (only).
- No external libraries should be used other than the standard C libraries.
- All contributions all welcome.
