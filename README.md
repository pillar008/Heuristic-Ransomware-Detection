# Heuristic Ransomware Detection: An Active C++ Defense System

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Cybersecurity](https://img.shields.io/badge/Cybersecurity-Active_Defense-red?style=for-the-badge)

Traditional signature-based antivirus solutions consistently fail to detect zero-day ransomware. This project introduces a high-performance, mathematically driven Host-Based Intrusion Detection System (HIDS) written in native C++17, designed to stop active encryption in real-time. 

By deploying structured deception decoys (honeyfiles) and monitoring them through native Linux kernel event hooks, the system operates with near-zero resource overhead. Upon modification, the Heuristic Brain calculates the Shannon Entropy of the raw binary buffer. If the entropy breaches the cryptographic threshold (7.5+), the Enforcer module autonomously traces the active file descriptor through the `/proc` virtual filesystem, sends a `SIGKILL` to the malicious PID, logs the forensics, and automatically restores the compromised decoy.

## 🧠 System Architecture

```mermaid
graph TD
    A[Daemon Start] --> B[Initialize Decoy Factory]
    B --> C[Deploy Low-Entropy Honeyfiles]
    C --> D[Attach inotify Kernel Hook]
    D --> E{Wait for File Event}
    
    E -- IN_MODIFY Triggered --> F[Extract File Path]
    F --> G[Native Math Engine]
    G --> H[Calculate Shannon Entropy]
    
    H --> I{Entropy >= 7.5?}
    
    I -- No --> J[Log as Benign Edit]
    J --> E
    
    I -- Yes --> K[DANGER: Encryption Detected]
    K --> L[Enforcer Module Activated]
    L --> M[Crawl /proc Virtual Filesystem]
    M --> N[Match Open File Descriptors]
    N --> O[Identify Malicious PID]
    O --> P[Send SIGKILL to Kernel]
    P --> Q[Log Attack Forensics]
    Q --> R[Auto-Restore Decoy File]
    R --> E
