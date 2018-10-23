# Design of Parallel and High-Performances Computing 
**@ ETH Zürich - FALL 2018**

## Description

This project aims to experiment optimizations using parallel programming for the Minimum Spawning Tree problem.


## Authors

Thomas Cambier, 
Raphaël Dang-Nhu, 
Thibault Dardinier, 
Clément Trassoudaine

## Compiling
cd code   
cmake -DBUILD_TDEBUG=ON/OFF  
make  
./bin/exec  

## Writing Debug Code
#ifdef DEBUG  
cout << x << endl;  
#endif  
