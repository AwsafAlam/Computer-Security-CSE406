gcc -g -z execstack -fno-stack-protector -o stack $1
./setpriv.sh
./stack
