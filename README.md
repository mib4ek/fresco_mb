# fresco_mb

Commands for use:
1. make - make code files
2. make test - build and run test also show coverage report
3. make clean - remove all built files

How to use application:
./fresco_mb bind ~/path/to/FIFO &
./fresco_mb send ~/path/to/FIFO - and on next step input string or put "q" for exit
pkill fresco_mb - will send terminated signal to the application and unlink FIFO file.

Output will be show in termenial.

P.S:
- Be sure that versions of gcov and g++ are the same, otherwise coverage report will not be generated.