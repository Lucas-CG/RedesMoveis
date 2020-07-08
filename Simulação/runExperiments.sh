#!/bin/bash

resultsFileName="results.csv"

echo "protocol,packet_size,step_size,num_clients,pdr,throughput,average_delay,first_transmitted_packet_delay,global_first_packet_delay" > $resultsFileName

for packetSize in 512 1024 1536 2048; do

	for stepSize in 25 50 75 100; do

		for numClients in 1 2 3; do

			for stack in ns3::Dot11sStack ns3::FlameStack; do

				#if i'm using FLAME
				if [ $stack = ns3::FlameStack ]; then

					rootArray=(ff:ff:ff:ff:ff:ff)

				#if i'm using IEEE 802.11s (HWMP/PMP)
				else

					rootArray=(ff:ff:ff:ff:ff:ff 00:00:00:00:00:01 00:00:00:00:00:07)

				fi

				for address in ${rootArray[@]}; do

					for iteration in 1; do

						echo "Iteration $iteration: now running with packets of $packetSize bytes, step size of $stepSize meters, a 3 x 3 grid, $numClients clients, $stack protocol stack and with $address as the root."
						echo 'Command: ./waf --run mesh-lucas --command-template="%s --x-size=3 --y-size=3 --step='$stepSize' --num-clients='$numClients' --time=200 --packet-interval=0.01 --packet-size='$packetSize' --interfaces=1 --stack='$stack' --root='$address' > teste.txt 2>&1"'

						./waf --run mesh-lucas --command-template="%s --x-size=3 --y-size=3 --step=$stepSize --num-clients=$numClients --time=200 --packet-interval=0.01 --packet-size=$packetSize --interfaces=1 --stack=$stack --root=$address" > teste.txt 2>&1

						#extract metrics

						#PDR (%)

						#received packets
						received=$(cat teste.txt | grep "RX" | wc -l)

						#sent packets
						sent=$(cat teste.txt | grep "TX " | wc -l)

						pdr=$(echo '100 * '"$received"' / '"$sent"'' | bc -l)

						echo "PDR = ${received}/${sent} = $pdr"

						#Average throughput (in Bps) (100 seconds is the transmission time!)

						throughput=$(echo ''"$packetSize"' * '"$received"' / 100' | bc -l)
						throughputMbps=$(echo ''"$throughput"' * 8 / 1000000' | bc -l)
						echo "Average throughput = $throughput Bps = $throughputMbps Mbps"

						#delay (in ms)

						protocol=0

						if [ $stack = ns3::FlameStack ]; then

							protocol="FLAME"

						else

							if [ $address = 00:00:00:00:00:07 ]; then

								protocol=HWMPHybridFarRoot

							elif [ $address = ff:ff:ff:ff:ff:ff ]; then

								protocol=HWMPProActive

							elif [ $address = 00:00:00:00:00:01 ]; then

								protocol=HWMPHybridNearRoot

							fi

						fi


						delayFileName=simDelay-${protocol}-${packetSize}B-${stepSize}mstep-${numClients}clients-${iteration}.csv

						# firstPacketIpsByHostFileName=firstPacketIpsByHost-${protocol}-${packetSize}B-${gridSize}colSize-${stepSize}mstep-${numClients}clients.csv

						echo "sequence_number,rx_time_ms,delay_ms" > $delayFileName

						totalDelay=0

						firstTxTime=9999999999999999999999999999999

						cat teste.txt | grep RX | awk '{print $11, $13, $15, $17}' | tr --delete ns+ > delayLog.txt

						while IFS='' read -r line || [[ -n "$line" ]]; do

							delay=$(echo $line | awk '{print $4}')

							delayInMs=$(echo ''"$delay"' / 1000000' | bc -l)

							totalDelay=$(echo ''"$totalDelay"' + '"$delayInMs"'' | bc -l)


							rxTime=$(echo $line | awk '{print $3}')

							rxTimeInMs=$(echo ''"$rxTime"' / 1000000' | bc -l)

							#fetching tx time for calculating the first packet delay
							txTime=$(echo $line | awk '{print $1}')

							if (( $(echo "$firstTxTime > $txTime" | bc -l) )); then
								firstTxTime=$txTime
								firstTransmittedPacketDelayInMs=$delayInMs
							fi

							sequenceNumber=$(echo $line | awk '{print $1}')

							#delay by global sequence number (UDP uid) - due to the variable number of clients

							echo $sequenceNumber,$rxTimeInMs,$delayInMs >> $delayFileName

						done < delayLog.txt

						rm delayLog.txt

						echo "First transmitted packet delay = $firstTransmittedPacketDelayInMs ms"

						#average delay
						averageDelay=$(echo ''"$totalDelay"'' / ''"$received"'' | bc -l)

						echo "Average delay = $totalDelay/$received = $averageDelay"


						#first packet delay (first transmitted packet, global and per user)

						#global (who has won the packet race?)
						globalFirstPacketDelay=$(cat teste.txt | grep RX | head -1 | awk '{print $17}' | tr --delete ns+)
						globalFirstPacketDelayInMs=$(echo ''"$globalFirstPacketDelay"' / 1000000' | bc -l)

						echo "First packet delay (packet race winner) = $globalFirstPacketDelayInMs"


						#per user

						#extracting the IPs of all clients

						#output from the log file (contains repeated IPs)
						# cat teste.txt | grep RX | awk '{print $6}' > repeatedIpList.txt
						#
						# python3 getSingleIPs.py repeatedIpList.txt singleIpList.txt
						#
						# rm repeatedIpList.txt

						# for ip in $(cat singleIpList.txt); do
						#
						# 	hostFirstPacketDelay=$(cat teste.txt | grep RX | grep $ip | head -1 | awk '{print $17}' | tr --delete ns+)
						# 	hostFirstPacketDelayInMs=$(echo ''"$hostFirstPacketDelay"' / 1000000' | bc -l)
						#
						# 	echo $ip,$hostFirstPacketDelayInMs >> $firstPacketIpsByHostFileName
						#
						# 	if (( $(echo "$firstTransmittedPacketDelayInMs > $hostFirstPacketDelayInMs" | bc -l) )); then
						# 		firstTransmittedPacketDelayInMs=$hostFirstPacketDelayInMs
						# 	fi

							#comma strings to add in the correct format of the CSV result file

							# ipCommaString+="first_packet_delay_"
							# ipCommaString+=ip
							# ipCommaString+=","
							#
							# delayCommaString+=$hostFirstPacketDelayInMs
							# delayCommaString+=","
						#
						# done

						# rm singleIpList.txt

						#ipCommaString=${ipCommaString%?}
						#delayCommaString=${delayCommaString%?}

						echo "Adding data to results file..."

						#echo "pdr,throughput,average_delay,first_transmitted_packet_delay,global_first_packet_delay,$ipCommaString" > $resultsFileName
						#echo "$pdr,$throughput,$average_delay,$firstTransmittedPacketDelayInMs,$globalFirstPacketDelayInMs,$delayCommaString" >> $resultsFileName

						#result file header: protocol,packet_size,step_size,grid_size,num_clients,pdr,throughput,average_delay,first_transmitted_packet_delay,global_first_packet_delay"
						echo "$protocol,$packetSize,$stepSize,$numClients,$pdr,$throughput,$averageDelay,$firstTransmittedPacketDelayInMs,$globalFirstPacketDelayInMs" >> $resultsFileName

						echo "Done."

					done
				done
			done
		done
	done
done

rm teste.txt
