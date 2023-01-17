
# pet_sort

The program sorts PET data (timestamp and energy) by time. 

## Prerequisites
- Compiler that supports C++ 2020 standard. If your compiler does not support this standard, please change the C++ standard in the CMakeList.txt file (at least C++ 2011 required)
- Installation of [CMAKE](https://cmake.org/) - Cross platform makefile generation

## Building

To build the program, start in the program directory pet_sort
```
mkdir build
cd build
cmake ..
make -j8
```

## Running
How to run the program:
The executable is called sortPETFile, it has been created in the build directory. For help concerning the possible parameters, type:
```
./sortPETFile -h
```

Example command line:
```
./sortPETFile -i inputfile.dat -o output.txt -n 10000
```

## Explanation of parameters
  
    -i: input file with PET data and extension .dat (mandatory)
    
    -o: output file with extension .txt (optional). If no output file is specified, the output is written to standard out.

    -n: Number of frames to sort (optional). If no parameter is given, the whole input file is analyzed.

    -l: Arrival latency of data in ns (optional). If no parameter is given, 100ns is assumed as specified in the task.

    -h: Show program usage.

## Questions/Improvements
1. The code works with any latency or transmission delay (values other than 100ns have to be set with the -l parameter). Timestamp overflows do not pose a problem, so the code works with the bonus data. 

2. On my Macbook it takes about 3.3 s to read in 10^7 hits and to write them to a text file. Bottleneck for the performance is not the reading of the data or the sorting, but the writing to file. Writing to a binary file would be faster than writing to a text file. I assume for a real implementation one would choose HDF5 files or root trees as data format. To improve the performance, a serialization package like [cerial](https://uscilab.github.io/cereal/serialization_functions.html) could be used, or [fmt]( https://github.com/fmtlib/fmt) file writer. The data for one frame is read sequentially, stored in a vector and subsequently sorted. The sorted data that has timestamps that are at least 100 ns smaller than the larges time are safe to be shifted to the output vector. From the output vector the data is written to file. The size of the storage vectors depends on the time structure and the transmission delay of the data. With the 100 ns delay it is never necessary to store more data than the data of two 1024 ns frames, and the frames of the example data did not contain many hits. To improve the reading and sorting speed, instead of reading in one complete frame and then sort it, one could read in always the same fixed number of hits and pre-allocate the memory. To reduce the size and functionality load of the Sorter class, the file reading and writing could be factored out in separate classes.

3. The program uses std::ifstream to read in the data from the file. Instead of std::ifstream another stream type could be used. For batch processing I do not see any show stopper.

4. The code could be parallelized following the producer-consumer paradigm. The file reader is the producer and writes hit data into a ring buffer. The hit sorter is the consumer, takes the hits from the ring buffer and sorts them. Either single hits could be written to a large ring buffer, or blocks of e.g. 100-1000 hits to a smaller ring buffer. When the ring buffer is full the producer sleeps, and is woken up by the consumer as soon as the consumer has taken one buffer entry. When the buffer is empty the consumer sleeps, and is woken up by the producer as soon as the producer has written data to a ring buffer entry. The buffer access has to be protected with a std::mutex. To wake up producer and consumer std::counting_semaphores can be used.