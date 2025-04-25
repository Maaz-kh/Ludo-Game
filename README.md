# 🎲 Ludo OS Project

A console-based Ludo game with a visually interactive interface built in **C++**, incorporating core **Operating System concepts** such as **multithreading**, **mutex locks**, and **synchronization**. This project simulates a 2-4 player Ludo game with interactive gameplay, turn-based logic, and concurrent thread management.

---

## 🚀 Features

- 🎮 Console-based interactive UI
- 🎲 Dice roll mechanics with unlock/move decision
- 🧩 Token movement with real-time updates
- 🔐 Thread-safe access using mutex locks
- 🧠 Player turns handled via dedicated threads
- 🛡️ Validation for user input and token conditions

---

## 🧠 Core Concepts Used

- **Multithreading**:  
  Each player is represented by a separate thread, simulating real-time parallel execution.

- **Mutex Locks & Synchronization**:  
  Mutexes ensure shared resources (like the game grid or token positions) are accessed safely without race conditions.

- **Critical Section Handling**:  
  Proper lock/unlock strategy ensures only one thread updates the board at a time.

---

## 🖥️ Technologies

- Language: **C++**
- Concepts: **Threads**, **Mutex**, **I/O Streams**, **Data Structures**
- Platform: **Windows Console / Terminal**

