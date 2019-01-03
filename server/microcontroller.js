const SerialPort = require('serialport');

const DELAY_WRITE = 16;

const serialPort = new SerialPort('COM4', {
    baudRate: 9600
});

const writeBuffer = [];
var initialized = false;

function run() {
    if (writeBuffer.length > 0) {
        var byte = writeBuffer.shift();
        var ok = serialPort.write(Buffer.from([byte]), 'hex');
        if (ok) {
            setTimeout(run, DELAY_WRITE)
        } else {
            writeBuffer.on('drain', run)
        }
    } else {
        setTimeout(run, DELAY_WRITE)
    }
}

module.exports = {
    initialize: function(cb) {
        serialPort.on('open', () => {
            serialPort.on('data', (data) => {
                if (!initialized && data.readInt8() === 42) {
                    initialized = true;
                    run();
                    cb();
                } else {
                    console.log('received data: ' + Uint8Array.from(data))
                }
            })
        })
    },
    writeBytes: function(bytes) {
        writeBuffer.push.apply(writeBuffer, bytes);
    }
}