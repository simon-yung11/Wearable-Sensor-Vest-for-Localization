#include "SparkFun_TMAG5273_Arduino_Library.h" // Used to send and recieve specific information from our sensor
#include <Wire.h>                              // Used to establish serial communication on the I2C bus

TMAG5273 sensor; // Initialize hall-effect sensor

// I2C default address
uint8_t i2cAddress = TMAG5273_I2C_ADDRESS_INITIAL;

const int N = 10;

float xBuf[N], yBuf[N], zBuf[N]; 
float xSum = 0, ySum = 0, zSum = 0;

int bufIndex = 0; 
int sampleCount = 0;

void setup()
{
    Serial.begin(115200);
    Wire.begin();
    sensor.begin(i2cAddress, Wire);
}

void loop()
{
    // Checks if mag channels are on - turns on in setup 
    if (sensor.getMagneticChannel() != 0) 
    { 
        float magX = sensor.getXData(); 
        float magY = sensor.getYData(); 
        float magZ = sensor.getZData();

        // Remove oldest sample from sum 
        xSum -= xBuf[bufIndex]; 
        ySum -= yBuf[bufIndex]; 
        zSum -= zBuf[bufIndex];

        // Store new sample 
        xBuf[bufIndex] = magX; 
        yBuf[bufIndex] = magY; 
        zBuf[bufIndex] = magZ; 
        
        // Add new sample to sum 
        xSum += magX; 
        ySum += magY; 
        zSum += magZ;

        bufIndex = (bufIndex + 1) % N; 
        if (sampleCount < N) sampleCount++; 
        float xAvg = xSum / sampleCount; 
        float yAvg = ySum / sampleCount; 
        float zAvg = zSum / sampleCount; 
        
        // Serial.print("X:"); 
        // Serial.print(magX); 
        // Serial.print(" "); 
        
        Serial.print("X_avg:"); 
        Serial.print(xAvg); 
        Serial.print(" "); 
        
        // Serial.print("Y:"); 
        // Serial.print(magY); 
        // Serial.print(" "); 
        
        Serial.print("Y_avg:"); 
        Serial.print(yAvg); 
        Serial.print(" "); 
        
        // Serial.print("Z:"); 
        // Serial.print(magZ); 
        // Serial.print(" "); 
        
        Serial.print("Z_avg:"); 
        Serial.println(zAvg);

        delay(100);
        } 
        else 
        { 
            // If there is an issue, stop the magnetic readings and restart sensor/example 
            Serial.println("Mag Channels disabled, stopping.."); 
            while (1); 
            } 
            delay(0); 
}