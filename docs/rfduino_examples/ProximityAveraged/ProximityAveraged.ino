/*
The sketch demonstrates how to do receive Bluetooth Low Energy 4
RSSI signal strength information from the RFduino.

This information is available to the RFduino after a connection
has been established.  The iPhone application is able to access
the RSSI signal strength information before a connection is
established if needed.

This sketch establishes four zones.  In the first zone (furtherist away),
both leds are off.  In the second zone (closer), led1 is on (green).  In
the third zone, both leds are on (yellow).  In the forth and closest zone
led2 is on (red).
*/

#include <RFduinoBLE.h>

#define ITERATIONS 16

// pin 3 on the RGB sheild is the green led
int led_green = 3;

// pin 2 on the RGB shield is the red led
int led_red = 2;

int average_rssi = 0;
int iterations = 0;

void setup() {
  // configure leds
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);

  // turn on led1 to indicate we are waiting for a connection
  digitalWrite(led_green, HIGH);

  // drop the transmission power to move the zones closer to
  // the RFduino (which also reduces the interference from
  // other wireless devices and reflection)
  // (all zones are within approxiately 10 feet of the RFduino)
  RFduinoBLE.txPowerLevel = -20;
  
  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
  // switch to lower power mode
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onConnect()
{
  // turn off led1 when we have a connection
  digitalWrite(led_green, LOW);
}

void RFduinoBLE_onDisconnect()
{
  // don't leave the leds on if they disconnect
  digitalWrite(led_green, LOW);
  digitalWrite(led_red, LOW);
}

// returns the dBm signal strength indicated by the receiver
// received signal strength indication (-0dBm to -127dBm)
void RFduinoBLE_onRSSI(int rssi)
{
  average_rssi += rssi;
  if (++iterations >= ITERATIONS)
  {
    iterations = 0;
    average_rssi /= ITERATIONS;
    // turn off both leds if the signal strength is out of range
    if (average_rssi <= -85)
    {
      digitalWrite(led_green, LOW);
      digitalWrite(led_red, LOW);
    }
    // in range, but signal strength is weak
    else if (average_rssi <= -70)
    {
      digitalWrite(led_green, HIGH);
      digitalWrite(led_red, LOW);
    }
    // in range, signal strength is stronger
    else if (average_rssi <= -55)
    {
      digitalWrite(led_green, HIGH);
      digitalWrite(led_red, HIGH);
    }
    // in range, signal strength is strong
    else
    {
      digitalWrite(led_green, LOW);
      digitalWrite(led_red, HIGH);
    }
    average_rssi = 0;
  }
}
