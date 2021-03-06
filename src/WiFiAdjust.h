
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Hash.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <LITTLEFS.h>
#include <DNSServer.h>
// #include <WiFiManager.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>
#endif

#include <ESPAsyncWiFiManager.h> //https://github.com/tzapu/WiFiManager

AsyncWebServer server(80);
DNSServer dns;

// Create an Event Source on /events
AsyncEventSource events("/events");

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ssid = "Beeline_2G_F22959";
const char *password = "dUNFcUEP";

//const char* ssid     = "Mironov AG";
//const char* password = "12345678";
String GeneralString;
const char *MyFile = {"/MyFile.txt"};
String TemporaryFile1;
String TemporaryFile2;
bool InitFlag = 0;
const char *parametr[leng] = {
    "HOUR", "MIN", "TEMP", "LEAK", "LIGHT",
    "BOBBER1", "BOBBER2", "BOBBER3", "BOBBER4",
    "SWITCH1", "SWITCH2", "SWITCH3", "SWITCH4",
    "OBTACLE1", "OBTACLE2",

    // from 15
    "STEP1_STATE", "STEP2_STATE", "STEP3_STATE", "STEP4_STATE", // where I am
    "SOLI", "LASER1", "LASER2",
    "VALVE1", "VALVE2", "VALVE3", "VALVE4", "VALVE5", "VALVE6",
    "PUMP1", "PUMP2", "PUMP3", "PUMP4", "BIG_PUMP",
    "DEVICE1", "DEVICE2", "DEVICE3", "DEVICE4", "DEVICE5", "DEVICE6", "DEVICE7", "DEVICE8",
    "DRIVE1", "DRIVE2", "DRIVE3", "DRIVE4", // Device 1 is LED

    // from 45
    "STEP3_hour_on", "STEP3_min_on", "STEP3_hour_off", "STEP3_min_off",
    "STEP4_hour_on", "STEP4_min_on", "STEP4_hour_off", "STEP4_min_off",

    // from 53
    "SOLI_hour_on", "SOLI_min_on", "SOLI_hour_off", "SOLI_min_off",
    "LASER1_hour_on", "LASER1_min_on", "LASER1_hour_off", "LASER1_min_off",
    "LASER2_hour_on", "LASER2_min_on", "LASER2_hour_off", "LASER2_min_off",
    "VALVE1_hour_on", "VALVE1_min_on", "VALVE1_hour_off", "VALVE1_min_off",
    "VALVE2_hour_on", "VALVE2_min_on", "VALVE2_hour_off", "VALVE2_min_off",
    "VALVE3_hour_on", "VALVE3_min_on", "VALVE3_hour_off", "VALVE3_min_off",
    "VALVE4_hour_on", "VALVE4_min_on", "VALVE4_hour_off", "VALVE4_min_off",
    "VALVE5_hour_on", "VALVE5_min_on", "VALVE5_hour_off", "VALVE5_min_off",
    "VALVE6_hour_on", "VALVE6_min_on", "VALVE6_hour_off", "VALVE6_min_off",
    // from 89
    "PUMP1_hour_on", "PUMP1_min_on", "PUMP1_hour_off", "PUMP1_min_off",
    "PUMP2_hour_on", "PUMP2_min_on", "PUMP2_hour_off", "PUMP2_min_off",
    "PUMP3_hour_on", "PUMP3_min_on", "PUMP3_hour_off", "PUMP3_min_off",
    "PUMP4_hour_on", "PUMP4_min_on", "PUMP4_hour_off", "PUMP4_min_off",
    "BIG_PUMP_hour_on", "BIG_PUMP_min_on", "BIG_PUMP_hour_off", "BIG_PUMP_min_off",
    // from 109
    "DEVICE1_hour_on", "DEVICE1_min_on", "DEVICE1_hour_off", "DEVICE1_min_off",
    "DEVICE2_hour_on", "DEVICE2_min_on", "DEVICE2_hour_off", "DEVICE2_min_off",
    "DEVICE3_hour_on", "DEVICE3_min_on", "DEVICE3_hour_off", "DEVICE3_min_off",
    "DEVICE4_hour_on", "DEVICE4_min_on", "DEVICE4_hour_off", "DEVICE4_min_off",
    "DEVICE5_hour_on", "DEVICE5_min_on", "DEVICE5_hour_off", "DEVICE5_min_off",
    "DEVICE6_hour_on", "DEVICE6_min_on", "DEVICE6_hour_off", "DEVICE6_min_off",
    "DEVICE7_hour_on", "DEVICE7_min_on", "DEVICE7_hour_off", "DEVICE7_min_off",
    "DEVICE8_hour_on", "DEVICE8_min_on", "DEVICE8_hour_off", "DEVICE8_min_off",
    "DRIVE1_hour_on", "DRIVE1_min_on", "DRIVE1_hour_off", "DRIVE1_min_off",
    "DRIVE2_hour_on", "DRIVE2_min_on", "DRIVE2_hour_off", "DRIVE2_min_off",
    "DRIVE3_hour_on", "DRIVE3_min_on", "DRIVE3_hour_off", "DRIVE3_min_off",
    "DRIVE4_hour_on", "DRIVE4_min_on", "DRIVE4_hour_off", "DRIVE4_min_off",
    //from 157
    "STEP1_hour_on", "STEP1_min_on", "STEP1_hour_off", "STEP1_min_off",

    // from 161
    "STEP2_hour_on", "STEP2_min_on", "STEP2_hour_off", "STEP2_min_off",

    "StartMonth", "StartDay", "StartHour", "StartMin",
    "watering", "irrigation", "begin", "cycle",
    "TankFlag", "CycleDay", "CycleHour",
    //from 177
    "IrrigationMix", "WateringMix",

    "STEP1_value", "STEP2_value", "STEP3_value", "STEP4_value", "key"}; // corner to turn

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>ESP Input Form</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script>
        function submitMessage() {
            alert("Saved value to ESP LittleFS");
            setTimeout(function () { document.location.reload(false); }, 500);
        }
    </script>

    <style>
        html {
            font-family: Arial;
            display: inline-block;
            text-align: center;
        }

        body {
            margin: 0;
        }

        .content {
            padding: 20px;
        }

        input[type=text] {
            width: 30px;
            margin: 8px 0px;
            border: 2px solid #808080;
            background-color: #C0C0C0;
        }

        input[type=submit] {
            width: 60px;
            border: 2px solid #808080;
            background-color: #A1DAD9;
        }

        .card {
            padding: 10px;
            background-color: white;
            box-shadow: 2px 2px 12px 1px rgba(140, 140, 140, .5);
        }

        p {
            color: navy;
            text-indent: 10px;
            margin-top: 0em;
            margin-bottom: 0em;
        }

        .cards {
            max-width: 800;
            margin: 0 auto;
            display: grid;
            grid-gap: 2rem;
            grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
        }
    </style>

</head>

<body>
    <div class="cards">
        <div class="card">
            <form action="/get" target="hidden-form">
                <p>Current time <span id="HOUR"> %HOUR%</span> : <span id="MIN">%MIN%</span> </p>
            </form><br>

            <form action="/get" target="hidden-form">
                <p>Temperature <span id="TEMP">%TEMP%</span> &deg;C</span> </p>
            </form><br>

            <form action="/get" target="hidden-form">
                <p> Water leaks <span id="LEAK"> %LEAK% </span> </p>
            </form><br>

            <form action="/get" target="hidden-form">
                <p>Light sensor <span id="LIGHT">%LIGHT% </span> </p>
            </form><br>

            <form action="/get" target="hidden-form">
                <p>Bobber_Tank_min_1 <span id="BOBBER1"> %BOBBER1% </span> </p>
                <p>Bobber_Tank_max_2 <span id="BOBBER2">%BOBBER2% </span> </p>
                <p>Bobber_3 <span id="BOBBER3">%BOBBER3% </span> </p>
                <p>Bobber_4 <span id="BOBBER4">%BOBBER4% </span></p>
            </form><br>

            <form action="/get" target="hidden-form">
                <p>Limit switch_1 <span id="SWITCH1"> %SWITCH1% </span> </p>
                <p>Limit switch_2 <span id="SWITCH2"> %SWITCH2% </span> </p>
                <p>Limit switch_3 <span id="SWITCH3">%SWITCH3% </span> </p>
                <p>Limit switch_4 <span id="SWITCH4">%SWITCH4%</span></p>
            </form><br>

            <form action="/get" target="hidden-form">
                <p>Obtacle1 <span id="OBTACLE1"> %OBTACLE1% </span> </p>
                <p>Obtacle2 <span id="OBTACLE2">%OBTACLE2% </span> </p>
            </form><br>

        </div>


            <div class="card"style="box-shadow: none">
            <div class="cards">
            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p> Start at <span id="StartDay"> %StartDay%</span>.<span id="StartMonth">%StartMonth% </span> 	&nbsp;/&nbsp; <span id="StartHour"> %StartHour%</span> : <span id="StartMin">%StartMin% </span></p>
                    <p><input type="datetime-local" name="StartMonth"></p>
                    <p>Cycle <span id="cycle">%cycle%</span> <input type="text" name="cycle"> hour</p>
                    <p>Angle of rotation <span id="STEP1_value">%STEP1_value%</span> <input type="text" name="STEP1_value"></p>
                    <p>Irrigation mix duration <span id="IrrigationMix"> %IrrigationMix% </span> <input type="text" name="IrrigationMix"> min</p>
                    <p>Irrigation duration<span id="irrigation"> %irrigation% </span> <input type="text" name="irrigation"> min</p>
                    <p>Watering mix duration <span id="WateringMix"> %WateringMix% </span>  <input type="text" name="WateringMix"> min</p>
                    <p>Watering duration <span id="watering"> %watering% </span>  <input type="text" name="watering"> min</p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>
            

            <div class="card">
            <p>Led control <span id="STEP2_STATE"> %STEP2_STATE% </span> </p>
                <form id="red-form" action="/get" target="hidden-form">
                    <p> distance <span id="STEP2_value">%STEP2_value%</span> <input type="text" name="STEP2_value">
                    <p> time on<span id="STEP2_hour_on"> %STEP2_hour_on% </span>:<span id="STEP2_min_on">
                            %STEP2_min_on% </span> <input type="time" name="STEP2_hour_on">
                     <p> time off<span id="STEP2_hour_off"> %STEP2_hour_off% </span>:<span id="STEP2_min_off">
                            %STEP2_min_off% </span>  <input type="time" name="STEP2_hour_off">
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
           </div>
    </div>
  </div>
  
            

<div class="card"style="box-shadow: none">
            <div class="cards">
            <div class="card">
            <form id="red-form" action="/get" target="hidden-form">
                <p>STEP1 state <span id="STEP1_STATE"> %STEP1_STATE% </span> </p>
                <p> corner <span id="STEP1_value">%STEP1_value% </span> <input type="text" name="STEP1_value"></p>
                <p> time  <span id="STEP1_hour_on">%STEP1_hour_on% </span> : <span id="STEP1_min_on">
                        %STEP1_min_on% </span> <input type="time" name="STEP1_hour_on"></p>
                <p> time  <span id="STEP1_hour_off">%STEP1_hour_off%</span> : <span
                        id="STEP1_min_off">%STEP1_min_off% </span><input type="time" name="STEP1_hour_off"></p>
                <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
            </form><br>
        </div>
        <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>STEP2 state <span id="STEP2_STATE"> %STEP2_STATE% </span> </p>
                    <p> corner <span id="STEP2_value">%STEP2_value%</span>  <input type="text" name="STEP2_value"></p>
                    <p> time  <span id="STEP2_hour_on"> %STEP2_hour_on% </span> : <span id="STEP2_min_on">
                            %STEP2_min_on% </span><input type="time" name="STEP2_hour_on"></p>
                    <p>time  <span id="STEP2_hour_off">%STEP2_hour_off% </span> : <span
                            id="STEP2_min_off">%STEP2_min_off%</span> <input type="time" name="STEP2_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>
    </div>
  </div>
  



<div class="card"style="box-shadow: none">
            <div class="cards">
            <div class="card">
            <form id="red-form" action="/get" target="hidden-form">
                <p>STEP3 state <span id="STEP3_STATE"> %STEP3_STATE% </span> </p>
                <p> corner <span id="STEP3_value">%STEP3_value% </span> <input type="text" name="STEP3_value"></p>
                <p> time  <span id="STEP3_hour_on">%STEP3_hour_on% </span> : <span
                        id="STEP3_min_on">%STEP3_min_on%</span> <input type="time" name="STEP3_hour_on"></p>
                <p> time  <span id="STEP3_hour_off">%STEP3_hour_off%</span> : <span
                        id="STEP3_min_off">%STEP3_min_off% </span><input type="time" name="STEP3_hour_off"></p>
                <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
            </form><br>
        </div>
        <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>STEP4 state <span id="STEP4_STATE"> %STEP4_STATE% </span> </p>
                    <p> corner <span id="STEP4_value">%STEP4_value%</span>  <input type="text" name="STEP4_value"></p>
                    <p> time  <span id="STEP4_hour_on"> %STEP4_hour_on% </span> : <span id="STEP4_min_on">
                            %STEP4_min_on% </span><input type="time" name="STEP4_hour_on"></p>
                    <p>time  <span id="STEP4_hour_off">%STEP4_hour_off% </span> : <span
                            id="STEP4_min_off">%STEP4_min_off%</span> <input type="time" name="STEP4_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>
    </div>
  </div>
  </div>
</div>
    <div class="content">
        <div class="cards">


            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>Solinoid state <span id="SOLI"> %SOLI% </span> </p>
                    <p> time on <span id="SOLI_hour_on">%SOLI_hour_on%</span> :<span
                            id="SOLI_min_on">%SOLI_min_on%</span> <input type="time" name="SOLI_hour_on"></p>
                    <p> time off <span id="SOLI_hour_off">%SOLI_hour_off%</span> : <span
                            id="SOLI_min_off">%SOLI_min_off%</span> <input type="time" name="SOLI_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"></p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>Laser 1 state <span id="LASER1"> %LASER1%</span> </p>
                    <p> time on <span id="LASER1_hour_on">%LASER1_hour_on%</span> : <span
                            id="LASER1_min_on">%LASER1_min_on%</span> <input type="time" name="LASER1_hour_on">
                    <p>time off <span id="LASER1_hour_off">%LASER1_hour_off% </span>: <span
                            id="LASER1_min_off">%LASER1_min_off%</span> <input type="time" name="LASER1_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>Laser 2 state <span id="LASER2">%LASER2% </span> </p>
                    <p> time on<span id="LASER2_hour_on">%LASER2_hour_on%</span> :<span
                            id="LASER2_min_on">%LASER2_min_on% </span> <input type="time" name="LASER2_hour_on">
                    <p>time off <span id="LASER2_hour_off">%LASER2_hour_off%</span> :<span
                            id="LASER2_min_off">%LASER2_min_off%</span> <input type="time" name="LASER2_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>VALVE1 state <span id="VALVE1">%VALVE1% </span> </p>
                    <p> time on <span id="VALVE1_hour_on">%VALVE1_hour_on%</span> :<span
                            id="VALVE1_min_on">%VALVE1_min_on%</span> <input type="time" name="VALVE1_hour_on">
                    <p>time off <span id="VALVE1_hour_off">%VALVE1_hour_off%</span> : <span
                            id="VALVE1_min_off">%VALVE1_min_off%</span> <input type="time" name="VALVE1_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>VALVE2 state <span id="VALVE2"> %VALVE2% </span> </p>
                    <p> time on<span id="VALVE2_hour_on">%VALVE2_hour_on% </span> : <span
                            id="VALVE2_min_on">%VALVE2_min_on% </span><input type="time" name="VALVE2_hour_on">
                    <p>time off <span id="VALVE2_hour_off">%VALVE2_hour_off% </span>: <span
                            id="VALVE2_min_off">%VALVE2_min_off%</span> <input type="time" name="VALVE2_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>VALVE3 state <span id="VALVE3">%VALVE3%</span> </p>
                    <p> time on <span id="VALVE3_hour_on">%VALVE3_hour_on%</span> : <span
                            id="VALVE3_min_on">%VALVE3_min_on% </span><input type="time" name="VALVE3_hour_on">
                    <p>time off<span id="VALVE3_hour_off"> %VALVE3_hour_off% </span>: <span
                            id="VALVE3_min_off">%VALVE3_min_off%</span> <input type="time" name="VALVE3_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>VALVE4 state <span id="VALVE4"> %VALVE4% </span> </p>
                    <p> time on <span id="VALVE4_hour_on">%VALVE4_hour_on% </span>: <span
                            id="VALVE4_min_on">%VALVE4_min_on%</span> <input type="time" name="VALVE4_hour_on">
                    <p>time off <span id="VALVE4_hour_off">%VALVE4_hour_off%</span> : <span
                            id="VALVE4_min_off">%VALVE4_min_off%</span> <input type="time" name="VALVE4_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>VALVE5 state <span id="VALVE5">%VALVE5% </span> </p>
                    <p> time on <span id="VALVE4_hour_off"></span>%VALVE5_hour_on% :<span id="VALVE4_hour_off"></span> %VALVE5_min_on% <input type="time" name="VALVE5_hour_on">
                    <p>time off <span id="VALVE4_hour_off"></span>%VALVE5_hour_off% : <span id="VALVE4_hour_off"></span>%VALVE5_min_off% <input type="time" name="VALVE5_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>VALVE6 state <span id="VALVE6"> %VALVE6% </span> </p>
                    <p> time on <span id="VALVE6_hour_on">%VALVE6_hour_on% </span>:<span id="VALVE6_min_on"> %VALVE6_min_on% </span><input type="time" name="VALVE6_hour_on">
                    <p>time off <span id="VALVE6_hour_off">%VALVE6_hour_off% </span>:<span id="VALVE6_min_off"> %VALVE6_min_off% </span><input type="time" name="VALVE6_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>PUMP1 state <span id="PUMP1">%PUMP1% </span> </p>
                    <p> time on <span id="PUMP1_hour_on">%PUMP1_hour_on% </span>:<span id="PUMP1_min_on">%PUMP1_min_on% </span> <input type="time" name="PUMP1_hour_on">
                    <p>time off<span id="PUMP1_hour_off"> %PUMP1_hour_off%</span> :<span id="PUMP1_min_off"> %PUMP1_min_off%</span> <input type="time" name="PUMP1_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>PUMP2 state <span id="PUMP2"> %PUMP2% </span> </p>
                    <p> time on <span id="PUMP2_hour_on">%PUMP2_hour_on%</span> : <span id="PUMP2_min_on">%PUMP2_min_on% </span><input type="time" name="PUMP2_hour_on">
                    <p>time off<span id="PUMP2_hour_off"> %PUMP2_hour_off%</span> :<span id="PUMP2_min_off">%PUMP2_min_off%</span>  <input type="time" name="PUMP2_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>PUMP3 state <span id="PUMP3"> %PUMP3% </span> </p>
                    <p> time on <span id="PUMP3_hour_on">%PUMP3_hour_on% </span>:<span id="PUMP3_min_on"> %PUMP3_min_on%</span> <input type="time" name="PUMP3_hour_on">
                    <p>time off<span id="PUMP3_hour_off"> %PUMP3_hour_off% </span>:<span id="PUMP3_min_off"> %PUMP3_min_off%</span> <input type="time" name="PUMP3_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>PUMP4 state <span id="PUMP4"> %PUMP4% </span> </p>
                    <p> time on <span id="PUMP4_hour_on">%PUMP4_hour_on% </span>:<span id="PUMP4_min_on"> %PUMP4_min_on%</span> <input type="time" name="PUMP4_hour_on">
                    <p>time off<span id="PUMP4_hour_off">%PUMP4_hour_off% </span> :<span id="PUMP4_min_off"> %PUMP4_min_off% </span><input type="time" name="PUMP4_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE1 state <span id="DEVICE1"> %DEVICE1% </span> </p>
                    <p> time on <span id="DEVICE1_hour_on">%DEVICE1_hour_on% </span>:<span id="DEVICE1_min_on">%DEVICE1_min_on%</span>  <input type="time" name="DEVICE1_hour_on">
                    <p>time off<span id="DEVICE1_hour_off"> %DEVICE1_hour_off% </span>:<span id="DEVICE1_min_off"> %DEVICE1_min_off%</span> <input type="time" name="DEVICE1_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE2 state <span id="DEVICE2">%DEVICE2% </span> </p>
                    <p> time on <span id="DEVICE2_hour_on">%DEVICE2_hour_on% </span>:<span id="DEVICE2_min_on">%DEVICE2_min_on% </span> <input type="time" name="DEVICE2_hour_on">
                    <p>time off<span id="DEVICE2_hour_off"> %DEVICE2_hour_off%</span> :<span id="DEVICE2_min_off">%DEVICE2_min_off%</span>  <input type="time" name="DEVICE2_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE3 state <span id="DEVICE3"> %DEVICE3% </span> </p>
                    <p> time on <span id="DEVICE3_hour_on">%DEVICE3_hour_on% </span>:<span id="DEVICE3_min_on"> %DEVICE3_min_on% </span><input type="time" name="DEVICE3_hour_on">
                    <p>time off <span id="DEVICE3_hour_off">%DEVICE3_hour_off% </span>:<span id="DEVICE3_min_off">%DEVICE3_min_off% </span> <input type="time" name="DEVICE3_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE4 state <span id="DEVICE4"> %DEVICE4% </span> </p>
                    <p> time on <span id="DEVICE4_hour_on">%DEVICE4_hour_on% </span>: <span id="DEVICE4_min_on">%DEVICE4_min_on% </span><input type="time" name="DEVICE4_hour_on">
                    <p>time off<span id="DEVICE4_hour_off"> %DEVICE4_hour_off% </span>: <span id="DEVICE4_min_off">%DEVICE4_min_off% </span><input type="time" name="DEVICE4_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE5 state <span id="DEVICE5"> %DEVICE5% </span> </p>
                    <p> time on <span id="DEVICE5_hour_on">%DEVICE5_hour_on% </span>: <span id="DEVICE5_min_on">%DEVICE5_min_on% </span><input type="time" name="DEVICE5_hour_on">
                    <p>time off<span id="DEVICE5_hour_off"> %DEVICE5_hour_off% </span>: <span id="DEVICE5_min_off">%DEVICE5_min_off%</span> <input type="time" name="DEVICE5_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE6 state <span id="DEVICE6"> %DEVICE6% </span> </p>
                    <p> time on<span id="DEVICE6_hour_on"> %DEVICE6_hour_on% </span>:<span id="DEVICE6_min_on"> %DEVICE6_min_on%</span> <input type="time" name="DEVICE6_hour_on">
                    <p>time off <span id="DEVICE6_hour_off">%DEVICE6_hour_off% </span>: <span id="DEVICE6_min_off">%DEVICE6_min_off%</span> <input type="time" name="DEVICE6_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE7 state <span id="DEVICE7"> %DEVICE7% </span> </p>
                    <p> time on <span id="DEVICE7_hour_on">%DEVICE7_hour_on% </span>:<span id="DEVICE7_min_on"> %DEVICE7_min_on% </span><input type="time" name="DEVICE7_hour_on">
                    <p>time off <span id="DEVICE7_hour_off">%DEVICE7_hour_off% </span>: <span id="DEVICE7_min_off">%DEVICE7_min_off%</span> <input type="time" name="DEVICE7_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DEVICE8 state <span id="DEVICE8"> %DEVICE8% </span> </p>
                    <p> time on <span id="DEVICE8_hour_on">%DEVICE8_hour_on% </span>: <span id="DEVICE8_min_on">%DEVICE8_min_on% </span><input type="time" name="DEVICE8_hour_on">
                    <p>time off <span id="DEVICE8_hour_off">%DEVICE8_hour_off% </span>:<span id="DEVICE8_min_off">%DEVICE8_min_off% </span> <input type="time" name="DEVICE8_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DRIVE1 state <span id="DRIVE1"> %DRIVE1% </span> </p>
                    <p> time on<span id="DRIVE1_hour_on"> %DRIVE1_hour_on%</span> :<span id="DRIVE1_min_on"> %DRIVE1_min_on% </span><input type="time" name="DRIVE1_hour_on">
                    <p>time off <span id="DRIVE1_hour_off">%DRIVE1_hour_off%</span> :<span id="DRIVE1_min_off">%DRIVE1_min_off%</span>  <input type="time" name="DRIVE1_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DRIVE2 state <span id="DRIVE2"> %DRIVE2% </span> </p>
                    <p> time on<span id="DRIVE2_hour_on"> %DRIVE2_hour_on% </span>: <span id="DRIVE2_min_on">%DRIVE2_min_on% </span><input type="time" name="DRIVE2_hour_on">
                    <p>time off <span id="DRIVE2_hour_off">%DRIVE2_hour_off% </span>:<span id="DRIVE2_min_off">%DRIVE2_min_off% </span> <input type="time" name="DRIVE2_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DRIVE3 state <span id="DRIVE3"> %DRIVE3% </span> </p>
                    <p> time on <span id="DRIVE3_hour_on">%DRIVE3_hour_on%</span> :<span id="DRIVE3_min_on"> %DRIVE3_min_on%</span> <input type="time" name="DRIVE3_hour_on">
                    <p>time off<span id="DRIVE3_hour_off"> %DRIVE3_hour_off%</span> : <span id="DRIVE3_min_off">%DRIVE3_min_off%</span> <input type="time" name="DRIVE3_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>

            <div class="card">
                <form id="red-form" action="/get" target="hidden-form">
                    <p>DRIVE4 state <span id="DRIVE4"> %DRIVE4% </span> </p>
                    <p> time on <span id="DRIVE4_hour_on">%DRIVE4_hour_on%</span> : <span id="DRIVE4_min_on">%DRIVE4_min_on% </span><input type="time" name="DRIVE4_hour_on">
                    <p>time off <span id="DRIVE4_hour_off">%DRIVE4_hour_off%</span> : <span id="DRIVE4_min_off">%DRIVE4_min_off%</span> <input type="time" name="DRIVE4_hour_off"></p>
                    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
                </form><br>
            </div>
        </div>

        <iframe style="display:none" name="hidden-form"></iframe>
    <script>
            if (!!window.EventSource) {
                var source = new EventSource('/events');

                source.addEventListener('open', function (e) {
                }, false);
                source.addEventListener('error', function (e) {
                    if (e.target.readyState != EventSource.OPEN) {
                    }
                }, false);

                source.addEventListener('message', function (e) {
                }, false);


let  saf=[
    "HOUR", "MIN", "TEMP", "LEAK", "LIGHT",
    "BOBBER1", "BOBBER2", "BOBBER3", "BOBBER4",
    "SWITCH1", "SWITCH2", "SWITCH3", "SWITCH4",
    "OBTACLE1", "OBTACLE2",

    "STEP1_STATE", "STEP2_STATE", "STEP3_STATE", "STEP4_STATE", 
    "SOLI", "LASER1", "LASER2",
    "VALVE1", "VALVE2", "VALVE3", "VALVE4", "VALVE5", "VALVE6",
    "PUMP1", "PUMP2", "PUMP3", "PUMP4", "BIG_PUMP",
    "DEVICE1", "DEVICE2", "DEVICE3", "DEVICE4", "DEVICE5", "DEVICE6", "DEVICE7", "DEVICE8",
    "DRIVE1", "DRIVE2", "DRIVE3", "DRIVE4", 

    "STEP3_hour_on", "STEP3_min_on", "STEP3_hour_off", "STEP3_min_off",
    "STEP4_hour_on", "STEP4_min_on", "STEP4_hour_off", "STEP4_min_off",

    "SOLI_hour_on", "SOLI_min_on", "SOLI_hour_off", "SOLI_min_off",
    "LASER1_hour_on", "LASER1_min_on", "LASER1_hour_off", "LASER1_min_off",
    "LASER2_hour_on", "LASER2_min_on", "LASER2_hour_off", "LASER2_min_off",
    "VALVE1_hour_on", "VALVE1_min_on", "VALVE1_hour_off", "VALVE1_min_off",
    "VALVE2_hour_on", "VALVE2_min_on", "VALVE2_hour_off", "VALVE2_min_off",
    "VALVE3_hour_on", "VALVE3_min_on", "VALVE3_hour_off", "VALVE3_min_off",
    "VALVE4_hour_on", "VALVE4_min_on", "VALVE4_hour_off", "VALVE4_min_off",
    "VALVE5_hour_on", "VALVE5_min_on", "VALVE5_hour_off", "VALVE5_min_off",
    "VALVE6_hour_on", "VALVE6_min_on", "VALVE6_hour_off", "VALVE6_min_off",

    "PUMP1_hour_on", "PUMP1_min_on", "PUMP1_hour_off", "PUMP1_min_off",
    "PUMP2_hour_on", "PUMP2_min_on", "PUMP2_hour_off", "PUMP2_min_off",
    "PUMP3_hour_on", "PUMP3_min_on", "PUMP3_hour_off", "PUMP3_min_off",
    "PUMP4_hour_on", "PUMP4_min_on", "PUMP4_hour_off", "PUMP4_min_off",
    "BIG_PUMP_hour_on", "BIG_PUMP_min_on", "BIG_PUMP_hour_off", "BIG_PUMP_min_off",

    "DEVICE1_hour_on", "DEVICE1_min_on", "DEVICE1_hour_off", "DEVICE1_min_off",
    "DEVICE2_hour_on", "DEVICE2_min_on", "DEVICE2_hour_off", "DEVICE2_min_off",
    "DEVICE3_hour_on", "DEVICE3_min_on", "DEVICE3_hour_off", "DEVICE3_min_off",
    "DEVICE4_hour_on", "DEVICE4_min_on", "DEVICE4_hour_off", "DEVICE4_min_off",
    "DEVICE5_hour_on", "DEVICE5_min_on", "DEVICE5_hour_off", "DEVICE5_min_off",
    "DEVICE6_hour_on", "DEVICE6_min_on", "DEVICE6_hour_off", "DEVICE6_min_off",
    "DEVICE7_hour_on", "DEVICE7_min_on", "DEVICE7_hour_off", "DEVICE7_min_off",
    "DEVICE8_hour_on", "DEVICE8_min_on", "DEVICE8_hour_off", "DEVICE8_min_off",
    "DRIVE1_hour_on", "DRIVE1_min_on", "DRIVE1_hour_off", "DRIVE1_min_off",
    "DRIVE2_hour_on", "DRIVE2_min_on", "DRIVE2_hour_off", "DRIVE2_min_off",
    "DRIVE3_hour_on", "DRIVE3_min_on", "DRIVE3_hour_off", "DRIVE3_min_off",
    "DRIVE4_hour_on", "DRIVE4_min_on", "DRIVE4_hour_off", "DRIVE4_min_off",

    "STEP1_hour_on", "STEP1_min_on", "STEP1_hour_off", "STEP1_min_off",
    "STEP2_hour_on", "STEP2_min_on", "STEP2_hour_off", "STEP2_min_off",
    "StartMonth", "StartDay", "StartHour", "StartMin",
    "watering", "irrigation", "begin", "cycle",
    "TankFlag", "CycleDay", "CycleHour",
    "IrrigationMix", "WateringMix",
    "STEP1_value", "STEP2_value", "STEP3_value", "STEP4_value", "key"
    ];

    for (let i = 0; i < 181; i++) {
        source.addEventListener(saf[i], function (e) {
        document.getElementById(saf[i]).innerHTML = e.data;
        }, false);
    }
}
</script>
</body>

</html>)rawliteral";

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char *path)
{
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        return String();
    }
    String fileContent;
    while (file.available())
    {
        fileContent += String((char)file.read());
    }
    return fileContent;
}

void writeFile(fs::FS &fs, const char *path, String message)
{
    File file = fs.open(path, "w");
    if (!file)
    {
        return;
    }
    if (file.print(message))
    {
    }
    else
    {
    }
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    File file = fs.open(path, "w");
    if (!file)
    {
        return;
    }
    if (file.print(message))
    {
    }
    else
    {
    }
}

String processor2(const String &var)
{
    for (int j = 0; j < leng; j++)
    {
        if (var == parametr[j])
        {
            TemporaryFile1 = parametr[j];
            TemporaryFile2 = "";
            if (j != leng - 1)
            {
                for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
                {
                    TemporaryFile2 += GeneralString[i];
                }
            }
            else if (j == leng - 1)
            {
                for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
                {
                    TemporaryFile2 += GeneralString[i];
                }
            }
            return TemporaryFile2;
        }
    }
    return String();
}

String GettingValueFromString(int j)
{
    TemporaryFile1 = parametr[j];
    TemporaryFile2 = "";
    if (j != leng - 1)
    {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
        {
            TemporaryFile2 += GeneralString[i];
        }
    }
    else if (j == leng - 1)
    {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
        {
            TemporaryFile2 += GeneralString[i];
        }
    }
    return TemporaryFile2;
}

void SendingValueToString(int j, int k)
{
    TemporaryFile1 = parametr[j];
    TemporaryFile2 = "";
    if (j != leng - 1)
    {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
        {
            TemporaryFile2 += GeneralString[i];
        }
    }
    else if (j == leng - 1)
    {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
        {
            TemporaryFile2 += GeneralString[i];
        }
    }
    GeneralString.replace(parametr[j] + TemporaryFile2, parametr[j] + String(k));
}

void WiFiSetup()
{
    Serial.println("ESP8266 INIT");
    Serial.println(F("Inizializing FS..."));
    (LittleFS.begin()) ? Serial.println(F("done.")) : Serial.println(F("fail."));

    GeneralString = readFile(LittleFS, MyFile);

    GeneralString="";
        for (int i = 0; i < leng-1; i++)
        {
            GeneralString += parametr[i] + String(2);
        }
        GeneralString += parametr[leng - 1] + String(254);
        writeFile(LittleFS, MyFile, GeneralString);

    for (int i = 0; i < leng; i++)
    {
        PreviousSensorState[i] = 0;
        InternetSensorState[i] = 0;
        CurrentSensorState[i] = GettingValueFromString(i).toInt();
    }
    Serial.println("GeneralString ");
    Serial.println(GeneralString);
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.autoConnect("AutoConnectAP");
    Serial.println("connected...yeey :)");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html, processor2); });
    // Send a GET request to <ESP_IP>/get?ENC=<inputMessage>

    server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  String inputMessage;
                  for (int i = 14; i < leng; i++)
                  {
                      if (request->hasParam(parametr[i]) && i < 169 && i != 165)
                      {
                          inputMessage = request->getParam(parametr[i])->value();
                          InternetSensorState[i] = inputMessage.substring(0, 2).toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                          i++;
                          InternetSensorState[i] = inputMessage.substring(3, 5).toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                      }
                      else if (request->hasParam(parametr[i]) && i >= 169 && i != 172)
                      {
                          inputMessage = request->getParam(parametr[i])->value();
                          InternetSensorState[i] = inputMessage.toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                      }
                      else if (request->hasParam(parametr[i]) && i == 165)
                      {
                          inputMessage = request->getParam(parametr[i])->value();
                          if (inputMessage.length() > 16)
                          {
                              inputMessage = inputMessage.substring(inputMessage.length() - 16, 18);
                          }
                          InternetSensorState[i] = inputMessage.substring(5, 7).toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                          i++;
                          InternetSensorState[i] = inputMessage.substring(8, 11).toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                          i++;
                          InternetSensorState[i] = inputMessage.substring(11, 14).toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                          i++;
                          InternetSensorState[i] = inputMessage.substring(14, 17).toInt();
                          SendingValueToString(i, InternetSensorState[i]);
                      }
                      if (request->hasParam(parametr[i]) && i == 172)
                      {
                          inputMessage = request->getParam(parametr[i])->value();
                          InternetSensorState[175] = inputMessage.toInt() % 24;
                          InternetSensorState[174] = (inputMessage.toInt() - InternetSensorState[175]) / 24;
                          SendingValueToString(174, InternetSensorState[174]);
                          SendingValueToString(175, InternetSensorState[175]);
                          SendingValueToString(172, InternetSensorState[174] * 24 + InternetSensorState[175]);
                          events.send(String(InternetSensorState[174] * 24 + InternetSensorState[175]).c_str(), parametr[172], millis());
                      }
                      if (!Internet_flag)
                          Internet_flag = 1;
                  }
                  request->send(200, "text/text", inputMessage);
              });
    server.onNotFound(notFound);

    events.onConnect([](AsyncEventSourceClient *client)
                     {
                         if (client->lastId())
                         {
                             Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
                         }
                         // send event with message "hello!", id current millis
                         // and set reconnect delay to 1 second
                         client->send("hello!", NULL, millis(), 10000);
                     });
    server.addHandler(&events);
    server.begin();

// other adjust
#ifndef ESP8266
    while (!Serial)
        ; // wait for serial port to connect. Needed for native USB
#endif
}