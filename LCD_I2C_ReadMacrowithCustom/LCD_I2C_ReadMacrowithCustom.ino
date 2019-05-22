// Demonstrates Outputting String with F Macro with Custom Function Integrated

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD_I2C(0x27, 20, 4);

void setup()
{
    Serial.begin(115200);
    LCD_I2C.init();
    LCD_I2C.backlight();
}
void loop()
{
    //LCD_I2C.setCursor(0, 0);
    //LCD_I2C.print(F("1 |Hello World| w F"));
    //LCD_I2C.setCursor(0, 1);
    //LCD_I2C.print(F("2 |Hello World| wo F"));
    LCD_I2C.setCursor(0, 0);
    LCD_I2C.print(F("LewF - "));
    LCD_I2C.print(StringHelper("|Hello World|", "Length", 0, 1), DEC);
    LCD_I2C.print(F(" LewoF - "));
    LCD_I2C.print(strlen("|Hello World|"));
    LCD_I2C.setCursor(0, 2);
    LCD_I2C.print(F("BwF - "));
    LCD_I2C.print(StringHelper("|Hello World|", "Bytes", 0, 3), DEC);
    LCD_I2C.print(F(" BwoF - "));
    LCD_I2C.print(sizeof("|Hello World|"));

    delay(10000000000); // Run Once
}
/*void loop()
{
    LCD_I2C.setCursor(0, 0);
    LCD_I2C.print(F("1 |Hello World| w F"));
    LCD_I2C.setCursor(0, 1);
    LCD_I2C.print(F("2 |Hello World| wo F"));
    LCD_I2C.setCursor(0, 2);
    LCD_I2C.print(F("LewF - "));
    LCD_I2C.print(StringHelper("|Hello World|", "Length"), DEC);
    LCD_I2C.print(F(" LewoF - "));
    LCD_I2C.print(strlen("|Hello World|"));
    LCD_I2C.setCursor(0, 3);
    LCD_I2C.print(F("BwF - "));
    LCD_I2C.print(StringHelper("|Hello World|", "Bytes"), DEC);
    LCD_I2C.print(F(" BwoF - "));
    LCD_I2C.print(sizeof("|Hello World|"));

    delay(10000000000); // Run Once
}*/

static short unsigned StringHelper(const char *StringGiven, const char *TypeGiven, short PosX, short PosY)
{
    Serial.println(F("StringHelper Function START"));
    Serial.print(F("StringGiven -> "));
    Serial.print(StringGiven);
    Serial.print(F(", Type Given - > "));
    Serial.println(TypeGiven);

    Serial.print(F("StringGiven Length Before Conversion -> "));
    Serial.println(strlen(StringGiven), DEC);
    Serial.print(F("StringGiven Byte Before Conversion -> "));
    Serial.println(sizeof(StringGiven), DEC);

    Serial.println(F("-----------------------------------------------------"));
    Serial.println(F("Converting from const char* to _FlashStringHelper..."));
    Serial.println(F("-----------------------------------------------------"));
    Serial.print(F("Size (In Bytes) of StringGiven That is !(TypeCasted) - > "));
    Serial.println(sizeof(StringGiven));
    Serial.println(F("-----------------------------------------------------"));
    Serial.print(F("Size (In Bytes) of StringGiven That is (TypeCasted) - > "));
    Serial.println(sizeof((const __FlashStringHelper *)StringGiven));
    Serial.println(F("-----------------------------------------------------"));
    Serial.println(F("Converted to _FlashStringHelper by TypeCast.\n\n Print StringGiven To Check Data..."));
    Serial.println(F("-----------------------------------------------------"));
    Serial.println(StringGiven);
    Serial.println(F("-----------------------------------------------------"));
    Serial.print(F("StringGiven Length After ->"));
    Serial.println(strlen(StringGiven));
    Serial.print(F("StringGiven Byte After ->"));
    Serial.println(sizeof((const __FlashStringHelper *)StringGiven));
    Serial.print(F("StringHelper Function END"));

    if (TypeGiven == "Bytes")
    {
        LCD_I2C.setCursor(PosX, PosY);
        LCD_I2C.print(F(StringGiven));
        return sizeof((const __FlashStringHelper *)StringGiven);
    }
    else
    {
        LCD_I2C.setCursor(PosX, PosY);
        LCD_I2C.print(F(StringGiven));
        return strlen(StringGiven);
    }
}

// Function is not returning as Short because the LCD_SetScroll is already on Global, Since we are printing strings, we should return the function back from what we received.
//static String StringLength_Process(const char *String)
//    LCD_SetScrollX += strlen(String); // This was an absolute length value, no need to change of //something else.
//    LCD_I2C.print(F(String));
//}

// Another Variant, This one uses F macro for Strings. This was an alternative and intended for strings only. The regular function is usable both for string without F macro and any other form of data types.
