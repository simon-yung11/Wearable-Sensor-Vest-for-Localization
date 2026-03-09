#include "SparkFun_TMAG5273_Arduino_Library.h" // Used to send and receive specific information from our sensor
#include <Wire.h>                              // Used to establish serial communication on the I2C bus

#define TCA_ADDR 0x70  // Qwiic Mux default address
#define NUM_SENSORS 4

TMAG5273 sensors[NUM_SENSORS]; // Initialize hall-effect sensor

// I2C default address
uint8_t i2cAddress = TMAG5273_I2C_ADDRESS_INITIAL;

const int N = 10;

float xBuf[NUM_SENSORS][N];
float yBuf[NUM_SENSORS][N];
float zBuf[NUM_SENSORS][N];

float xSum[NUM_SENSORS] = {0};
float ySum[NUM_SENSORS] = {0}; 
float zSum[NUM_SENSORS] = {0};

int bufIndex[NUM_SENSORS] = {0}; 
int sampleCount[NUM_SENSORS] = {0};


void selectMuxChannel(uint8_t channel)
{
    if (channel > 7) return;
    Wire.beginTransmission(TCA_ADDR);
    Wire.write(1 << channel);
    Wire.endTransmission();
}


void setup()
{
    Serial.begin(115200);
    Wire.begin();
    //sensor.begin(i2cAddress, Wire);
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        selectMuxChannel(i);

        if (!sensors[i].begin(TMAG5273_I2C_ADDRESS_INITIAL, Wire))
        {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.println(" failed!");
            while (1);
        }
    }

    Serial.println("All sensors initialized.");
}


void loop()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        selectMuxChannel(i);

        if (sensors[i].getMagneticChannel() == 0)
            continue;

        float magX = sensors[i].getXData();
        float magY = sensors[i].getYData();
        float magZ = sensors[i].getZData();
       
        // Remove oldest sample from sum
        xSum[i] -= xBuf[i][bufIndex[i]];
        ySum[i] -= yBuf[i][bufIndex[i]];
        zSum[i] -= zBuf[i][bufIndex[i]];

        // Store new sample
        xBuf[i][bufIndex[i]] = magX;
        yBuf[i][bufIndex[i]] = magY;
        zBuf[i][bufIndex[i]] = magZ;

        // Add new sample to sum
        xSum[i] += magX;
        ySum[i] += magY;
        zSum[i] += magZ;

        bufIndex[i] = (bufIndex[i] + 1) % N;
        if (sampleCount[i] < N) sampleCount[i]++;

        float xAvg = xSum[i] / sampleCount[i];
        float yAvg = ySum[i] / sampleCount[i];
        float zAvg = zSum[i] / sampleCount[i];


        Serial.print("S"); Serial.print(i); Serial.print("_X_avg:");
        Serial.print(xAvg); Serial.print(" ");

        Serial.print("S"); Serial.print(i); Serial.print("_Y_avg:");
        Serial.print(yAvg); Serial.print(" ");

        Serial.print("S"); Serial.print(i); Serial.print("_Z_avg:");
        Serial.print(zAvg); Serial.print(" ");

        Serial.println();

        //Sanity Check for printing values of One Sensor
        // Serial.print("X:");
        // Serial.print(magX);
        // Serial.print(" ");

        //Serial.print("X_avg:");
        //Serial.print(xAvg);
        //Serial.print(" ");

        // Serial.print("Y:");
        // Serial.print(magY);
        // Serial.print(" ");

        //Serial.print("Y_avg:");
        //Serial.print(yAvg);
        //Serial.print(" ");

        // Serial.print("Z:");
        // Serial.print(magZ);
        // Serial.print(" ");

        //Serial.print("Z_avg:");
        //Serial.println(zAvg);

        delay(100);

    }
    //else
    //{
        // If there is an issue, stop the magnetic readings and restart sensor/example
      //  Serial.println("Mag Channels disabled, stopping..");
      //  while (1);
    //}    
    // Checks if mag channels are on - turns on in setup
    //if (sensor.getMagneticChannel() != 0)
}