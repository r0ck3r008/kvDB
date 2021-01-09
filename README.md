## A simplistic in-menory key-value store written in C.

### Dependencies
* GNU Make
* GNU GCC

### Compilation
```bash
# Compile
$> make
# Cleanup
$> make clean
# Hard cleanup (Including binary files)
$> make distclean
```

### Running the benchmark
The compilation creates two binary files, _viz._, _libkbdb.so_ and _benchmark.so_ in _bin/_ directory.
The _libkvdb.so_ can be linked to any project by importing header file _kvdb.h_ in the root of the project.
A benchmark is provided as _bin/benchmark.so_ which serves as an example of usage as well as testing the run time.
A dictionary file, _example/words.txt_ can be used to provide sample keys to test on with NULL values.

### Test setup
* OS: Arch Linux (kernel 5.4.87-1-lts)
* GCC version: 10.2.0
* GNU Make version: 4.3

### Future Trajectory
The project can be extended as the following,
* Use the prefix tree to provide suggestions in case of a failed lookup (#661743d).