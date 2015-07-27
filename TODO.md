#TODO

./
+ look into RFID info
+ Fix establishContact to detect both remote and local commands
+ Implement id file loading from flash memory
+ Implement UPnP protocols
+ Possible implementations of device
	+ Write device-identifier/upnp variables & functions into a defined namespace. Manipulate that namespace such as in http://stackoverflow.com/questions/16485006/get-list-of-functions-in-a-namespace-at-runtime
	+ Write it into flash/EEPROM. Obtain from there.

+ Fix up the stuff with C++ classes. Try to make it readable and shit man.
+ Have to account for case where cmd isn't valid but cmd needs to clear. overflow it?

#WORKLOG

####7.27.2015
./socket.ino
+ renamed main file and folder as per panel's style
+ Implemented Commander class
+ Transmissions work +++

####7.24.2015
./client.ino
+ Started fixing up the codebase by implementing a BulbSocket class. Code as a whole is broken rn, still needs work.
+ deleted constants.h bc it was useless and renamed the folder to socket as per panel style

6.20.15
./client.ino
+ Get wireless comms working
+ Install virtualwire (now RadioHead RH_ASK)
+ Implement command parsing
+ Implement Circuit interactions

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
