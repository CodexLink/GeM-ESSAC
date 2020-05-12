#include <ShiftOut.h>
#define redLed 12 // Set Led Pins
#define greenLed 11
#define blueLed 10
ShiftOut<1> shift;

// BYTE CHECKTHISBYTE {A,B,C,D,E,F,G,DEC}
byte DigitSegment_0[] = {1, 1, 1, 1, 1, 1, 1, 1};
byte DigitSegment_1[] = {0, 0, 0, 0, 0, 0, 0, 0};
byte DigitSegment_2[] = {1, 1, 0, 1, 1, 0, 1, 0};
byte DigitSegment_3[] = {1, 1, 1, 1, 0, 0, 1, 0};
byte DigitSegment_4[] = {0, 1, 1, 0, 0, 1, 1, 0};
byte DigitSegment_5[] = {1, 0, 1, 1, 0, 1, 1, 0};
byte DigitSegment_6[] = {1, 0, 1, 1, 1, 1, 1, 0};
byte DigitSegment_7[] = {1, 1, 1, 0, 0, 0, 0, 0};
byte DigitSegment_8[] = {1, 1, 1, 1, 1, 1, 1, 1};
byte DigitSegment_9[] = {1, 1, 1, 1, 0, 1, 1, 0};
char SegmentRespect[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
int led = 0;
void setup()
{
    pinMode(redLed, OUTPUT);
    pinMode(greenLed, OUTPUT);
    pinMode(blueLed, OUTPUT);
    Serial.begin(9600);
    // declare pins: data, clock, latch
    shift.begin(26, 28, 29);
    pinMode(34, OUTPUT);
    pinMode(35, OUTPUT);
}

void loop()
{
    shift.setAllLow(); // set all to zero
    //shift.setAllHigh(); // set all to zero
    shift.setHigh(led); // set led to high
    shift.write();      // write out to 74HC595
    //Serial.print(led);
    //Serial.print(" | ");
    //Serial.print(SegmentRespect[led]);
    //Serial.print(" | ");
    Serial.println(led = (led + 1) % shift.getDataWidth()); // increment led
    if (SegmentRespect[led] % 2 == 0)
    {
        Serial.println("Changed To #1");
        digitalWrite(34, HIGH);
        digitalWrite(12, HIGH);
        digitalWrite(11, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(35, LOW); // wait a bit

    }
    else
    {
        Serial.println("Changed To #2");
        digitalWrite(35, HIGH);
        digitalWrite(12, LOW); // wait a bit
        digitalWrite(11, LOW); // wait a bit
        digitalWrite(10, LOW); // wait a bit
        digitalWrite(34, LOW); // wait a bit

    }
    delay(300);
}
