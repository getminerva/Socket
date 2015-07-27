/*
 * Socket Client
 *
 * Written by Joshua Paul A. Chan
 */

/*
 * Setup info (includes, pin defs, etc)
 */

#include <RH_ASK.h>
#include <SPI.h> // Needed for compilation

#include <BulbSocket.h>
#include <Commander.h>

#define powerPin 3

#define txPin 12
#define rxPin 11
#define pttPin 10

// Create transceiver object
RH_ASK recvr(2000, rxPin, txPin, pttPin);

// Create BulbSocket object
castle::BulbSocket bulb(powerPin, 0, 100);

// Create Commander object
castle::Commander cmdr;

// String[] feats("get", "set", "do");

/*
 * Communications
 */

// Tests to see if is connected
bool isConnected() {
    // Pings src transmitter?
    return true;
}

void establishContact() {
    while (!isConnected()) {
        Serial.print("UUID:");
        Serial.println("CHAN-EMB01-Photon");
        delay(300);
    }
    onConnect();
}

// Things to do on connect (hello)
void onConnect() {
    blink(1, 1000);
    return;
}

// Things to do on disconnect (bye-bye)
void onDisconnect() {
    Serial.println("Ta-ta, yo.");
}

void serialEvent() {
    while (Serial.available()) {
        // Get new byte(s)
        char rcvChar = (char)Serial.read();

        // Hand them off to commander
        cmdr.recvCmd((String)rcvChar);
    }
}

// Interrupt for Output Power
void interrupt_outputPowwer() {
    //Delay before output
    int val = bulb.getLevel();
    delayMicroseconds(val+1000);
    digitalWrite(powerPin, HIGH);

    delayMicroseconds(200);
    digitalWrite(powerPin, LOW);
    return;
}

void initInterrupt() {
    return;
}

/*
 * Diagnostics
 */

// Blink light n times
void blink(int n, int d) {
    if ((0 >= n) or (0 >= d)) {return;} // Don't give me neg numbers
    if ((10 < n) or (10000 < d)) {return;} // Don't give me a huge number

    for (int i = 0; i < n; i++) {
        bulb.toggle();
        delay(d);
        bulb.toggle();
        delay(d);
    }
}

/*
 * Main stuff
 */

void execCmd(String cmd) {
    Serial.print("Executing command: ");
    Serial.println(cmd);

    // TODO
    // switch (achar) {
    //     case 's':
    //         bulb.setLevel(val);
    //         break;
    //     case 'd':
    //         bulb.toggle();
    //         break;
    // }

    delay(100);
    Serial.println("Command executed.");
}

void setup() {
    Serial.begin(9600);

    // Blink at first power on
    blink(2, 100);

    // Component diagnostics
    if (!recvr.init()) {
        Serial.println("[socket:recvr] init failed.");
    }
    if (false) {
        Serial.println("[socket:bulb] init failed.");
    }
    if (false) {
        Serial.println("[socket:cmdr] init failed.");
    }

    Serial.println("[socket] is ready.");

    // establishContact();
}

void loop() {
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (recvr.recv(buf, &buflen)) {
        // driver.printBuffer("Got: ", buf, buflen);  // Diagnostic printing
        String msg = castle::strFromCharArray(buf, buflen);
        // Serial.println(msg); // Diagnostic printing

        if (50 <= (cmdr.getRawCmd()).length()) { // Thresholding - once the string gets too big, it's cleared
            cmdr.clearCmd();
        }
        cmdr.recvCmd(msg);

    }
    if (cmdr.hasValidCmd()) {
        // cmdr.execCmd(cmdr.getRawCmd()); // TODO: y'know, in fact, I could just let cmdr handle the string manipulations and checking and implement an execCmd function that takes the rawCmd and does whatever based on that.
        // Actually, yeah, I'll do that.
        execCmd(cmdr.getRawCmd());
        cmdr.clearCmd();
    }
    // Diagnostic printing
    Serial.println("Commander has: " + cmdr.getRawCmd());
    delay(10);
}
