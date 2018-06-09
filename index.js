var config = require('./config.json')
var omxp = require('omxplayer-controll');
var gpio = require('rpi-gpio');
var SerialPort = require('serialport');

var port = new SerialPort('/dev/ttyACM0', { baudRate: 2400, autoOpen: true });
var time = 0;

const parsers = SerialPort.parsers;

const parser = new parsers.Readline({
  delimiter: '\n'
});

port.pipe(parser);

parser.on('data', function (data) {
    console.log('Received Data : ' + data);
});


// Open errors will be emitted as an error event
port.on('error', function(err) {
  if (err) {
	console.log('Error: ', err.message);
  }
})

omxp.on('changeStatus',function(status){

    //console.log('Status',status);
    time = Math.floor(status.pos/1000000);	
    console.log(time);

        if (time in config.timeSteps){      

            //console.log(config.timeSteps[time]);

            Object.keys(config.timeSteps[time])
            .forEach(function(currentValue, index, arr){
                
                var a = config.timeSteps[time][currentValue].split(':');

                if (a[0] == 'LED'){                
                    var RGB         = a[4].split(',');             
                    var LED_Range   = a[2].split('-');

                    // console.log("Starting Strip-" +a[1]+
                    // " with LED Range "+LED_Range[0]+"-"+LED_Range[1]+
                    // " and Brightness=" +a[3]+
                    // " and RGB values r=" +RGB[0] +" g=" +RGB[1]+ " b=" +RGB[2]+
                    // " for " + (currentValue-time)) + " seconds!";                       
                      
                    var data1 = a[1]+":"+
                                ('000' + LED_Range[0]).substr(-3)+"-"+
                                ('000' + LED_Range[1]).substr(-3)+":"+
                                ('000' + a[3]).substr(-3)+":"+
                                ('000' + RGB[0]).substr(-3)+","+
                                ('000' + RGB[1]).substr(-3)+","+
                                ('000' + RGB[2]).substr(-3);

                    for(var i=0; i<data1.length; i++){
                        port.write(new Buffer(data1[i], 'ascii'), function(err, results) {
                            if (err) console.log('Error: ' + err);
                            //console.log('Results ' + results);
                        });
                    }
                    
                    // Sending the terminate character
                    port.write(new Buffer('\n', 'ascii'), function(err, results) {
                        if (err) console.log('err ' + err);
                        //console.log('results ' + results);
                    });
                    
                    console.log("CONSOLE-LOG:" + data1);

                    setTimeout(function(){                      
    
                       	var data2 = (parseInt(a[1])+2).toString()+":"+
                                        ('000' + LED_Range[0]).substr(-3)+"-"+
                                        ('000' + LED_Range[1]).substr(-3)+":"+
                                        ('000' + a[3]).substr(-3)+":"+
                                        ('000' + RGB[0]).substr(-3)+","+
                                        ('000' + RGB[1]).substr(-3)+","+
                                        ('000' + RGB[2]).substr(-3);
                            

                        for(var i=0; i<data2.length; i++){
                            port.write(new Buffer(data2[i], 'ascii'), function(err, results) {
                                if (err) console.log('Error: ' + err);
                                //console.log('Results ' + results);
                            });
                        }
                        
                        // Sending the terminate character
                        port.write(new Buffer('\n', 'ascii'), function(err, results) {
                            if (err) console.log('err ' + err);
                            //console.log('results ' + results);
                        });                           

                        console.log("CONSOLE-LOG:" + data2);                        
                    }, (currentValue-time)*1000);    
                }            
                else{
                    console.log("STARTING "+ config.timeSteps[time][currentValue] + " for " + (currentValue-time));
                    start_Relay(config.timeSteps[time][currentValue], currentValue-time);
                }
            })
        }   
    
});





Object.keys(config.Relays)
        .forEach(function(currentValue, index, arr){
            gpio.setup(parseInt(config.Relays[currentValue]),    gpio.DIR_HIGH, done(parseInt(config.Relays[currentValue])));
        })

        
function done(pin){
    console.log("Pin:"+pin+" Initialized!");
}

function stop_Relay(relay) {

    gpio.write(relay, true, function(err) {
         if (err) throw err;
         console.log('Relay-'+ relay + ' Turned OFF');
    });

}

function start_Relay(value, time) {

    var relay = config.Relays[value];

    gpio.write(relay, false, function(err) {
         if (err) throw err;
         console.log('Relay-'+ relay + ' Turned ON');
    });
    
    setTimeout(function(){ stop_Relay(relay); }, time*1000); //RELAY OFF after 'time' seconds
}    

var opts = {
    'audioOutput': 'hdmi', //  'hdmi' | 'local' | 'both'
    'blackBackground': false, //false | true | default: true
    'disableKeys': true, //false | true | default: false
    'disableOnScreenDisplay': true, //false | true | default: false
    'disableGhostbox': true, //false | true | default: false
    'subtitlePath': '', //default: ""
    'startAt': 0, //default: 0
    'startVolume': 0.8 //0.0 ... 1.0 default: 1.0
};





function theLAST(){
    process.nextTick(function () { process.exit(0); });
}

var signals = {
    'SIGINT': 2,
    'SIGTERM': 15
  };
  
function shutdown(signal, value) {
    console.log('Stopped by ' + signal);
    
    gpio.destroy(function() {
          console.log('All pins unexported');
      }); 
    setTimeout(theLAST, 2000);   
}
  
Object.keys(signals).forEach(function (signal) {
    process.on(signal, function () {
      shutdown(signal, signals[signal]);
    });
});

omxp.on('aboutToFinish',function(){
    console.log('Movie about to finish');
});

omxp.on('finish', function() {
    console.log('============= Finished =============');
    //omxp.open(config.Video, opts);
});

omxp.open(config.Video, opts);
console.log('Program started. Press <ctrl>+C to exit.');



