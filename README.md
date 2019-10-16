# Computer-Project
An 8 bit processing machine demonstrating computer architecture fundamentals



![High Level Block Diagram](/images/Computer_r004.JPG)

## Overview
The SLG46826 GreenPAK device was used as the primary IC for building the computer: [SLG46826](https://www.dialog-semiconductor.com/products/slg46826).  It is a programmable device with both LUTs and DFFs such that you can build up functional blocks such as the Program counter, or a portion (three bits) of the ALU.  The only other IC used was the parallel memory SRAM HY6116.  Program and data memory are shared on the same memory chip, and thus, the architecture is ![Von Neumann Architecture]https://en.wikipedia.org/wiki/Von_Neumann_architecture.  
