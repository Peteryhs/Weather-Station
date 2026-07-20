#include <Wire.h>
#include <Adafruit_BME680.h>

// Custom I2C pins
#define I2C_SDA 25
#define I2C_SCL 26

// BME680 I2C address (0x76 since SDO -> GND)
#define BME680_ADDR 0x76

Adafruit_BME680 bme;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  // Start I2C on custom pins
  Wire.begin(I2C_SDA, I2C_SCL);

  Serial.println("Initializing BME680...");

  if (!bme.begin(BME680_ADDR, &Wire)) {
    Serial.println("Could not find a valid BME680 sensor. Check wiring!");
    while (1) delay(10);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320°C for 150 ms

  Serial.println("BME680 initialized successfully!");
  delay(1000);
}

void loop() {
  if (!bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    delay(10000);
    return;
  }

  Serial.println("----- BME680 Readings -----");

  Serial.print("Temperature = ");
  Serial.print(bme.temperature);
  Serial.println(" °C");

  Serial.print("Humidity = ");
  Serial.print(bme.humidity);
  Serial.println(" %");

  Serial.print("Pressure = ");
  Serial.print(bme.pressure / 100.0);
  Serial.println(" hPa");

  Serial.print("Gas Resistance = ");
  Serial.print(bme.gas_resistance / 1000.0);
  Serial.println(" KOhms");

  // Rough air quality estimate based on gas resistance
  // Higher resistance generally = cleaner air (fewer VOCs)
  float gasKOhms = bme.gas_resistance / 1000.0;
  Serial.print("Air Quality: ");
  if (gasKOhms > 50) {
    Serial.println("Good");
  } else if (gasKOhms > 20) {
    Serial.println("Moderate");
  } else if (gasKOhms > 10) {
    Serial.println("Poor");
  } else {
    Serial.println("Very Poor");
  }

  Serial.println();
  delay(10000);
}