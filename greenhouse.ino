// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define TEMPERATURE_PIN 8
#define RELAY_OPEN_PIN 9
#define RELAY_CLOSE_PIN 10

#define OPEN_TEMP 30
#define CLOSE_TEMP 20
#define OPERATION_TIME 30 // seconds

#define OPEN 2
#define CLOSED 4

int status = 0;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(TEMPERATURE_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

/*
   The setup function. We only start the sensors here
*/
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  pinMode(RELAY_OPEN_PIN, OUTPUT);
  pinMode(RELAY_CLOSE_PIN, OUTPUT);

  digitalWrite(RELAY_OPEN_PIN, HIGH);  //default => off
  digitalWrite(RELAY_CLOSE_PIN, HIGH);

  // Start up the library
  sensors.begin();
}

/*
   Main function, get and show the temperature
*/
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if (tempC != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(tempC);

//    // Open/Close logic
    if(tempC >= OPEN_TEMP) {
      openWindow();
    } else if(tempC <= CLOSE_TEMP) {
      closeWindow();
    }
  }
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
}

void openWindow() {
  if (status != OPEN) {
    digitalWrite(RELAY_CLOSE_PIN, HIGH);  // First off
    digitalWrite(RELAY_OPEN_PIN, LOW);
    delay(OPERATION_TIME * 1000);
    digitalWrite(RELAY_OPEN_PIN, HIGH);
    status = OPEN;
  }
}
 void closeWindow() {
  if (status != CLOSED) {
    digitalWrite(RELAY_OPEN_PIN, HIGH);   // First off
    digitalWrite(RELAY_CLOSE_PIN, LOW);
    delay(OPERATION_TIME * 1000);
    digitalWrite(RELAY_CLOSE_PIN, HIGH);
    status = CLOSED;
  }
}
