#!/bin/bash

python3 process_map.py
python3 verify_grid.py
g++ -Wall -Wextra main.cpp -o main
./main
