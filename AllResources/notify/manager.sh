#!/bin/bash
#Script accept password using read command

PASS="centos5"
read -s -p "Password: " mypassword
echo ""
if [ "$mypassword" == "$PASS" ]; then
    echo "Password Accepted"
else
    echo "Access denied"
    exit 0
fi

echo "What would you like to do?"
echo "Here are the list of options."
echo "-Press 'f' to freeze the filesystem."
echo "-Press 't' to thaw the filesystem."
echo "-Press 'r' to rollback changes and restart the centOS."
echo "-Press 'q' to quit."

cp -r ~/Desktop/freezer/* ~/Desktop/.icebox

while :
do

read input

case $input in
	f) echo "Filesystem is now frozen.";
	   ./notify &;;
	t) echo "Filesystem is now thawed.";
	   kill -9 $(pidof notify);;
	r) echo "Filesystem is now rolling back and restart.";
	   ./parse; 	   
	   rm -rf ~/Desktop/.freezelog.txt;;
#	   shutdown -r now;;
	q) echo "Closing userspace application.";
	   exit 0;;
	*) echo "Incorrect response. Please select from the following:";
	   echo "Press 'f' to freeze the filesystem.";
	   echo "Press 't' to thaw the filesystem.";
	   echo "Press 'r' to rollback changes and restart the centOS.";
	   echo "Press 'q' to quit.";;
esac
done
