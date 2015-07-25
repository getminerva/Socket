/*
 * Socket Client
 *
 * Written by Joshua Paul A. Chan
 */


/*
 * Setup info (pin defs, etc)
 */

#include <RH_ASK.h>
#include <SPI.h> // Needed for compilation

#define powerPin 3
#define zeroDetectPin 2

#define txPin 12
#define rxPin 11
#define pttPin 10
#define transSpeed 2000
#define serialSpeed 9600

#define ON true
#define OFF false

RH_ASK recvr(transSpeed, rxPin, txPin, pttPin);

String inputString = "";
bool stringComplete = false;

String feats = "get||set||do";

String action = "";
String feat = "";

int val = 100;
int pre_val = 0;


/*
 * Data Persistency
 */

void loadData() {
  return;
}

void saveData() {
  return;
}

void getDeviceId() {
  return;
}

/*
 * Communications
 */

// Tests to see if is connected
bool isConnected() {
    // Pings src transmitter?
    return true;
}

void establishContact() {
    while (0 == sizeof(inputString)) {
        Serial.print("UUID:");
        Serial.println(UUID);
        delay(300);
    }
    onConnect();
}

// Things to do on connect
void onConnect() {
    // hello
    blink(1, 1000);
    return;
}

// Things to do on disconnect
void onDisconnect() {
    // bye-bye
    blink(2, 1000);
    return;
}

/*
 * CMD stuff
 */

// Validates cmd
bool isValidCmd(String s) {
    Serial.print("Testing: ");
    Serial.println(s);

    // Pre-format
    s.toLowerCase();
    s.trim();

    // Must be prefaced with "cmd::"
    if (!s.startsWith("cmd::")) {
        Serial.println("Test 1 failed.");
        return false;
    }
    s = s.substring(5);

    //  Serial.println(s);

    // Action must be 'get', 'set' or 'do'
    if (!s.startsWith("get::") &&  !s.startsWith("set::") && !s.startsWith("do::")) {
        Serial.println("Test 2 failed.");
        return false;
    }
    int action_pos = s.indexOf(':') + 1;
    action = s.substring(0, action_pos);
    s = s.substring(action_pos + 1);

    // Serial.println(s);

    // Can only have feature 'power' or 'brightness'
    if (!s.startsWith("power") && !s.startsWith("brightness")) {
        Serial.println("Test 3 failed.");
        return false;
    }
    int pos = s.indexOf(':') + 1;
    feat = s.substring(action_pos + 1, pos);
    s = s.substring(pos + 1);

    // Serial.println(s);

    // Value should only exist on set
    // Value should be between 0-100 (percentage)

    val = s.toInt();
    if ((0 > val) or (100 < val)) {return false;}

    Serial.println("Tests passed.");

    return true;
}

// Executes the cmd
void executeCmd(String cmd) {
    Serial.print("Executing : ");
    Serial.println(cmd);

    char achar = (char)action[0];

    switch (achar) {
        case 's':
            setBrightness(val);
            break;
        case 'd':
            togglePower();
            break;
    }

    // case 'g' is default
    Serial.print("Brightness: ");
    Serial.println(getBrightness());
    return;
}

// Clear the internal cmd
void clearCmd() {
    action = "";
    feat = "";
    pre_val = val;
    val = 0;

}

void serialEvent() {
    while (Serial.available()) {
        // Get new byte(s)
        char rcvChar = (char)Serial.read();

        inputString += rcvChar;
        if (rcvChar == '\n') {
            stringComplete = true;
        }
    }
}

String strFromBuffer(uint8_t* buf, uint8_t buflen) {
    String o = "";
    for (uint8_t i = 0; i < buflen; ++i) {
        /*Serial.print((char)*(buf+i));*/
        o += ((char)*(buf+i));
    }
    return o;
}

void recvCmd() {
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t buflen = sizeof(buf);

    if (recvr.recv(buf, &buflen)) {

        recvr.printBuffer(" ", buf, buflen);
        // Get new byte(s)

        String msg = strFromBuffer(buf, buflen);
        Serial.println(msg);

        inputString += msg;
        // TODO: How to check if whole message recv'd
        if (inputString.endsWith("::end")) {
            stringComplete = true;
        }
    }
}

/*
 * Socket Functionality
 */

// Function for getting the power status of the lightbulb
bool getPower() {
    return ((bool)bitRead(PORTD, powerPin));
}

// function for setting the power status of the lightbulb
void setPower(bool state) {
    pre_val = val;
    if (state == true) {
        digitalWrite(powerPin, HIGH);
        val = 100;
    }
    else {
        digitalWrite(powerPin, LOW);
        val = 0;
    }
    return;
}

// function for toggling the power status of the lightbulb
void togglePower() {
    // if Relay
    bool state = getPower();
    setPower((!state));

    //if Triac
    if (0 < val) {setBrightness(0);}
    else {setBrightness(pre_val);}
}

// function for getting the brightness level of the lightbulb
int getBrightness() {
    return val;
}

//function for setting the brightness level of the lightbulb
void setBrightness(int n_val) {
    pre_val = val;
    if (100 < n_val) {n_val = 100;}
    else if (0 > val) {n_val = 0;}
    val = n_val;
    return;
}


// Interrupt Output Power
void interrupt_outputPowwer() {
    //Delay before output
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
    // Don't give me a stupid number
    if ((0 >= n) or (0 >= d)) {return;}
    // Don't give me a huge number
    if ((10 < n) or (10000 < d)) {return;}

    for (int i = 0; i < n; i++) {
        togglePower();
        delay(d);
        togglePower();
        delay(d);
    }
}

// Pulses in an sinusodial fashion
void pulse(int n) {
    // Don't give me a stupid number
    if (0 >= n) {return;}
    // Don't give me a huge number
    if (10 < n) {return;}

    // Phase length in mS
    int phaseLen = 1000;

    // Brighten until phaseLen/2, then dim
    // TODO

    return;
}

/*
 * Main stuff
 */

    // Set it low first
void setup() {
    digitalWrite(powerPin, LOW);

    // Then set as output
    pinMode(powerPin, OUTPUT);
    setPower(ON);

    // Blink at first power on
    blink(2, 100);
    setPower(ON);

    Serial.begin(serialSpeed);

    if (!recvr.init()) {
        Serial.println("[socket::recvr] init failed.");
    }

    Serial.println("[socket] is ready.");

    establishContact();
}

void loop() {
    recvCmd();

    if (stringComplete) {
        Serial.print(inputString);
        if (isValidCmd(inputString)) {
            executeCmd(inputString);
            clearCmd();
        }
        inputString = "";
        stringComplete = false;
    }
    if (!isConnected()) {
        onDisconnect();
    }
}
