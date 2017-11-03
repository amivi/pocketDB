# pocketDB
A lightweight key-value data-store backed by Log Structured Merge (LSM) Tree.
This implementation of LSM tree aims at providing low cost indexing for a file system with high rates of insertion and retrieval.
This project is specifically useful for the applications with high rates of insertions and retrievals.

# Project Structure
This project is divided into three main modules :
a) Memory Component
    treemap.c/.h
b) Disk Component
c) Engine Manager

Three is also a UI component right now to input queries and output result(s) :
    input.c/.h
    output.c/.h
    
# *This project is currently under development.
