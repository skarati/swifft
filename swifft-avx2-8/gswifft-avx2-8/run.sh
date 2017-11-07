gcc -mavx2 -O3 -fomit-frame-pointer -funroll-all-loops swifft-avx2-8.c
./a.out < input.txt  > output_gswifft8.txt
