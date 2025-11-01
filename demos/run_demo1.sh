#!/bin/bash

#CONFIG
BUILD_DIR=../build/main_floppy.img #The path to the SCHizOS floppy image.

WAIT_TIME=3.0 #How much seconds to wait for boot
BETWEEN_COMMAND_INTERVALS=1.0 #How much seconds to sleep between individual commands?

QEMU_MONITOR_PORT=45454 #
QEMU_HOST=127.0.0.1
#END CONFIG

function simulate_command(){
	local cmd="$1"
    (for (( i=0; i<${#cmd}; i++ )); do
        (
		char="${cmd:$i:1}"
		case "$char" in
			" ") char="spc" ;;
			"/") char="slash" ;;
			":") char="colon" ;;
			*) ;;
		esac
		echo "sendkey ${char}"
		
		)
    done)
	echo "sendkey ret"
	sleep $BETWEEN_COMMAND_INTERVALS
}

#See: run.sh for reference.
qemu-system-i386 -audiodev pa,id=speaker -machine pcspk-audiodev=speaker -alt-grab -fda ${BUILD_DIR} -monitor tcp:${QEMU_HOST}:${QEMU_MONITOR_PORT},server,nowait &

#Get the process ID
QEMU_PID=$!

#Without -alt-grab it would not work for some reason. It QEMU will say, that it is deprecated but don't worry.

#Waiting for successful boot.
sleep $WAIT_TIME

#Begin simulation
{
	#echo "sendkey d"
	#echo "sendkey i"
	#echo "sendkey r"
	#echo "sendkey ret"
	
	simulate_command "dir"
	simulate_command "pwd"
	simulate_command "cat userinfo.txt"
	simulate_command "cd userland"
	simulate_command "ls"
	simulate_command "cat userinfo.txt"
} | nc $QEMU_HOST $QEMU_MONITOR_PORT #Pipe them into NetCat

wait $PID
