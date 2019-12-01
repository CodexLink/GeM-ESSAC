// ! Experimental Prototyping |> PWM LED Test
// ! By Janrey Licas
// Created on: 12/01/2019, <Untracked Time>

/*  * @info => Tests 3 LEDs attached as indicator of data processing.
    * @includes => Literal 3 LEDs.
    * @extras => PWM
    ! |> Why?
    *   - I just want that "transistion" fade in / out after outputting indications of data processing.

    ! Note:
    *   - There are some sequences that is commented out. More to come I guess but I'll keep things simple
    *   so that's what I can provide for myself right now.
*/

#define GREEN_LED 10
#define YELLOW_LED 11
#define RED_LED 12

// ! Potential Macro Definitions for the Final Sketch Design
#define ON_STATE(LED_PIN) digitalWrite(LED_PIN, HIGH)
#define OFF_STATE(LED_PIN) digitalWrite(LED_PIN, LOW)
#define PWM_STATE(LED_PIN, BRIGHTNESS) analogWrite(LED_PIN, BRIGHTNESS)
void init_LEDs();

void setup()
{
    Serial.begin(115200);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    init_LEDs();
}

void loop()
{
    // * Tuning EACH UP
    for (int i = 10; i < 13; i++)
    {
        for (int a = 0; a <= 255; a++)
        {
            Serial.print(i);
            Serial.print(" | ");
            Serial.println(a);
            PWM_STATE(i, a);
            delay(2);
        }
        //delay(300);
    }
    // * Tuning EACH DOWN
    for (int i = 10; i < 13; i++)
    {
        for (int a = 255; a >= 0; a--)
        {
            Serial.print(i);
            Serial.print(" | ");
            Serial.println(a);
            PWM_STATE(i, a);
            delay(2);
        }
        //delay(1500);
    }
}

// ! One of these sequence will not be included upon POST of the device in the final sketch design.
// ! Other sequences were just added here just to make things look like we're troubleshooting the LEDs itself.
// ! You can uncomment other parts of the LED test.
void init_LEDs()
{
    // Sequence #1 - On ALL
    Serial.println("Sequence #1 | BEGIN");
    ON_STATE(GREEN_LED);
    ON_STATE(YELLOW_LED);
    ON_STATE(RED_LED);
    delay(1000);
    // Sequence #1 - Finish
    Serial.println("Sequence #1 | END");
    Serial.println();
    OFF_STATE(GREEN_LED);
    OFF_STATE(YELLOW_LED);
    OFF_STATE(RED_LED);

    delay(1000);
    /*
    // Sequence #2 - Open Once @ 2000ms
    Serial.println("Sequence #2 | BEGIN");
    ON_STATE(GREEN_LED);
    delay(2000);
    ON_STATE(YELLOW_LED);
    delay(2000);
    ON_STATE(RED_LED);
    delay(2000);

    // Sequence #2 - Finish
    Serial.println("Sequence #2 | END");
    Serial.println();
    OFF_STATE(GREEN_LED);
    OFF_STATE(YELLOW_LED);
    OFF_STATE(RED_LED);

    // Sequence #3 - BLINK ALL AT ONCE @ 2000ms
    Serial.println("Sequence #3 | START");
    for (auto i = 0; i <= 5; i++)
    {
        Serial.print("Sequence #3 | @ ");
        Serial.println(i);
        ON_STATE(GREEN_LED);
        ON_STATE(YELLOW_LED);
        ON_STATE(RED_LED);
        delay(2000);
        OFF_STATE(GREEN_LED);
        OFF_STATE(YELLOW_LED);
        OFF_STATE(RED_LED);
        delay(2000);
    }

    // Sequence #3 - Finish (We ensure that all LEDS are off.)
    Serial.println("Sequence #3 | END");
    Serial.println();
    OFF_STATE(GREEN_LED);
    OFF_STATE(YELLOW_LED);
    OFF_STATE(RED_LED);

    while (1)
    {
        Serial.println("Done.");
        delay(10000);
    }
    */
}