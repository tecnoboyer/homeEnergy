#include <SoftwareSerial.h>
#include <DHT.h>

// Define pins
#define DHTPIN 2          // DHT22 data pin
#define DHTTYPE DHT22     // DHT22 type
#define PIR_PIN 5         // PIR sensor pin
#define CO2_RX_PIN 3      // MH-Z19B RX pin
#define CO2_TX_PIN 4      // MH-Z19B TX pin

// Initialize sensors
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial co2Serial(CO2_RX_PIN, CO2_TX_PIN); // RX, TX for MH-Z19B

// Variables to store sensor data
float temperature = 0;
float humidity = 0;
int co2Level = 0;
bool isOccupied = false;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  co2Serial.begin(9600); // MH-Z19B default baud rate
  dht.begin();

  // Initialize PIR sensor pin
  pinMode(PIR_PIN, INPUT);

  Serial.println("Sensor initialization complete.");
}

void loop() {
  // Read temperature and humidity from DHT22
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Read CO2 level from MH-Z19B
  co2Level = readCO2();

  // Read occupancy from PIR sensor
  isOccupied = digitalRead(PIR_PIN);

  // Print sensor data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, CO2: ");
  Serial.print(co2Level);
  Serial.print(" ppm, Occupancy: ");
  Serial.println(isOccupied ? "Occupied" : "Not Occupied");

  // Delay before next reading
  delay(2000);
}

// Function to read CO2 level from MH-Z19B
int readCO2() {
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  byte response[9];

  // Send command to the sensor
  co2Serial.write(cmd, 9);

  // Wait for the response
  if (co2Serial.available()) {
    co2Serial.readBytes(response, 9);
    if (response[0] == 0xFF && response[1] == 0x86) {
      int high = response[2];
      int low = response[3];
      return high * 256 + low;
    }
  }
  return -1; // Return -1 if reading fails
}