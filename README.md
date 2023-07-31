# Depth-First Search Maze Solver
![Screenshot 2023-07-31 210641](https://github.com/milckywayy/DFSMazeSolver/assets/120181288/4fa1ead6-423d-4761-9f58-54b45db0541c)

## Description
This project is a maze-solving application that generates a random maze of user-specified size and finds the shortest path using Depth-First Search (DFS) algorithm. The application generates a maze with random weights assigned to nodes and then explores all possible paths to find the shortest one.

## Usage
The program is invoked with the following command-line arguments:  
  
    ./main.exe <height> <width> <start x> <end x>
where:
1. Maze height (integer from the range <2, ∞>).
2. Maze width (integer from the range <2, ∞>).
3. Starting x position of the maze (integer from the range <1, Maze width>).
4. Ending x position of the maze (integer from the range <1, Maze width>).

Use -h option to see short usage guide:  
    
    ./main.exe -h

## Features
- Maze Generation: The program generates a random maze of the size specified by the user, creating a challenging labyrinth for pathfinding.
- Random Weights: Each node in the maze is assigned random weights, adding complexity to pathfinding.
- Depth-First Search: The application uses the Depth-First Search algorithm to explore all possible paths through the maze.
- Shortest Path Determination: After exploring all paths, the program identifies the shortest path based on the accumulated weights of nodes.

## Technologies
- C programming Language
- Pathfinding: Depth-First Search (DFS) algorithm
