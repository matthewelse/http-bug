#!/usr/bin/env python3
from scapy.all import *

segments = [
    open('segment1.txt', 'rb').read(),
    open('segment2.txt', 'rb').read()
]

# listen for a packet on port 80
syn = sniff(count=1, filter='tcp and host ' + sys.argv[1] + ' and port 80')

src_ip = syn[0][IP].src
src_port = syn[0][TCP].sport
seq_no = syn[0][TCP].seq
ack_no = seq_no + 1

# send a SYN-ACK
# we'll see if we need any more options, but this reflects what is seen in the Wireshark capture
SYN_ACK = TCP(sport=80, dport=src_port, flags="SA", seq=seq_no, ack=ack_no, options=[('MSS', 1460)])

IP_PACKET = IP(dst=src_ip)

ANSWER = sr1(IP_PACKET/SYN_ACK)

# We should now receive an ACK for our SYN-ACK and the GET request from the client.
GEThttp = sniff(filter="tcp and port 80",count=2)

GEThttp[0].show()

ack_no += len(GEThttp[0].load)
seq_no += 1

# so, now we'll send them a response.
response_TCP = []

response_TCP.append(TCP(sport=80, dport=src_port, flags="PA", seq=seq_no, ack=ack_no))

seq_no += len(segments[0])
response_TCP.append(TCP(sport=80, dport=src_port, flags="PAF", seq=seq_no, ack=ack_no))

send(IP_PACKET/response_TCP[0]/segments[0])
send(IP_PACKET/response_TCP[1]/segments[1])
