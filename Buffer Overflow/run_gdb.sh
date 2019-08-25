gcc -g -z execstack -fno-stack-protector -o stack $1
gdb --nx stack
