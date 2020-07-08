#!env/bin/python3

import sys

if( len(sys.argv) != 3 or sys.argv[1] == "--help"):
    print("getSingleIPs.py -> Python3 script used to remove duplicates of IP addresses from a list of sender IPs extracted from an ns3::UdpServer RX log.")
    print("Usage: python3 getSingleIPs.py [input file name] [output file name]")
    print("The input file must be a list of IP addresses, being each address in one line. The addresses are sent to a file in the same style with the name given in the second argument.")

addresses = []

with open(sys.argv[1], 'r') as dataFile:

    for line in dataFile:

        if(line in addresses):
            continue

        else:
            addresses.append(line)

with open(sys.argv[2], 'w+') as resultsFile:

    for address in addresses:

        resultsFile.write(address)
