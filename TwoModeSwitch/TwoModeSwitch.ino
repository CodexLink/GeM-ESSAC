#define SwitchMode_One 26
#define SwitchMode_Two 27

#define INVERSE_LOW HIGH
#define INVERSE_HIGH LOW


void setup()
{
    Serial.begin(115200);
    pinMode(SwitchMode_One, INPUT_PULLUP);
    pinMode(SwitchMode_Two, INPUT_PULLUP);
}

void loop()
{
    if ((digitalRead(SwitchMode_One) == INVERSE_HIGH) && (digitalRead(SwitchMode_Two) == INVERSE_LOW))
    {
        Serial.println("Mode 1 Switch");
    }
    else if ((digitalRead(SwitchMode_One) == INVERSE_HIGH) && (digitalRead(SwitchMode_Two) == INVERSE_HIGH))
    {
        Serial.println("Mode 2 Switch");
    }
    else if ((digitalRead(SwitchMode_One) == INVERSE_LOW) && (digitalRead(SwitchMode_Two) == INVERSE_HIGH))
    {
        Serial.println("Mode 3 Switch");
    }
    else if ((digitalRead(SwitchMode_One) == INVERSE_LOW) && (digitalRead(SwitchMode_Two) == INVERSE_LOW))
    {
        Serial.println("Mode 4 Switch");
    }
    delay(500);
}