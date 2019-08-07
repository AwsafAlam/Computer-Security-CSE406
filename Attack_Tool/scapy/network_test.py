#!/usr/bin/env python3

#call ./dhcp.py <function>
# where function is one of "init", "discover", "request" 
# Use iface as parameter for init

# Install Python3 and scapy to run this

import json

#Kill IPv6 warning:
import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)

import scapy
from scapy.all import *


#discover may be the first
def do_discover():
 print("In do_discover")

# VERY important: Tell scapy that we do not require IP's to be swapped to identify an answer 
 scapy.all.conf.checkIPaddr = False

settings = eval(open("dhcp.conf").read())

iface = settings["iface"]
 
 mac = get_if_raw_hwaddr(iface)
 
 dhcp_discover = (
 Ether(dst="ff:ff:ff:ff:ff:ff") /
 IP(src="0.0.0.0", dst="255.255.255.255") /
 UDP(sport=68, dport=67) /
 BOOTP(chaddr=mac, xid=5678) /
 DHCP(options=[("message-type","discover"),"end"]))
# dump what we plan to send
# ls(dhcp_discover)

disc_ans = srp1(dhcp_discover,iface=iface,filter="udp and (port 67 or 68)")

# The answer is a DHCP_OFFER - check it out
 print("Options:", disc_ans[DHCP].options)
# print("xID:", disc_ans[BOOTP].xid)

# Save the offer to be used in a request
 settings["serverIP"] = disc_ans[BOOTP].siaddr
 settings["clientIP"] = disc_ans[BOOTP].yiaddr
 settings["XID"] = disc_ans[BOOTP].xid

with open('dhcp.conf','w') as file:
 file.write(json.dumps(settings))

return


#this does a request without a discover first
def do_request():
 print("In do_request")
# VERY important: Tell scapy that we do not require IP's to be swapped to identify an answer 
 scapy.all.conf.checkIPaddr = False

settings = eval(open("dhcp.conf").read())

iface = settings["iface"]
 mac = get_if_raw_hwaddr(iface)
 
 dhcp_request = (
 Ether(dst="ff:ff:ff:ff:ff:ff") /
 IP(src="0.0.0.0", dst="255.255.255.255") /
 UDP(sport=68, dport=67) /
 BOOTP(chaddr=mac) /
 DHCP(options=[("message-type","request"),("server_id",settings["serverIP"]),("requested_addr",settings["clientIP"] ),"end"]))
# dump what we plan to send
# ls(dhcp_request)

ans_req = srp1(dhcp_request,iface=iface,filter="udp and (port 67 or 68)") 
 print("Options:", ans_req[DHCP].options)
# print("xID:", ans_req[BOOTP].xid)

#this does a discover - then a request
def do_discover_request():
 print("In do_discover_request - not implemented yet")

return

def do_none():
 print("Try ./dhcp.py help")

return

def do_init():
 try:
 iface = sys.argv[2]
 except:
 print("init must have as second parameter the relevant interface")
 return

settings = {'serverIP': "255.255.255.255" , "clientIP": "0.0.0.0", "XID": "0","iface": iface}
 with open('dhcp.conf','w') as file:
 file.write(json.dumps(settings))
 return

def do_help():
 print("Examples of usage:")
 print(" ./dhcp.py init eth0 Second Param: interface. This is stored in dhcp.conf")
 print(" sudo ./dhcp.py discover Send a DHCP DISCOVER to the server and get a DHCP_OFFER")
 print(" sudo ./dhcp.py request Send a DHCP REQUEST to the server and get a DHCP_ACK")


action = {'discover': do_discover, 'request': do_request, 'discover_request': do_discover_request, 'init': do_init, 'help': do_help}

#main code

try:
 command = sys.argv[1]
except:
 command = "none"


action.get(command, do_none)()