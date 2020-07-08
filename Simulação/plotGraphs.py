#!/env/bin/python3

#-*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import csv

flameData = []
hybrid80211sFarRootData = []
hybrid80211sNearRootData = []
proactive80211sData = []

with open("results.csv", 'r') as resultsFile:

    reader = csv.DictReader(resultsFile, delimiter=",")

    for row in reader:

        if(row['protocol'] == "FLAME"):

            flameData.append(row)

        elif(row['protocol'] == "HWMPProActive"):

            proactive80211sData.append(row)

        elif(row['protocol'] == "HWMPHybridNearRoot"):

            hybrid80211sNearRootData.append(row)

        elif(row['protocol'] == "HWMPHybridFarRoot"):

            hybrid80211sFarRootData.append(row)

#graphs:

#metrics x packet size/step size/num clients

#pdr x packetsize


#fetch from data info with clients = 1, 2, 3 and stepsize = 20, 50, 75 and 100

#--------------------------------------------------------------------------------------------------------------------

#loop to fetch data variating the NUMBER OF CLIENTS

for packetSize in [512, 1024, 1536, 2048]:

    for stepSize in [25, 50, 75, 100]:

        pdr = {}
        avgDelay = {}
        firstTransmittedPacketDelay = {}
        globalFirstPacketDelay = {}

        pdr['flame'] = []
        avgDelay['flame'] = []
        firstTransmittedPacketDelay['flame'] = []
        globalFirstPacketDelay['flame'] = []

        pdr['hybrid80211sFarRoot'] = []
        avgDelay['hybrid80211sFarRoot'] = []
        firstTransmittedPacketDelay['hybrid80211sFarRoot'] = []
        globalFirstPacketDelay['hybrid80211sFarRoot'] = []

        pdr['hybrid80211sNearRoot'] = []
        avgDelay['hybrid80211sNearRoot'] = []
        firstTransmittedPacketDelay['hybrid80211sNearRoot'] = []
        globalFirstPacketDelay['hybrid80211sNearRoot'] = []

        pdr['proactive80211s'] = []
        avgDelay['proactive80211s'] = []
        firstTransmittedPacketDelay['proactive80211s'] = []
        globalFirstPacketDelay['proactive80211s'] = []

        for row in flameData:

            if (int(row['packet_size']) == packetSize and int(row['step_size']) == stepSize):

                pdr['flame'].append( float(row['pdr']) )
                avgDelay['flame'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['flame'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['flame'].append( float(row['global_first_packet_delay']) )


        for row in hybrid80211sFarRootData:

            if (int(row['packet_size']) == packetSize and int(row['step_size']) == stepSize):

                pdr['hybrid80211sFarRoot'].append( float(row['pdr']) )
                avgDelay['hybrid80211sFarRoot'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['hybrid80211sFarRoot'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['hybrid80211sFarRoot'].append( float(row['global_first_packet_delay']) )


        for row in hybrid80211sNearRootData:


            if (int(row['packet_size']) == packetSize and int(row['step_size']) == stepSize):

                pdr['hybrid80211sNearRoot'].append( float(row['pdr']) )
                avgDelay['hybrid80211sNearRoot'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['hybrid80211sNearRoot'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['hybrid80211sNearRoot'].append( float(row['global_first_packet_delay']) )


        for row in proactive80211sData:

            if ( int(row['packet_size']) == packetSize and int(row['step_size']) == stepSize):

                print(row['pdr'])

                pdr['proactive80211s'].append(float(row['pdr']))
                avgDelay['proactive80211s'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['proactive80211s'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['proactive80211s'].append( float(row['global_first_packet_delay']) )


        #pdr

        plt.xlabel("Número de Clientes")
        plt.ylabel("Pacotes Entregues (%)")

        print(pdr['hybrid80211sNearRoot'])

        plt.plot([1, 2, 3], pdr['flame'], 'r', label="FLAME")
        plt.plot([1, 2, 3], pdr['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([1, 2, 3], pdr['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([1, 2, 3], pdr['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/pdr_x_numclients_packet" + str(packetSize) + "_step" + str(stepSize) + ".png", bbox_inches = 'tight')

        plt.clf()


        #avg delay

        plt.xlabel("Número de Clientes")
        plt.ylabel("Atraso Médio (ms)")

        plt.plot([1, 2, 3], avgDelay['flame'], 'r', label="FLAME")
        plt.plot([1, 2, 3], avgDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([1, 2, 3], avgDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([1, 2, 3], avgDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/avgDelay_x_numclients_packet" + str(packetSize) + "_step" + str(stepSize) + ".png", bbox_inches = 'tight')

        plt.clf()


        #first transmitted packet delay

        plt.xlabel("Número de Clientes")
        plt.ylabel("Atraso do Primeiro Pacote Transmitido (ms)")

        plt.plot([1, 2, 3], firstTransmittedPacketDelay['flame'], 'r', label="FLAME")
        plt.plot([1, 2, 3], firstTransmittedPacketDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([1, 2, 3], firstTransmittedPacketDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([1, 2, 3], firstTransmittedPacketDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/firstTransmittedPacketDelay_x_numclients_packet" + str(packetSize) + "_step" + str(stepSize) + ".png", bbox_inches = 'tight')

        plt.clf()


        #global first packet delay

        plt.xlabel("Número de Clientes")
        plt.ylabel("Atraso do Primeiro Pacote Recebido (ms)")

        plt.plot([1, 2, 3], globalFirstPacketDelay['flame'], 'r', label="FLAME")
        plt.plot([1, 2, 3], globalFirstPacketDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([1, 2, 3], globalFirstPacketDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([1, 2, 3], globalFirstPacketDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/globalFirstPacketDelay_x_numclients_packet" + str(packetSize) + "_step" + str(stepSize) + ".png", bbox_inches = 'tight')

        plt.clf()


#----------------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------------------------

#loop to fetch data variating the STEP SIZE

for packetSize in [512, 1024, 1536, 2048]:

    for numClients in [1, 2, 3]:

        pdr = {}
        avgDelay = {}
        firstTransmittedPacketDelay = {}
        globalFirstPacketDelay = {}

        pdr['flame'] = []
        avgDelay['flame'] = []
        firstTransmittedPacketDelay['flame'] = []
        globalFirstPacketDelay['flame'] = []

        pdr['hybrid80211sFarRoot'] = []
        avgDelay['hybrid80211sFarRoot'] = []
        firstTransmittedPacketDelay['hybrid80211sFarRoot'] = []
        globalFirstPacketDelay['hybrid80211sFarRoot'] = []

        pdr['hybrid80211sNearRoot'] = []
        avgDelay['hybrid80211sNearRoot'] = []
        firstTransmittedPacketDelay['hybrid80211sNearRoot'] = []
        globalFirstPacketDelay['hybrid80211sNearRoot'] = []

        pdr['proactive80211s'] = []
        avgDelay['proactive80211s'] = []
        firstTransmittedPacketDelay['proactive80211s'] = []
        globalFirstPacketDelay['proactive80211s'] = []


        for row in flameData:


            if (int(row['packet_size']) == packetSize and int(row['num_clients']) == numClients):

                pdr['flame'].append( float(row['pdr']) )
                avgDelay['flame'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['flame'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['flame'].append( float(row['global_first_packet_delay']) )


        for row in hybrid80211sFarRootData:


            if ( int(row['packet_size']) == packetSize and int(row['num_clients']) == numClients ):

                pdr['hybrid80211sFarRoot'].append( float(row['pdr']) )
                avgDelay['hybrid80211sFarRoot'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['hybrid80211sFarRoot'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['hybrid80211sFarRoot'].append( float(row['global_first_packet_delay']) )


        for row in hybrid80211sNearRootData:


            if (int(row['packet_size']) == packetSize and int(row['num_clients']) == numClients):

                pdr['hybrid80211sNearRoot'].append( float(row['pdr']) )
                avgDelay['hybrid80211sNearRoot'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['hybrid80211sNearRoot'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['hybrid80211sNearRoot'].append( float(row['global_first_packet_delay']) )


        for row in proactive80211sData:


            if (int(row['packet_size']) == packetSize and int(row['num_clients']) == numClients):

                pdr['proactive80211s'].append( float(row['pdr']) )
                avgDelay['proactive80211s'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['proactive80211s'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['proactive80211s'].append( float(row['global_first_packet_delay']) )


        #pdr

        plt.xlabel("Distância entre Roteadores (m)")
        plt.ylabel("Pacotes Entregues (%)")

        plt.plot([25, 50, 75, 100], pdr['flame'], 'r', label="FLAME")
        plt.plot([25, 50, 75, 100], pdr['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([25, 50, 75, 100], pdr['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([25, 50, 75, 100], pdr['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/pdr_x_stepsize_packet" + str(packetSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


        #avg delay

        plt.xlabel("Distância entre Roteadores (m)")
        plt.ylabel("Atraso Médio (ms)")

        plt.plot([25, 50, 75, 100], avgDelay['flame'], 'r', label="FLAME")
        plt.plot([25, 50, 75, 100], avgDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([25, 50, 75, 100], avgDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([25, 50, 75, 100], avgDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/avgDelay_x_stepsize_packet" + str(packetSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


        #first transmitted packet delay

        plt.xlabel("Distância entre Roteadores (m)")
        plt.ylabel("Atraso do Primeiro Pacote Transmitido (ms)")

        plt.plot([25, 50, 75, 100], firstTransmittedPacketDelay['flame'], 'r', label="FLAME")
        plt.plot([25, 50, 75, 100], firstTransmittedPacketDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([25, 50, 75, 100], firstTransmittedPacketDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([25, 50, 75, 100], firstTransmittedPacketDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/firstTransmittedPacketDelay_x_stepsize_packet" + str(packetSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


        #global first packet delay

        plt.xlabel("Distância entre Roteadores (m)")
        plt.ylabel("Atraso do Primeiro Pacote Recebido (ms)")

        plt.plot([25, 50, 75, 100], globalFirstPacketDelay['flame'], 'r', label="FLAME")
        plt.plot([25, 50, 75, 100], globalFirstPacketDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([25, 50, 75, 100], globalFirstPacketDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([25, 50, 75, 100], globalFirstPacketDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/globalFirstPacketDelay_x_stepsize_packet" + str(packetSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


#----------------------------------------------------------------------------------------------------------


#--------------------------------------------------------------------------------------------------------------------

#loop to fetch data variating the PACKET SIZE

for stepSize in [25, 50, 75, 100]:

    for numClients in [1, 2, 3]:

        pdr = {}
        avgDelay = {}
        firstTransmittedPacketDelay = {}
        globalFirstPacketDelay = {}

        pdr['flame'] = []
        avgDelay['flame'] = []
        firstTransmittedPacketDelay['flame'] = []
        globalFirstPacketDelay['flame'] = []

        pdr['hybrid80211sFarRoot'] = []
        avgDelay['hybrid80211sFarRoot'] = []
        firstTransmittedPacketDelay['hybrid80211sFarRoot'] = []
        globalFirstPacketDelay['hybrid80211sFarRoot'] = []

        pdr['hybrid80211sNearRoot'] = []
        avgDelay['hybrid80211sNearRoot'] = []
        firstTransmittedPacketDelay['hybrid80211sNearRoot'] = []
        globalFirstPacketDelay['hybrid80211sNearRoot'] = []

        pdr['proactive80211s'] = []
        avgDelay['proactive80211s'] = []
        firstTransmittedPacketDelay['proactive80211s'] = []
        globalFirstPacketDelay['proactive80211s'] = []


        for row in flameData:

            if (int(row['step_size']) == stepSize and int(row['num_clients']) == numClients):

                pdr['flame'].append( float(row['pdr']) )
                avgDelay['flame'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['flame'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['flame'].append( float(row['global_first_packet_delay']) )


        for row in hybrid80211sFarRootData:


            if ( int(row['step_size']) == stepSize and int(row['num_clients']) == numClients):

                pdr['hybrid80211sFarRoot'].append( float(row['pdr']) )
                avgDelay['hybrid80211sFarRoot'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['hybrid80211sFarRoot'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['hybrid80211sFarRoot'].append( float(row['global_first_packet_delay']) )


        for row in hybrid80211sNearRootData:

            if ( int(row['step_size']) == stepSize and int(row['num_clients']) == numClients):

                pdr['hybrid80211sNearRoot'].append( float(row['pdr']) )
                avgDelay['hybrid80211sNearRoot'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['hybrid80211sNearRoot'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['hybrid80211sNearRoot'].append( float(row['global_first_packet_delay']) )


        for row in proactive80211sData:

            if ( int(row['step_size']) == stepSize and int(row['num_clients']) == numClients):

                pdr['proactive80211s'].append( float(row['pdr']) )
                avgDelay['proactive80211s'].append( float(row['average_delay']) )
                firstTransmittedPacketDelay['proactive80211s'].append( float(row['first_transmitted_packet_delay']) )
                globalFirstPacketDelay['proactive80211s'].append( float(row['global_first_packet_delay']) )


        #pdr

        plt.xlabel("Tamanho dos Pacotes (bytes)")
        plt.ylabel("Pacotes Entregues (%)")

        plt.plot([512, 1024, 1536, 2048], pdr['flame'], 'r', label="FLAME")
        plt.plot([512, 1024, 1536, 2048], pdr['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([512, 1024, 1536, 2048], pdr['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([512, 1024, 1536, 2048], pdr['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/pdr_x_packetsize_step" + str(stepSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


        #avg delay

        plt.xlabel("Tamanho dos Pacotes (bytes)")
        plt.ylabel("Atraso Médio (ms)")

        plt.plot([512, 1024, 1536, 2048], avgDelay['flame'], 'r', label="FLAME")
        plt.plot([512, 1024, 1536, 2048], avgDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([512, 1024, 1536, 2048], avgDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([512, 1024, 1536, 2048], avgDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/avgDelay_x_packetsize_step" + str(stepSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


        #first transmitted packet delay

        plt.xlabel("Tamanho dos Pacotes (bytes)")
        plt.ylabel("Atraso do Primeiro Pacote Transmitido (ms)")

        plt.plot([512, 1024, 1536, 2048], firstTransmittedPacketDelay['flame'], 'r', label="FLAME")
        plt.plot([512, 1024, 1536, 2048], firstTransmittedPacketDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([512, 1024, 1536, 2048], firstTransmittedPacketDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([512, 1024, 1536, 2048], firstTransmittedPacketDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/firstTransmittedPacketDelay_x_packetsize_step" + str(stepSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


        #global first packet delay

        plt.xlabel("Tamanho dos Pacotes (bytes)")
        plt.ylabel("Atraso do Primeiro Pacote Recebido (ms)")

        plt.plot([512, 1024, 1536, 2048], globalFirstPacketDelay['flame'], 'r', label="FLAME")
        plt.plot([512, 1024, 1536, 2048], globalFirstPacketDelay['hybrid80211sFarRoot'], 'b', label="802.11s - Híbrido 1")
        plt.plot([512, 1024, 1536, 2048], globalFirstPacketDelay['hybrid80211sNearRoot'], 'g', label="802.11s - Híbrido 2")
        plt.plot([512, 1024, 1536, 2048], globalFirstPacketDelay['proactive80211s'], 'y', label="802.11s - Somente reativo")

        plt.legend(loc='best')

        plt.savefig("graphs/globalFirstPacketDelay_x_packetsize_step" + str(stepSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

        plt.clf()


#----------------------------------------------------------------------------------------------------------


#delay over sequence

#delayFileName=simDelay-${protocol}-${packetSize}B-${stepSize}mstep-${numClients}clients-${iteration}.csv

#echo "sequence_number,rx_time_ms,delay_ms" > $delayFileName

for protocol in ["FLAME", "HWMPProActive", "HWMPHybridNearRoot", "HWMPHybridFarRoot"]:

    for stepSize in [25, 50, 75, 100]:

        for packetSize in [512, 1024, 1536, 2048]:

            for numClients in [1, 2, 3]:

                with open("simDelay-" + protocol + "-" + str(packetSize) + "B-" + str(stepSize) + "mstep-" + str(numClients) + "clients-1" + ".csv", 'r') as delayFile:

                    reader = csv.DictReader(delayFile, delimiter=",")

                    seqNums = []
                    delays = []

                    for row in reader:
                        seqNums.append( int(row['sequence_number']) )
                        delays.append( float(row['delay_ms']) )


                plt.vlines(seqNums, [ 0 for i in range( len(seqNums) ) ], delays, 'k' )
                plt.xlabel("Sequência")
                plt.ylabel("Atraso (ms)")

                plt.savefig("graphs/seqnums_x_delay_" + protocol + "_packet" + str(packetSize) + "_step" + str(stepSize) + "_clients" + str(numClients) + ".png", bbox_inches = 'tight')

                plt.clf()
