import time
import serial;
from threading import Thread
from flask import Flask, request, abort, jsonify

class Led:
    def __init__(self, index, red, green, blue):
        self.index = index
        self.red = red
        self.green = green
        self.blue = blue

class Strip:
    def __init__(self, leds = []):
        self.leds = leds

ser = serial.Serial(
    'COM4',
    baudrate=9600
)
time.sleep(1)
ser.setDTR(False)
time.sleep(1)

def addToPayload(led, payload = []):
    payload.append(led.index)
    payload.append(led.red)
    payload.append(led.green)
    payload.append(led.blue)

def sendPayload(ser, payload = []):
    print('Sending payload ' + str(payload))
    ser.write(bytes(payload))
    time.sleep(.1)

def updateStrip(ser, strip):
    payload = []
    for led in strip.leds:
        addToPayload(led, payload)
        sendPayload(ser, payload)
        payload = []

def updateLed(ser, led):
    payload = []
    addToPayload(led, payload)
    sendPayload(ser, payload)

strip = Strip(leds = [
    Led(0, 255, 0, 0),
    Led(1, 0, 255, 0),
    Led(2, 0, 0, 255),
    Led(3, 255, 255, 0),
    Led(4, 255, 0, 255),
    Led(5, 255, 0, 255),
    Led(6, 255, 255, 255),
    Led(7, 0, 0, 0)
])

updateStrip(ser, strip)

app = Flask(__name__)

@app.route('/')
def route_index():
    return app.send_static_file("index.html")

@app.route('/update-led', methods=['POST'])
def route_update_led():
    if not request.json or not 'idx' in request.json:
        abort(400)
    led = Led(
        request.json['idx'],
        request.json['r'],
        request.json['g'],
        request.json['b']
    )
    updateLed(ser, led)
    return jsonify({'success': True})
    
if __name__ == '__main__':
    app.run()