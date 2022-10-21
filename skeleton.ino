#include <Arduino.h>
#include <Wire.h>
#include <Notecard.h>

#define serialDebug Serial

const int buttonPin = 6; // the digital pin of the push button
int buttonState = 0;     // variable for reading button status

Notecard notecard;

void setup()
{
    serialDebug.begin(115200);
    while (!serialDebug)
    {
        ;
    }

    // initialize digital pin LED_BUILTIN as an output
    pinMode(LED_BUILTIN, OUTPUT);
    // initialize the push button pin as an input
    pinMode(buttonPin, INPUT);

    notecard.setDebugOutputStream(serialDebug);
    Wire.begin();
    notecard.begin();

    J *req = NoteNewRequest("hub.set");
    JAddStringToObject(req, "mode", "continuous");
    JAddStringToObject(req, "product", "<your-product-uid>");
    JAddBoolToObject(req, "sync", true);
    notecard.sendRequest(req);

    req = NoteNewRequest("hub.sync");
    notecard.sendRequest(req);
}

// the loop function runs over and over again forever
void loop()
{
    // read the state of the push button value:
    buttonState = digitalRead(buttonPin);

    // check if the push button is pressed
    if (buttonState == HIGH)
    {
        // turn LED on:
        digitalWrite(LED_BUILTIN, HIGH);

        J *req = NoteNewRequest("note.add");
        JAddStringToObject(req, "file", "surprise.qo");
        JAddBoolToObject(req, "sync", true);
        J *body = JCreateObject();
        JAddBoolToObject(body, "jump", true);
        JAddItemToObject(req, "body", body);
        notecard.sendRequest(req);

        delay(3000);
    }
    else
    {
        // turn LED off:
        digitalWrite(LED_BUILTIN, LOW);
    }
}