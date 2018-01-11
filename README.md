____
# Beemeal Libc Rand Cracker
## Version : V1.0.0
## Date    : 2018/01/01
## Author  : cv.dev.cpp@gmail.com
## Compiler: Visual studio 2017
##### Protection: Beemeal Source Shadow (V1.0.0)
____

## __Project description__
This tool permits to find the random seed given by srand that permits to
generate a pseudo-random sequence given by the user
The tool has 2 modes:
- Mode 1: i want to find the seed that generated the <RandVal_0>...<RandVal_N> sequence:
    Usage: ./FindRandSeed.exe --break <NbThread> <RandVal_0>...<RandVal_N>
	
- Mode 2: i want to generate a random sequence to test the '--break' mode later:
	Usage: ./FindRandSeed.exe --generate-rand-sequence <RandSequenceSize>
	Notice: <RandSequenceSize> minimum is 10