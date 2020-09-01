/**
 * Serial Call-Response
 * by Tom Igoe.
 *
 * Sends a byte out the serial port, and reads 3 bytes in.
 * Sets foregound color, xpos, and ypos of a circle onstage
 * using the values returned from the serial port.
 * Thanks to Daniel Shiffman  and Greg Shakar for the improvements.
 *
 * Note: This sketch assumes that the device on the other end of the serial
 * port is going to send a single byte of value 65 (ASCII A) on startup.
 * The sketch waits for that byte, then sends an ASCII A whenever
 * it wants more data.
 */

int bgcolor;			     // Background color
int fgcolor;			     // Fill color
Serial myPort;                       // The serial port
int[] serialInArray = new int[3];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive
int xpos, ypos;		             // Starting position of the ball
boolean firstContact = false;        // Whether we've heard from the microcontroller

void setup() {
	size(256, 256);  // Stage size
	noStroke();      // No border on the next thing drawn

	// Set the starting position of the ball (middle of the stage)
	xpos = width / 2;
	ypos = height / 2;

	// Print a list of the serial ports, for debugging purposes:
	printArray(Serial.list());

	// I know that the first port in the serial list on my mac
	// is always my  FTDI adaptor, so I open Serial.list()[0].
	// On Windows machines, this generally opens COM1.
	// Open whatever port is the one you're using.
	String portName = Serial.list()[0];
	myPort = new Serial(this, portName, 9600);
}

void draw() {
	background(bgcolor);
	fill(fgcolor);
	// Draw the shape
	ellipse(xpos, ypos, 20, 20);
}

void serialEvent(Serial myPort) {
	// read a byte from the serial port:
	int inByte = myPort.read();
	// if this is the first byte received, and it's an A,
	// clear the serial buffer and note that you've
	// had first contact from the microcontroller. 
	// Otherwise, add the incoming byte to the array:
	if (firstContact == false) {
		if (inByte == 'A') {
			myPort.clear();          // clear the serial port buffer
			firstContact = true;     // you've had first contact from the microcontroller
			myPort.write('A');       // ask for more
		}
	}
	else {
		// Add the latest byte from the serial port to array:
		serialInArray[serialCount] = inByte;
		serialCount++;

		// If we have 3 bytes:
		if (serialCount > 2) {
			xpos = serialInArray[0];
			ypos = serialInArray[1];
			fgcolor = serialInArray[2];

			// print the values (for debugging purposes only):
			println(xpos + "\t" + ypos + "\t" + fgcolor);

			// Send a capital A to request new sensor readings:
			myPort.write('A');
			// Reset serialCount:
			serialCount = 0;
		}
	}
}

Serial myPort;      // The serial port
int whichKey = -1;  // Variable to hold keystoke values
int inByte = -1;    // Incoming serial data

void setup() {
	size(400, 300);
	// create a font with the third font available to the system:
	PFont myFont = createFont(PFont.list()[2], 14);
	textFont(myFont);

	// List all the available serial ports:
	printArray(Serial.list());

	// I know that the first port in the serial list on my mac
	// is always my  FTDI adaptor, so I open Serial.list()[0].
	// In Windows, this usually opens COM1.
	// Open whatever port is the one you're using.
	String portName = Serial.list()[0];
	myPort = new Serial(this, portName, 9600);
}

void draw() {
	background(0);
	text("Last Received: " + inByte, 10, 130);
	text("Last Sent: " + whichKey, 10, 100);
}

void serialEvent(Serial myPort) {
	inByte = myPort.read();
}

void keyPressed() {
	// Send the keystroke out:
	myPort.write(key);
	whichKey = key;
}

/**
 * Many Serial Ports
 *
 * Read data from the multiple Serial Ports
 */

Serial[] myPorts = new Serial[2];  // Create a list of objects from Serial class
int[] dataIn = new int[2];         // a list to hold data from the serial ports

void setup() {
	size(400, 300);
	// print a list of the serial ports:
	printArray(Serial.list());
	// On my machine, the first and third ports in the list
	// were the serial ports that my microcontrollers were 
	// attached to.
	// Open whatever ports ares the ones you're using.

  // get the ports' names:
	String portOne = Serial.list()[0];
	String portTwo = Serial.list()[2];
	// open the ports:
	myPorts[0] = new Serial(this, portOne, 9600);
	myPorts[1] = new Serial(this, portTwo, 9600);
}


void draw() {
	// clear the screen:
	background(0);
	// use the latest byte from port 0 for the first circle
	fill(dataIn[0]);
	ellipse(width / 3, height / 2, 40, 40);
	// use the latest byte from port 1 for the second circle
	fill(dataIn[1]);
	ellipse(2 * width / 3, height / 2, 40, 40);
}

/**
  * When SerialEvent is generated, it'll also give you
  * the port that generated it.  Check that against a list
  * of the ports you know you opened to find out where
  * the data came from
*/
void serialEvent(Serial thisPort) {
	// variable to hold the number of the port:
	int portNumber = -1;

	// iterate over the list of ports opened, and match the 
	// one that generated this event:
	for (int p = 0; p < myPorts.length; p++) {
		if (thisPort == myPorts[p]) {
			portNumber = p;
		}
	}
	// read a byte from the port:
	int inByte = thisPort.read();
	// put it in the list that holds the latest data from each port:
	dataIn[portNumber] = inByte;
	// tell us who sent what:
	println("Got " + inByte + " from serial port " + portNumber);
}

/*
The following Wiring/Arduino code runs on both microcontrollers that
were used to send data to this sketch:

void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read analog input, divide by 4 to make the range 0-255:
  int analogValue = analogRead(0)/4;
  Serial.write(analogValue);
  // pause for 10 milliseconds:
  delay(10);
}


*/

/**
 * Simple Read
 *
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port

void setup()
{
	size(200, 200);
	// I know that the first port in the serial list on my mac
	// is always my  FTDI adaptor, so I open Serial.list()[0].
	// On Windows machines, this generally opens COM1.
	// Open whatever port is the one you're using.
	String portName = Serial.list()[0];
	myPort = new Serial(this, portName, 9600);
}

void draw()
{
	if (myPort.available() > 0) {  // If data is available,
		val = myPort.read();         // read it and store it in val
	}
	background(255);             // Set background to white
	if (val == 0) {              // If the serial value is 0,
		fill(0);                   // set fill to black
	}
	else {                       // If the serial value is not 0,
		fill(204);                 // set fill to light gray
	}
	rect(50, 50, 100, 100);
}

/*

// Wiring / Arduino Code
// Code for sensing a switch status and writing the value to the serial port.

int switchPin = 4;                       // Switch connected to pin 4

void setup() {
  pinMode(switchPin, INPUT);             // Set pin 0 as an input
  Serial.begin(9600);                    // Start serial communication at 9600 bps
}

void loop() {
  if (digitalRead(switchPin) == HIGH) {  // If switch is ON,
	Serial.write(1);               // send 1 to Processing
  } else {                               // If the switch is not ON,
	Serial.write(0);               // send 0 to Processing
  }
  delay(100);                            // Wait 100 milliseconds
}

*/

/**
 * Simple Write.
 *
 * Check if the mouse is over a rectangle and writes the status to the serial port.
 * This example works with the Wiring / Arduino program that follows below.
 */


Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

void setup()
{
	size(200, 200);
	// I know that the first port in the serial list on my mac
	// is always my  FTDI adaptor, so I open Serial.list()[0].
	// On Windows machines, this generally opens COM1.
	// Open whatever port is the one you're using.
	String portName = Serial.list()[0];
	myPort = new Serial(this, portName, 9600);
}

void draw() {
	background(255);
	if (mouseOverRect() == true) {  // If mouse is over square,
		fill(204);                    // change color and
		myPort.write('H');              // send an H to indicate mouse is over square
	}
	else {                        // If mouse is not over square,
		fill(0);                      // change color and
		myPort.write('L');              // send an L otherwise
	}
	rect(50, 50, 100, 100);         // Draw a square
}

boolean mouseOverRect() { // Test if mouse is over square
	return ((mouseX >= 50) && (mouseX <= 150) && (mouseY >= 50) && (mouseY <= 150));
}


/*
  // Wiring/Arduino code:
 // Read data from the serial and turn ON or OFF a light depending on the value

 char val; // Data received from the serial port
 int ledPin = 4; // Set the pin to digital I/O 4

 void setup() {
 pinMode(ledPin, OUTPUT); // Set pin as OUTPUT
 Serial.begin(9600); // Start serial communication at 9600 bps
 }

 void loop() {
 while (Serial.available()) { // If data is available to read,
 val = Serial.read(); // read it and store it in val
 }
 if (val == 'H') { // If H was received
 digitalWrite(ledPin, HIGH); // turn the LED on
 } else {
 digitalWrite(ledPin, LOW); // Otherwise turn it OFF
 }
 delay(100); // Wait 100 milliseconds for next reading
 }

 */