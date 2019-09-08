from __future__ import print_function
from scapy.all import *
import time
import binascii

command = "echo 'pwned'"

if os.geteuid() != 0:
    sys.exit("Run as root ~sudo su")


def handle_packet(packet):
    print(packet.summary())
    if packet.haslayer(DHCP):
        mac_addr = packet[Ether].src
        raw_mac = binascii.unhexlify(mac_addr.replace(":", ""))
        print('---')
        # print(packet.summary())
        print(ls(packet))
    if packet.haslayer(DNS):
        print('---')
        print(packet.summary())
        # print(ls(packet))

    return


if __name__ == "__main__":
    sniff(filter='ip host 192.168.2.4', iface='wlp6s0', prn=handle_packet)
