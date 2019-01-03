var express = require('express');
var bodyParser = require('body-parser');
var ledController = require('./ledController');

ledController.initialize(() => {
    console.log('initialized')
})

var app = express();
app.use(bodyParser.json())

app.post('/updateLed', function(req, res) {
    const leds = []
    if (req.body.length) {
        req.body.forEach(led => {
            leds.push({
                idx: Number(led.idx),
                r: Number(led.r),
                g: Number(led.g),
                b: Number(led.b)
            })
        })
    }
    else {
        leds.push({
            idx: Number(req.body.idx),
            r: Number(req.body.r),
            g: Number(req.body.g),
            b: Number(req.body.b)
        })
    }
    leds.forEach(led => ledController.updateLed(led.idx, led.r, led.g, led.b))
    res.send(200);
})

app.post('/setColor', function(req, res) {
    ledController.setColor(req.body.r, req.body.g, req.body.b)
    res.send(200);
})

app.post('/setRainbow', function(req, res) {
    ledController.setRainbow(req.body.r, req.body.g, req.body.b)
    res.send(200);
})

app.post('/nextPattern', function(req, res) {
    ledController.nextPattern()
    res.send(200);
})

app.use('/', express.static('static'))

module.exports = app;