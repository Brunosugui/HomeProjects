# First Arduino Project

* DHT11 sensor (humidity and temperature)
* Sound sensor
* PIR sensor (movement and presence)
* LDR application (Light Dependent Resistor)
* serial comunication with a ESP32 module

## Project application:

### on Arduino:
        * clapper (two claps between 300 and 700ms) trigger DHT read data and send via Serial to pc
        * PIR sensor trigger blueLed or redLed according to its digital output
        * LDR used as voltage divisor ground resistor, measured its output voltage via analog pin and turn on/off a yellow led according to its value
        * Serial with ESP32 module, listening to incoming data, and parse it

### on ESP32:
        * connect to wifi
        * connect to mqtt broker server
        * Serial with Arduino Uno
        * listening to a topic (mqtt protocol) and parse its data. Send command to Arduino, such as temperature or humidity
        * Arduino receives the comand and send the sensor reading back to ESP32
        * When data is received from arduino, publish it on specific topics on broker server
