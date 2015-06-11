/*
 * Socket Client
 *
 * Written by Joshua Paul A. Chan
 */


 /*
  * Setup info (pin defs, etc)
  */

#define relayPin 3
#define txPin 0
#define rxPin 1

#define ON true
#define OFF false

String DEVID = "01af";

String inputString = "";
bool stringComplete = false;

String feats = "get||set||do";


String action = "";
String feat = "";
int val = 100;

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

void executeCmd(String cmd) {
    Serial.print("Executing : ");
    Serial.println(cmd);

    char achar = (char)action[0];

    switch (achar) {
        case 'g':
            // Universal get
            break;
        case 's':
            // Universal set
            break;
        case 'd':
            togglePower(relayPin);
            break;
    }
    return;
}

void clearCmd() {
    action = "";
    feat = "";
    val = 0;
}

bool isConnected() {
    // TODO
}

void establishContact() {
    while (Serial.available() <= 0) {
        Serial.print("DEVID:");
        Serial.println(DEVID);
        delay(300);
    }

    onConnect();
}

// Things to do on connect
void onConnect() {

    while (Serial.available() <= 0) {
        Serial.println("DEVID:01af");
        delay(300);
    }

    // hello
    blink(relayPin, 1, 1000);
    return;
}

// Things to do on disconnect
void onDisconnect() {

    // bye-bye
    blink(relayPin, 2, 1000);
    return;
}

/*
 * Socket Functionality
 */

// Function for getting the power status of the lightbulb
bool getPower(int pin) {
    // Assuming pin is in output
    if ((0 <= pin) and (13 >= pin)) {
        return ((bool)bitRead(PORTD, pin));
    }
    return false;
}

// function for setting the power status of the lightbulb
void setPower(int pin, bool state) {
    if (state == true) {
        digitalWrite(pin, HIGH);
    }
    else {
        digitalWrite(pin, LOW);
    }
    return;
}

// function for toggling the power status of the lightbulb
void togglePower(int pin) {
    bool state = getPower(pin);
    setPower(pin, (not state));
}

// function for getting the brightness level of the lightbulb
int getBrightness(int pin) {
    return 1;
}

//function for setting the brightness level of the lightbulb
void setBrightness(int pin, int val) {
    return;
}


/*
 * Diagnostics
 */

// Blink light n times
void blink(int switchPin, int n, int d) {
    // Don't give me a stupid number
    if ((0 >= n) or (0 >= d)) {return;}
    // Don't give me a huge number
    if ((10 < n) or (10000 < d)) {return;}

    for (int i = 0; i < n; i++) {
        togglePower(switchPin);
        delay(d);
        togglePower(switchPin);
        delay(d);
    }
}

// Pulses in an sinusodial fashion
void pulse(int switchPin, int n) {
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

void setup() {
  // Set it low first
  digitalWrite(relayPin, LOW);

  // Then set as output
  pinMode(relayPin, OUTPUT);
  setPower(relayPin, ON);

  // Blink at first power on
  blink(relayPin, 2, 100);
  setPower(relayPin, ON);

  Serial.begin(9600);
  Serial.println("[socket] is ready.");

  establishContact();
}

void loop() {
  if (stringComplete) {
    if (isValidCmd(inputString)) {
      executeCmd(inputString);
      clearCmd();
    }
    inputString = "";
    stringComplete = false;
    Serial.flush();
  }
}

void serialEvent() {
  while (Serial.available()) {
    // Get new byte(s)
    char rcvChar = (char)Serial.read();

    inputString += rcvChar;
    if (rcvChar == '\n') {
      stringComplete = true;
//      Serial.print(inputString);
    }
  }
}
