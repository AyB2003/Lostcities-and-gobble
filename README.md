# LostCities & Gobble

This repository contains two strategy games developed in **C** using **SDL** (Simple DirectMedia Layer) for graphical rendering. The games implement advanced algorithms for decision-making, offering challenging gameplay and engaging mechanics.

---

## Game 1: LostCities

### Overview
**LostCities** is a strategy card game that uses the **Monte Carlo Tree Search (MCTS)** algorithm for AI decision-making. The game challenges players to strategize their moves effectively to outscore their opponent.

### Features
- **AI Decision-Making**: Implements the **Monte Carlo Tree Search** algorithm for intelligent and adaptive gameplay.
- **Graphical Interface**: Built with **SDL** for smooth and interactive visuals.

### How MCTS Works in LostCities
The AI simulates multiple possible game states by:
1. Exploring various moves through **random playouts**.
2. Evaluating the outcomes to make the most promising decision.
3. Continuously improving decisions based on simulated results.

---

## Game 2: Gobble

### Overview
**Gobble** is a strategy board game that employs the **Minimax Algorithm** for AI decision-making, enhanced with **alpha-beta pruning** for efficiency. The objective is to outsmart the AI or another player by strategically placing and moving pieces.

### Features
- **AI Decision-Making**: Implements the **Minimax Algorithm** with **alpha-beta pruning** to evaluate optimal moves.
- **Dynamic Gameplay**: Features a growing complexity as the game progresses.

### How Minimax Works in Gobble
1. Evaluates all possible moves for both players up to a specific depth.
2. Selects the move that minimizes the opponent’s chances of winning while maximizing its own.
3. Uses **alpha-beta pruning** to ignore suboptimal branches, speeding up the decision process.
