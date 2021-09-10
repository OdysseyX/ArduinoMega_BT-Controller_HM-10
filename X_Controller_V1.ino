//========================Library======================//
#include <SoftwareSerial.h>
#include <ArduinoBlue.h>
#include <Servo.h>
//=====================================================//

//========================Int Setup=====================//
Servo motor;
Servo wheel;
// Bluetooth TX -> Arduino D2
const int BLUETOOTH_TX = 51;
// Bluetooth RX -> Arduino D3
const int BLUETOOTH_RX = 50;
// 
int throttle, steering, sliderVal, button, sliderId;
int wheel_left, wheel_right, wheel_center;// Steering Servo variables
int forwards_fast, forwards_medium, forwards_slow, backwards_fast, backwards_medium, backwards_slow, throttle_off; // Motor Servo Varables

//=====================================================//

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);
ArduinoBlue phone(bluetooth); // pass reference of bluetooth object to ArduinoBlue constructor.

// Setup code runs once after program starts.
void setup() {
    // Servo/Motor Setup
    motor.attach(30);
    wheel.attach(40);
    motor.write(90); // Activating Motor
    motor.write(91); // Activating Motor
    // Start serial monitor at 9600 bps.
    Serial.begin(9600);

    // Start bluetooth serial at 9600 bps.
    bluetooth.begin(9600);

    // delay just in case bluetooth module needs time to "get ready".
    delay(100);

    //==============Motor Activation Sequence==============//
    motor.write(90);delay(50);
    motor.write(91);delay(50);
    motor.write(90);delay(50);
    motor.write(91);delay(50);
    motor.write(90);delay(50);
    motor.write(91);delay(50);
    motor.write(90);delay(50);
    motor.write(91);delay(50);
    //=====================================================//

    // default//
    wheel_left = 140;
    wheel_right = 50;
    wheel_center = 88;
    forwards_fast = 300;
    forwards_medium = 105;
    forwards_slow = 98;
    backwards_fast = 30;
    backwards_medium = 50;
    backwards_slow = 70;
    throttle_off = 90;

    
    Serial.println("setup complete");
}

// Put your main code here, to run repeatedly:
void loop() {
    //========================Motor Arming======================//
    // Just incase motor still hasn't activated
    motor.write(91);delay(50); //motor arming
    motor.write(90);delay(50); //motor arming
    //==========================================================//
    
    
    //========================Data Setup======================//
    // ID of the button pressed pressed.
    button = phone.getButton();
    // Returns the text data sent from the phone.
    // After it returns the latest data, empty string "" is sent in subsequent.
    // calls until text data is sent again.
    String str = phone.getText();
    //==========================================================//
    
    
    //===========================Simple Control===============================//
    // Display button data whenever its pressed if connected.
    // Convert button ID to command for arduino to follow  
    if (button != -1) 
    {
        Serial.print("Button: ");
        Serial.println(button);
        if (button == 1)// Drive Forwards
        {
          Serial.print("Drive Forwards:");
          motor.write(forwards_slow);wheel.write(wheel_center); delay(2000);
        }
        else if (button == 2)// Drive Backwards
        {
          Serial.print("Drive Backwards:");
          motor.write(backwards_slow);wheel.write(wheel_center); delay(2000);
        }
        else if (button == 3)// Turn Left 
        {
          Serial.print("Turn Left:");
          motor.write(forwards_slow);wheel.write(wheel_left); delay(2000);
        }
        else if (button == 4)// Turn Right
        {
          Serial.print("Turn Right:");
          motor.write(forwards_slow);wheel.write(wheel_right); delay(2000);
        }
        else
        {
          Serial.print("Unkown Command");
          Serial.print("//==========================================================//");
        }
    }

    // If a text from the phone was sent print it to the serial monitor
    if (str != "") 
    {
        Serial.println(str);
    }

    // Send string from serial command line to the phone. This will alert the user.
    if (Serial.available()) {
        Serial.write("send: ");
        String str = Serial.readString();
        phone.sendMessage(str); // phone.sendMessage(str) sends the text to the phone.
        Serial.print(str);
        Serial.write('\n');
    }
}
