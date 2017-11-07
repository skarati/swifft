gcc -mavx2 -O3 -fomit-frame-pointer -funroll-all-loops swifft-avx2-16.c
./a.out < input.txt > output_bswifft16.txt
