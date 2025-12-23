# 🌟 A* Metric Pathfinder (C++ & Python Stack)

> A high-performance autonomous navigation stack that reads a binary map, calculates the optimal path using the A* algorithm, and visualizes the result—all while accounting for robot physical dimensions.

![C++](https://img.shields.io/badge/Language-C++17-blue?style=for-the-badge&logo=c%2B%2B)
![Python](https://img.shields.io/badge/Perception-Python-yellow?style=for-the-badge&logo=python)
![Status](https://img.shields.io/badge/Status-Operational-green?style=for-the-badge)

---

## 🗺️ How It Works (The Pipeline)

This project simulates a full robotics stack, separated into **Perception**, **Planning**, and **Visualization**.

```mermaid
graph LR
    A[Raw Image Map] -->|Python| B(Binary Grid .txt)
    B -->|Load Data| C{C++ A* Solver}
    C -->|Calculate Path| D[Path Coordinates]
    D -->|Export| E(Visual Grid .txt)
    style C fill:#f9f,stroke:#333,stroke-width:4px
Perception (Python): Reads a pixel map (warehouse.png) and converts it into a 0/1 binary grid.Planning (C++): The "Brain." Loads the grid, runs the A* algorithm, and handles collision physics.Visualization: Exports a human-readable ASCII map showing the safe path.🧠 The Logic: A* Algorithm ExplainedA* (A-Star) is the industry standard for pathfinding because it is smart. Unlike a "blind" search, it uses math to predict the best future step.The "Holy Trinity" FormulaEvery step the robot takes is scored using this formula:$$F = G + H$$VariableNameMeaningAnalogyGGiven CostHow far have I walked from the Start?The Odometer (History)HHeuristicHow far is the Goal (straight line)?The Map Estimate (Future)FFinal ScoreTotal estimated trip cost.The Decision MakerDecision FlowchartThe code follows this decision loop thousands of times per second:Code snippetflowchart TD
    Start((Start)) --> AddOpen[Add to To-Do List]
    AddOpen --> CheckEmpty{Is List Empty?}
    CheckEmpty -- Yes --> Fail[No Path Found]
    CheckEmpty -- No --> PickBest[Pick Node with Lowest F-Cost]
    PickBest --> IsGoal{Is it Goal?}
    IsGoal -- Yes --> Success((Goal Reached!))
    IsGoal -- No --> Expand[Check Neighbors]
    Expand --> Calc[Calculate G, H, F]
    Calc --> Update[Update Parent & Cost]
    Update --> CheckEmpty
🤖 The "Fat Robot" PhysicsMost basic tutorials treat the robot as a single pixel. This engine is different.We implemented a Convolutional Collision Check. Before moving to a pixel, the robot scans a 3x3 area around the center to ensure its "body" fits through the gap.Visualizing the Check:If the robot tries to step on C (Center), it scans all X spots.Plaintext  X  X  X   <-- Top Shoulder
  X  C  X   <-- Belly Button (Center)
  X  X  X   <-- Bottom Feet
If ANY X hits a wall (#), the move is illegal. This forces the robot to keep a safe distance from walls.🚀 How to RunPrerequisitesC++ Compiler: g++Python: python3The "One-Click" LaunchWe have automated the entire build and run process. Just run the master script:Bash# Make the script executable (only needed once)
chmod +x run_stack.sh

# Launch the Stack
./run_stack.sh
Manual Run (Under the Hood)If you want to run components individually:Bash# 1. Perception Layer
python3 process_map.py

# 2. Compile Planner
g++ -Wall -Wextra main.cpp -o main

# 3. Run Planner
./main
📂 Project Structuremain.cpp - The C++ Source Code (The "Whale").process_map.py - Python script to convert images to text grids.run_stack.sh - Bash script to automate the pipeline.binary_grid.txt - Intermediate data layer (The Map).visual_grid.txt - Final Output (The Result).
