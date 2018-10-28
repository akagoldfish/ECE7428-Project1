# A program to generate a number of random text files in a specific location

#!/bin/bash

help () {
	
	echo -e "\nUsage: $0 [-l] QUANTITY DIRECTORY"
	echo ""
	echo "### WILL ERASE DIRECTORY ###"
	echo ""	
	echo "Generates QUANTITY many random text files in a DIRECTORY."
	echo "Use -l for longer text files."
	echo ""

	exit 2
}

if [ $# -lt 2 ]
then
	echo "Too few arguements"
	help
elif [ $# -gt 3 ]
then
	echo "Too many arguements"
	help
fi

if [ $# -eq 3 ] && [ "$1" == "-l" ]
then
	
	QUAN=$2
	i=${3:$((${#str}-1)):1}

	if [ "$i" != "/" ]
	then
		DIR=$3"/"
	else
		DIR=$3
	fi 


	rm $DIR*
	if [ $QUAN -eq 0 ]
	then
		echo "Creating zero files..."
		exit 1
	fi

	for i in $(seq 1 $QUAN)
	do
		fortune -l | cat > $DIR"text"$i
	done
elif [ $# -eq 3 ] && [ "$1" != "-l" ]
then
	help
else
	QUAN=$1
	i=${2:$((${#str}-1)):1}

	if [ "$i" != "/" ]
	then
		DIR=$2"/"
	else
		DIR=$2
	fi 


	rm $DIR*
	if [ $QUAN -eq 0 ]
	then
		echo "Creating zero files..."
		exit 1
	fi

	for i in $(seq 1 $QUAN)
	do
		fortune | cat > $DIR"text"$i
	done
fi
