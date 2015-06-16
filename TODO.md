#TODO

./
+ Implement id file loading from flash memory
+ Implement UPnP protocols
+ Implement Circuit interactions
+ Implement command parsing
+ Possible implementations of device
	+ Write device-identifier/upnp variables & functions into a defined namespace. Manipulate that namespace such as in http://stackoverflow.com/questions/16485006/get-list-of-functions-in-a-namespace-at-runtime
	+ Write it into flash/EEPROM. Obtain from there.
+ Install virtualwire
+ Get wireless comms working

#WORKLOG
6.15.15
./client.ino
+ Removed pin as function input for most functions
+ Implemented brightness/AC things

6.11.2015 (decent hours)
./client.ino
+ Fixed establishContact by removing redundancy with onConnect

6.11.2015
./client.ino (non-decent hours)
+ Implemented serial cmd parsing
+ Fixed a few blink issues
+ Add establishContact (handshaking)

6.7.2015
./client.ino
+ Implemented blink, simple get,set,binary toggle functions
