Tasks shown in LAB

Preliminery:

check the privilege of /etc/shadow /etc/passwd file
test passwd sudp command....

1. Test uid euid

Create two user tom and bob
create a C file tom.c in /tmp folder from tom's terminal
create the executable file
set the uid bit ON of executable file
run the executable from bob's terminal

2. Get root privilege as a normal user (seed)

Create the file asroot.c and asroot.sh from root account
create executable from asroot.c file
set the uid bit ON for both file
run the executables as seed user
==> asroot.sh will have permission denied message but asroot will run successfully
you can get a shell (write system("sh") in the C file )

3. Checking capability leak exploit
create a file zzz in etc folder as root (with permission r/w to root, r for others)

create cap_leak.c as root
create the executable from cap_leak.c file
set the uid bit ON
run the cap_leak executable from seed terminal
You will get a shell as seed user
write "echo zzzzzzzzzz >& (file_id) " , then exit from shell
check /etc/zzz file , you successfully write as a seed user
