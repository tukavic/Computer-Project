# Computer-Project
An 8 bit processing machine demonstrating computer architecture fundamentals
 
![The Real Deal](/images/Computer_img.jpg)

![High Level Block Diagram](/images/Computer_r004_a.JPG)

## Overview
The [SLG46826](https://www.dialog-semiconductor.com/products/slg46826) GreenPAK device was used as the primary IC for building the computer.  It is a programmable device with both LUTs and DFFs such that you can build up functional blocks such as the Program counter, or a portion (three bits) of the ALU.  The only other IC used was the parallel memory SRAM HY6116.  Program and data memory are shared on the same memory chip, and thus, the architecture is [Von Neumann Architecture](https://en.wikipedia.org/wiki/Von_Neumann_architecture).

## ISA
The instruction set was designed specifically for this project, and has been optimized to the various tradeoffs required with the limited resources of the ICs.  Currently there are 8 Instructions + a NOP command.  Below is the truth table that illustrates the CTRL logic as it manages control flow.  All memory access instructions (LDa P, LDb P, STa P, ADD&STaP) are two cycle instructions; therefore the hardware has accommodations for such commands.

![ISA Truth Table](/images/ISA_Truth_Table.jpg) 

## List of Commands
* **LDa C**- Load Register_A with constant.  The constant is packaged in the 4 LSB of the instruction.  
* **LDa P**- Load Register_A from pointer.  The pointer location is packaged in the 4 LSB of the instruction.     
* **LDb C**- Load Register_B with constant.  The constant is packaged in the 4 LSB of the instruction.     
* **LDb P**- Load Register_B from pointer.  The pointer location is packaged in the 4 LSB of the instruction.
* **STa P**- Store the value in Register_A to memory at pointer location indicated in instruction.   
* **Add & STaP**- Add Register_A to Register_B and store result to memory at pointer location indicated in instruction. 
* **OUTA**- latch contents of Register_A to output    
* **NOP**- No Operation
* **BCH_OUT**- Branch to instruction location indicated in instruction if OUT register is zero.  Else continue.

## Testing
At the complexity of the project, it is impossible to easily debug with a simple oscilloscope or logic analyzer.  By using I2C and an ![Arduino script](Computer_debugger.ino), the Arduino is used to load program code into the memory chip, as well as read all pins and internal nodes of each GPAK IC to pull out all relevant information into text.  The arduino program also allows you to step through the program for debugging.


![debugging example](/images/gif_arduino_debug.gif)

## Example of GreenPAK
The configurable GreenPAK has a nice GUI interface.  Here are a few examples:
### ALU_ONE
![ALU_ONE](/images/ALU_one.png)

### Control Logic
![CTRL](/images/CTRL.png)

### Data Pointer
![Data Pointer](/images/DP.png)
