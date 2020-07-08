#/usr/bin/tcsh -f

if ($# != 2) then
	echo "$0 <output file name> <Application (UDP or ECHO)>"
	exit
else
	set FILE = $1
	set APP = $2
	echo "Parsing file: $FILE..."
endif

if ($APP == "ECHO") then
	set INITIALTIME = `grep "server received" $FILE | \
		head -1 | \
		awk '{print $3;}' | \
		awk -F s '{print $1;}'`

	set FINALTIME = `grep "server received" $FILE | \
		tail -1 | \
		awk '{print $3;}' | \
		awk -F s '{print $1;}'`

	set RXBITS = `grep "server received" $FILE | \
	     awk 'BEGIN {sum = 0;} {sum = sum + $6; } END {print sum*8;}'`
else if ($APP == "UDP") then
	set INITIALTIME = `grep "RX" $FILE | \
		head -1 | \
		awk '{print $15;}' | \
		awk -F ns '{print $1;}' | \
		awk -F + '{print $2/1000000000;}'`

	set FINALTIME = `grep "RX" $FILE | \
		tail -1 | \
		awk '{print $15;}' | \
		awk -F ns '{print $1;}' | \
		awk -F + '{print $2/1000000000;}'`

	set RXBITS = `grep "RX" $FILE | \
	     awk 'BEGIN {sum = 0;} {sum = sum + $3; } END {print sum*8;}'`
endif

set ELAPSEDTIME = `echo 4 k $FINALTIME $INITIALTIME - p | dc`

set THROUGHPUT = `echo 4 k $RXBITS $ELAPSEDTIME / p | dc`

echo "Initial time: $INITIALTIME Final time: $FINALTIME"
echo "Elapsed time: $ELAPSEDTIME"
echo "Bits Received by the server: $RXBITS"
echo "Throughput: $THROUGHPUT b/s"


