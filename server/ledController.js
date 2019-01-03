var microcontroller = require('./microcontroller');

const ledController = {
    initialize: function(cb) {
        microcontroller.initialize(cb);
    },
    updateLed: function(idx, r, g, b) {
        microcontroller.writeBytes([
            1, // command byte
            idx, // index byte
            0,
            r,
            g,
            b
        ])
    },
    setColor: function(r, g, b) {
        microcontroller.writeBytes([
            2,
            0,
            0,
            r,
            g,
            b
        ])
    },
    setRainbow: function(r, g, b) {
        microcontroller.writeBytes([
            3,
            0,
            0,
            r,
            g,
            b
        ])
    },
    nextPattern: function() {
        microcontroller.writeBytes([
            4, 0, 0, 0, 0, 0
        ])
    }
};

module.exports = ledController