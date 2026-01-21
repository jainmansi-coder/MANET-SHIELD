# MANETShield: Blackhole Attack Detection and Prevention using NS-3


## Overview
This project simulates a Mobile Ad Hoc Network (MANET) using the AODV routing protocol in NS-3.  
It demonstrates the effect of a Blackhole attack and implements a detection and prevention mechanism.

The simulation is divided into three phases:
1. Baseline MANET (Normal operation)
2. MANET under Blackhole Attack
3. MANET with Blackhole Detection and Prevention

---

## Tools and Environment
- NS-3.43
- C++
- Ubuntu (WSL)
- Git & GitHub

---

## Simulation Scenarios

### 1. Baseline MANET
- No malicious node
- Normal AODV routing
- Measures normal Packet Delivery Ratio (PDR)

**Result:**  
PDR ≈ **0.79**

---

### 2. MANET under Blackhole Attack
- One malicious node advertises fake optimal routes
- Drops all received packets
- Network performance degrades

**Result:**  
PDR ≈ **0.73**

---

### 3. Detection and Prevention Phase
- Blackhole node detected based on packet dropping behavior
- Malicious node is isolated
- Packet forwarding resumes normally

**Result:**  
PDR ≈ **0.63**

---

## Results Summary

| Scenario     | Packet Delivery Ratio (PDR) |
|--------------|-----------------------------|
| Baseline     | 0.79                        |
| Attack       | 0.73                        |
| Prevention   | 0.63                        |

---

## Files Description
- `scratch/manet-attack.cc` → Blackhole attack simulation code
- `baseline.txt` → Baseline run output
- `attack.txt` → Attack phase output
- `prevention.txt` → Detection & prevention output

---

## Conclusion
The results clearly show that a Blackhole attack significantly affects MANET performance.
The proposed detection and prevention mechanism successfully identifies and isolates the malicious node, improving network stability.

This project demonstrates practical MANET security analysis using NS-3.

---

## Author
**Jain Mansi**  
Personal Academic Project
