
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
    "OBTACLE1", "OBTACLE2", //15

    "SOLI", "LASER1", "LASER2",
    "VALVE1", "VALVE2", "VALVE3", "VALVE4", "VALVE5", "VALVE6",
    "PUMP1", "PUMP2", "PUMP3", "PUMP4", "BIG_PUMP",
    "DEVICE1", "DEVICE2", "DEVICE3", "DEVICE4", "DEVICE5", "DEVICE6", "DEVICE7", "DEVICE8",
    "STEP1", "STEP2", "STEP3", "STEP4",
    "DRIVE1", "DRIVE2", "DRIVE3", "DRIVE4", //30

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
    "DEVICE8_hour_on", "DEVICE8_min_on", "DEVICE8_hour_off", "DEVICE8_min_off", //88

    "STEP1_hour_on00", "STEP1_min_on00",
    "STEP1_hour_on01", "STEP1_min_on01",
    "STEP1_hour_on02", "STEP1_min_on02",
    "STEP1_hour_on03", "STEP1_min_on03",
    "STEP1_hour_on04", "STEP1_min_on04",
    "STEP1_hour_on05", "STEP1_min_on05",
    "STEP1_hour_on06", "STEP1_min_on06",
    "STEP1_hour_on07", "STEP1_min_on07",
    "STEP1_hour_on08", "STEP1_min_on08",
    "STEP1_hour_on09", "STEP1_min_on09",
    "STEP1_hour_on10", "STEP1_min_on10",
    "STEP1_hour_on11", "STEP1_min_on11",
    "STEP1_hour_on12", "STEP1_min_on12",
    "STEP1_hour_on13", "STEP1_min_on13",
    "STEP1_hour_on14", "STEP1_min_on14",
    "STEP1_hour_on15", "STEP1_min_on15",
    "STEP1_hour_on16", "STEP1_min_on16",
    "STEP1_hour_on17", "STEP1_min_on17", //36
    "STEP2_hour_on", "STEP2_min_on", "STEP2_hour_off", "STEP2_min_off",
    "STEP3_hour_on", "STEP3_min_on", "STEP3_hour_off", "STEP3_min_off",
    "STEP4_hour_on", "STEP4_min_on", "STEP4_hour_off", "STEP4_min_off",
    "DRIVE1_hour_on", "DRIVE1_min_on", "DRIVE1_hour_off", "DRIVE1_min_off",
    "DRIVE2_hour_on", "DRIVE2_min_on", "DRIVE2_hour_off", "DRIVE2_min_off",
    "DRIVE3_hour_on", "DRIVE3_min_on", "DRIVE3_hour_off", "DRIVE3_min_off",
    "DRIVE4_hour_on", "DRIVE4_min_on", "DRIVE4_hour_off", "DRIVE4_min_off",
    "STEP1_value", "STEP2_value", "STEP3_value", "STEP4_value"}; //28

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP LittleFS");
      setTimeout(function(){ document.location.reload(false); }, 500);  
}
  </script>

<style> 
html 
{
  font-family: Arial; 
  display: inline-block; 
  text-align: center;
}

body {  margin: 0;}
.content { padding: 20px; }

input[type=text]{
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
.card 
{ 
  padding: 10px;
  background-color: white; 
  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); 
}
p {
  color: navy;
  text-indent: 10px;
  margin-top: 0em;
  margin-bottom: 0em;
}

 .cards { max-width: 800; 
 margin: 0 auto; display: grid; 
 grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); 
 }

</style>

  </head>
  <body>
 <div class="cards">
 <div class="card">
<form action="/get" target="hidden-form">
    <p>Current time  <span id="HOUR"> %HOUR%</span> :  <span id="MIN">%MIN%</span>   </p>
  </form><br>

<form action="/get" target="hidden-form">
    <p>Temperature <span id="TEMP">%TEMP%</span> &deg;C</span>  </p>
  </form><br>

 <form action="/get" target="hidden-form">
   <p> Water leaks <span id="LEAK"> %LEAK% </span>  </p>
  </form><br>

  <form action="/get" target="hidden-form">
  <p>Light sensor  <span id="LIGHT">%LIGHT% </span>  </p>
  </form><br>
  
  <form action="/get" target="hidden-form">
     <p>Obtacle1 <span id="OBTACLE1"> %OBTACLE1% </span>    </p>
      <p>Obtacle2  <span id="OBTACLE2">%OBTACLE2% </span> </p>
  </form><br>
  
  <form action="/get" target="hidden-form">
    <p>Bobber_1 <span id="BOBBER1"> %BOBBER1% </span> </p>
    <p>Bobber_2  <span id="BOBBER2">%BOBBER2% </span> </p>
    <p>Bobber_3 <span id="BOBBER3">%BOBBER3% </span> </p>
    <p>Bobber_4  <span id="BOBBER4">%BOBBER4% </span></p>
  </form><br>
 
  <form action="/get" target="hidden-form">
    <p>Limit switch_1 <span id="SWITCH1"> %SWITCH1%  </span>  </p>
    <p>Limit switch_2 <span id="SWITCH2"> %SWITCH2%  </span>  </p>
    <p>Limit switch_3  <span id="SWITCH3">%SWITCH3%   </span> </p> 
    <p>Limit switch_4  <span id="SWITCH4">%SWITCH4%</span></p> 
  </form><br>

    </div>
 <div class="card">

<form id="red-form" action="/get" target="hidden-form">
    <p>STEP1 state <span id="STEP1"> %STEP1% </span> </p>
    <p> corner %STEP1_value% <input type="text" name="STEP1_value">
    <p> time on1 %STEP1_hour_on00% : %STEP1_min_on00% <input type="time" name="STEP1_hour_on00">
    <p> time on2 %STEP1_hour_on01% : %STEP1_min_on01% <input type="time" name="STEP1_hour_on01">
    <p> time on3 %STEP1_hour_on02% : %STEP1_min_on02% <input type="time" name="STEP1_hour_on02">
    <p> time on4 %STEP1_hour_on03% : %STEP1_min_on03% <input type="time" name="STEP1_hour_on03">
    <p> time on5 %STEP1_hour_on04% : %STEP1_min_on04% <input type="time" name="STEP1_hour_on04">
    <p> time on6 %STEP1_hour_on05% : %STEP1_min_on05% <input type="time" name="STEP1_hour_on05">
    <p> time on7 %STEP1_hour_on06% : %STEP1_min_on06% <input type="time" name="STEP1_hour_on06">
    <p> time on8 %STEP1_hour_on07% : %STEP1_min_on07% <input type="time" name="STEP1_hour_on07">
    <p> time on9 %STEP1_hour_on08% : %STEP1_min_on08% <input type="time" name="STEP1_hour_on08">
    <p> time on10 %STEP1_hour_on09% : %STEP1_min_on09% <input type="time" name="STEP1_hour_on09">
    <p> time on11 %STEP1_hour_on10% : %STEP1_min_on10% <input type="time" name="STEP1_hour_on10">
    <p> time on12 %STEP1_hour_on11% : %STEP1_min_on11% <input type="time" name="STEP1_hour_on11">
    <p> time on13 %STEP1_hour_on12% : %STEP1_min_on12% <input type="time" name="STEP1_hour_on12">
    <p> time on14 %STEP1_hour_on13% : %STEP1_min_on13% <input type="time" name="STEP1_hour_on13">
    <p> time on15 %STEP1_hour_on14% : %STEP1_min_on14% <input type="time" name="STEP1_hour_on14">
    <p> time on16 %STEP1_hour_on15% : %STEP1_min_on15% <input type="time" name="STEP1_hour_on15">
    <p> time on17 %STEP1_hour_on16% : %STEP1_min_on16% <input type="time" name="STEP1_hour_on16">
    <p> time on18 %STEP1_hour_on17% : %STEP1_min_on17% <input type="time" name="STEP1_hour_on17">
    <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>
  </div>



   <div class="content">
 <div class="cards">
 <div class="card">
  <form id="red-form" action="/get" target="hidden-form">
    <p>Solinoid state  <span id="SOLI"> %SOLI% </span> </p>
    <p> time on %SOLI_hour_on% : %SOLI_min_on% <input type="time" name="SOLI_hour_on">
        <p> time off %SOLI_hour_off% : %SOLI_min_off% <input type="time" name="SOLI_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"></p> 
  </form><br>
  </div>
  
<div class="card">
  <form id="red-form" action="/get" target="hidden-form">
    <p>Laser 1 state <span id="LASER1"> %LASER1%</span>  </p>
    <p> time on %LASER1_hour_on% : %LASER1_min_on% <input type="time" name="LASER1_hour_on">
    <p>time off %LASER1_hour_off% : %LASER1_min_off% <input type="time" name="LASER1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>Laser 2 state  <span id="LASER2">%LASER2% </span> </p>
    <p> time on %LASER2_hour_on% : %LASER2_min_on% <input type="time" name="LASER2_hour_on">
    <p>time off %LASER2_hour_off% : %LASER2_min_off% <input type="time" name="LASER2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE1 state  <span id="VALVE1">%VALVE1% </span>  </p>
    <p> time on %VALVE1_hour_on% : %VALVE1_min_on% <input type="time" name="VALVE1_hour_on">
    <p>time off %VALVE1_hour_off% : %VALVE1_min_off% <input type="time" name="VALVE1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>
  
<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE2 state <span id="VALVE2"> %VALVE2% </span> </p>
    <p> time on %VALVE2_hour_on% : %VALVE2_min_on% <input type="time" name="VALVE2_hour_on">
    <p>time off %VALVE2_hour_off% : %VALVE2_min_off% <input type="time" name="VALVE2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE3 state  <span id="VALVE3">%VALVE3%</span>  </p>
    <p> time on %VALVE3_hour_on% : %VALVE3_min_on% <input type="time" name="VALVE3_hour_on">
    <p>time off %VALVE3_hour_off% : %VALVE3_min_off% <input type="time" name="VALVE3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE4 state <span id="VALVE4"> %VALVE4% </span> </p>
    <p> time on %VALVE4_hour_on% : %VALVE4_min_on% <input type="time" name="VALVE4_hour_on">
    <p>time off %VALVE4_hour_off% : %VALVE4_min_off% <input type="time" name="VALVE4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE5 state  <span id="VALVE5">%VALVE5% </span> </p>
    <p> time on %VALVE5_hour_on% : %VALVE5_min_on% <input type="time" name="VALVE5_hour_on">
    <p>time off %VALVE5_hour_off% : %VALVE5_min_off% <input type="time" name="VALVE5_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE6 state <span id="VALVE6"> %VALVE6% </span> </p>
    <p> time on %VALVE6_hour_on% : %VALVE6_min_on% <input type="time" name="VALVE6_hour_on">
    <p>time off %VALVE6_hour_off% : %VALVE6_min_off% <input type="time" name="VALVE6_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP1 state <span id="PUMP1">%PUMP1% </span> </p>
    <p> time on %PUMP1_hour_on% : %PUMP1_min_on% <input type="time" name="PUMP1_hour_on">
    <p>time off %PUMP1_hour_off% : %PUMP1_min_off% <input type="time" name="PUMP1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP2 state  <span id="PUMP2"> %PUMP2% </span> </p>
    <p> time on %PUMP2_hour_on% : %PUMP2_min_on% <input type="time" name="PUMP2_hour_on">
    <p>time off %PUMP2_hour_off% : %PUMP2_min_off% <input type="time" name="PUMP2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP3 state <span id="PUMP3"> %PUMP3% </span> </p>
    <p> time on %PUMP3_hour_on% : %PUMP3_min_on% <input type="time" name="PUMP3_hour_on">
    <p>time off %PUMP3_hour_off% : %PUMP3_min_off% <input type="time" name="PUMP3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP4 state  <span id="PUMP4"> %PUMP4% </span> </p>
    <p> time on %PUMP4_hour_on% : %PUMP4_min_on% <input type="time" name="PUMP4_hour_on">
    <p>time off %PUMP4_hour_off% : %PUMP4_min_off% <input type="time" name="PUMP4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE1 state  <span id="DEVICE1"> %DEVICE1% </span> </p>
    <p> time on %DEVICE1_hour_on% : %DEVICE1_min_on% <input type="time" name="DEVICE1_hour_on">
    <p>time off %DEVICE1_hour_off% : %DEVICE1_min_off% <input type="time" name="DEVICE1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE2 state  <span id="DEVICE2">%DEVICE2% </span> </p>
    <p> time on %DEVICE2_hour_on% : %DEVICE2_min_on% <input type="time" name="DEVICE2_hour_on">
    <p>time off %DEVICE2_hour_off% : %DEVICE2_min_off% <input type="time" name="DEVICE2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE3 state  <span id="DEVICE3"> %DEVICE3% </span> </p>
    <p> time on %DEVICE3_hour_on% : %DEVICE3_min_on% <input type="time" name="DEVICE3_hour_on">
    <p>time off %DEVICE3_hour_off% : %DEVICE3_min_off% <input type="time" name="DEVICE3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE4 state  <span id="DEVICE4"> %DEVICE4% </span> </p>
    <p> time on %DEVICE4_hour_on% : %DEVICE4_min_on% <input type="time" name="DEVICE4_hour_on">
    <p>time off %DEVICE4_hour_off% : %DEVICE4_min_off% <input type="time" name="DEVICE4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE5 state <span id="DEVICE5"> %DEVICE5% </span> </p>
    <p> time on %DEVICE5_hour_on% : %DEVICE5_min_on% <input type="time" name="DEVICE5_hour_on">
    <p>time off %DEVICE5_hour_off% : %DEVICE5_min_off% <input type="time" name="DEVICE5_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE6 state <span id="DEVICE6"> %DEVICE6% </span> </p>
    <p> time on %DEVICE6_hour_on% : %DEVICE6_min_on% <input type="time" name="DEVICE6_hour_on">
    <p>time off %DEVICE6_hour_off% : %DEVICE6_min_off% <input type="time" name="DEVICE6_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE7 state <span id="DEVICE7"> %DEVICE7% </span> </p>
    <p> time on %DEVICE7_hour_on% : %DEVICE7_min_on% <input type="time" name="DEVICE7_hour_on">
    <p>time off %DEVICE7_hour_off% : %DEVICE7_min_off% <input type="time" name="DEVICE7_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE8 state <span id="DEVICE8"> %DEVICE8% </span> </p>
    <p> time on %DEVICE8_hour_on% : %DEVICE8_min_on% <input type="time" name="DEVICE8_hour_on">
    <p>time off %DEVICE8_hour_off% : %DEVICE8_min_off% <input type="time" name="DEVICE8_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
<p> corner %STEP2_value% <input type="text" name="STEP2_value">
    <p>STEP2 state <span id="STEP2"> %STEP2% </span> </p>
    <p> checking time %STEP2_hour_on% : %STEP2_min_on% <input type="time" name="STEP2_hour_on">
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  
  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
<p> corner %STEP3_value% <input type="text" name="STEP3_value">
    <p>STEP3 state <span id="STEP3"> %STEP3% </span> </p>
    <p> time on %STEP3_hour_on% : %STEP3_min_on% <input type="time" name="STEP3_hour_on">
    <p>time off %STEP3_hour_off% : %STEP3_min_off% <input type="time" name="STEP3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
<p> corner %STEP4_value% <input type="text" name="STEP4_value">
    <p>STEP4 state <span id="STEP4"> %STEP4% </span> </p>
    <p> time on %STEP4_hour_on% : %STEP4_min_on% <input type="time" name="STEP4_hour_on">
    <p>time off %STEP4_hour_off% : %STEP4_min_off% <input type="time" name="STEP4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE1 state <span id="DRIVE1"> %DRIVE1% </span> </p>
    <p> time on %DRIVE1_hour_on% : %DRIVE1_min_on% <input type="time" name="DRIVE1_hour_on">
    <p>time off %DRIVE1_hour_off% : %DRIVE1_min_off% <input type="time" name="DRIVE1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  
  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE2 state <span id="DRIVE2"> %DRIVE2% </span> </p>
    <p> time on %DRIVE2_hour_on% : %DRIVE2_min_on% <input type="time" name="DRIVE2_hour_on">
    <p>time off %DRIVE2_hour_off% : %DRIVE2_min_off% <input type="time" name="DRIVE2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE3 state <span id="DRIVE3"> %DRIVE3% </span> </p>
    <p> time on %DRIVE3_hour_on% : %DRIVE3_min_on% <input type="time" name="DRIVE3_hour_on">
    <p>time off %DRIVE3_hour_off% : %DRIVE3_min_off% <input type="time" name="DRIVE3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  
 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE4 state <span id="DRIVE4"> %DRIVE4% </span> </p>
    <p> time on %DRIVE4_hour_on% : %DRIVE4_min_on% <input type="time" name="DRIVE4_hour_on">
    <p>time off %DRIVE4_hour_off% : %DRIVE4_min_off% <input type="time" name="DRIVE4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
</div>
 
  <iframe style="display:none" name="hidden-form"></iframe>



  <script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('MIN', function(e) {
  console.log("MIN", e.data);
  document.getElementById("MIN").innerHTML = e.data;
 }, false);
 
 source.addEventListener('HOUR', function(e) {
  console.log("HOUR", e.data);
  document.getElementById("HOUR").innerHTML = e.data;
 }, false);

 source.addEventListener('TEMP', function(e) {
  console.log("TEMP", e.data);
  document.getElementById("TEMP").innerHTML = e.data;
 }, false);
 
 source.addEventListener('LEAK', function(e) {
  console.log("LEAK", e.data);
  document.getElementById("LEAK").innerHTML = e.data;
 }, false);
 
 source.addEventListener('LIGHT', function(e) {
  console.log("LIGHT", e.data);
  document.getElementById("LIGHT").innerHTML = e.data;
 }, false);
 
 source.addEventListener('OBTACLE1', function(e) {
  console.log("OBTACLE1", e.data);
  document.getElementById("OBTACLE1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('OBTACLE2', function(e) {
  console.log("OBTACLE2", e.data);
  document.getElementById("OBTACLE2").innerHTML = e.data;
 }, false);
 
 source.addEventListener('BOBBER1', function(e) {
  console.log("BOBBER1", e.data);
  document.getElementById("BOBBER1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('BOBBER2', function(e) {
  console.log("BOBBER2", e.data);
  document.getElementById("BOBBER2").innerHTML = e.data;
 }, false);
 
 source.addEventListener('BOBBER3', function(e) {
  console.log("BOBBER3", e.data);
  document.getElementById("BOBBER3").innerHTML = e.data;
 }, false);
 
 source.addEventListener('BOBBER4', function(e) {
  console.log("BOBBER4", e.data);
  document.getElementById("BOBBER4").innerHTML = e.data;
 }, false);
 
 source.addEventListener('SWITCH1', function(e) {
  console.log("SWITCH1", e.data);
  document.getElementById("SWITCH1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('SWITCH2', function(e) {
  console.log("SWITCH2", e.data);
  document.getElementById("SWITCH2").innerHTML = e.data;
 }, false);
 
 source.addEventListener('SWITCH3', function(e) {
  console.log("SWITCH3", e.data);
  document.getElementById("SWITCH3").innerHTML = e.data;
 }, false);
 
 source.addEventListener('SWITCH4', function(e) {
  console.log("SWITCH4", e.data);
  document.getElementById("SWITCH4").innerHTML = e.data;
 }, false);

 source.addEventListener('SOLI', function(e) {
  console.log("SOLI", e.data);
  document.getElementById("SOLI").innerHTML = e.data;
 }, false);
 
 source.addEventListener('LASER1', function(e) {
  console.log("LASER1", e.data);
  document.getElementById("LASER1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('LASER2', function(e) {
  console.log("LASER2", e.data);
  document.getElementById("LASER2").innerHTML = e.data;
 }, false);
 
 source.addEventListener('VALVE1', function(e) {
  console.log("VALVE1", e.data);
  document.getElementById("VALVE1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('VALVE2', function(e) {
  console.log("VALVE2", e.data);
  document.getElementById("VALVE2").innerHTML = e.data;
 }, false);
 
 source.addEventListener('VALVE3', function(e) {
  console.log("VALVE3", e.data);
  document.getElementById("VALVE3").innerHTML = e.data;
 }, false);
 
 source.addEventListener('VALVE4', function(e) {
  console.log("VALVE4", e.data);
  document.getElementById("VALVE4").innerHTML = e.data;
 }, false);
 
 source.addEventListener('VALVE5', function(e) {
  console.log("VALVE5", e.data);
  document.getElementById("VALVE5").innerHTML = e.data;
 }, false);
 
 source.addEventListener('VALVE6', function(e) {
  console.log("VALVE6", e.data);
  document.getElementById("VALVE6").innerHTML = e.data;
 }, false);
 
 source.addEventListener('PUMP1', function(e) {
  console.log("PUMP1", e.data);
  document.getElementById("PUMP1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('PUMP2', function(e) {
  console.log("PUMP2", e.data);
  document.getElementById("PUMP2").innerHTML = e.data;
 }, false);
 
 source.addEventListener('PUMP3', function(e) {
  console.log("PUMP3", e.data);
  document.getElementById("PUMP3").innerHTML = e.data;
 }, false);
 
 source.addEventListener('PUMP4', function(e) {
  console.log("PUMP4", e.data);
  document.getElementById("PUMP4").innerHTML = e.data;
 }, false);
 
 source.addEventListener('DEVICE1', function(e) {
  console.log("DEVICE1", e.data);
  document.getElementById("DEVICE1").innerHTML = e.data;
 }, false);
 
source.addEventListener('DEVICE2', function(e) {
  console.log("DEVICE2", e.data);
  document.getElementById("DEVICE2").innerHTML = e.data;
 }, false);

 source.addEventListener('DEVICE3', function(e) {
  console.log("DEVICE3", e.data);
  document.getElementById("DEVICE3").innerHTML = e.data;
 }, false);

 source.addEventListener('DEVICE4', function(e) {
  console.log("DEVICE4", e.data);
  document.getElementById("DEVICE4").innerHTML = e.data;
 }, false);
 
source.addEventListener('DEVICE5', function(e) {
  console.log("DEVICE5", e.data);
  document.getElementById("DEVICE5").innerHTML = e.data;
 }, false);

 source.addEventListener('DEVICE6', function(e) {
  console.log("DEVICE6", e.data);
  document.getElementById("DEVICE6").innerHTML = e.data;
 }, false);

 source.addEventListener('DEVICE7', function(e) {
  console.log("DEVICE7", e.data);
  document.getElementById("DEVICE7").innerHTML = e.data;
 }, false);
 
source.addEventListener('DEVICE8', function(e) {
  console.log("DEVICE8", e.data);
  document.getElementById("DEVICE8").innerHTML = e.data;
 }, false);

 source.addEventListener('STEP1', function(e) {
  console.log("STEP1", e.data);
  document.getElementById("STEP1").innerHTML = e.data;
 }, false);

 source.addEventListener('STEP2', function(e) {
  console.log("STEP2", e.data);
  document.getElementById("STEP2").innerHTML = e.data;
 }, false);
 
source.addEventListener('STEP3', function(e) {
  console.log("STEP3", e.data);
  document.getElementById("STEP3").innerHTML = e.data;
 }, false);

 source.addEventListener('STEP4', function(e) {
  console.log("STEP4", e.data);
  document.getElementById("STEP4").innerHTML = e.data;
 }, false);

 source.addEventListener('DRIVE1', function(e) {
  console.log("DRIVE1", e.data);
  document.getElementById("DRIVE1").innerHTML = e.data;
 }, false);
 
 source.addEventListener('DRIVE2', function(e) {
  console.log("DRIVE2", e.data);
  document.getElementById("DRIVE2").innerHTML = e.data;
 }, false);
 
  source.addEventListener('DRIVE3', function(e) {
  console.log("DRIVE3", e.data);
  document.getElementById("DRIVE3").innerHTML = e.data;
 }, false);
 
  source.addEventListener('DRIVE4', function(e) {
  console.log("DRIVE4", e.data);
  document.getElementById("DRIVE4").innerHTML = e.data;
 }, false);
 
}
</script>
</body></html>)rawliteral";

// HTML web page to handle 3 input fields (inputString, TEMP, inputFloat)

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
  Serial.begin(115200);

  Serial.println("ESP8266 INIT");
  Serial.println(F("Inizializing FS..."));

  (LittleFS.begin()) ? Serial.println(F("done.")) : Serial.println(F("fail."));
  /*
  GeneralString = readFile(LittleFS, MyFile);
  for (int j = 0; j < leng; j++)
  {
    if (!GeneralString.indexOf(parametr[j]))
      InitFlag = 1;
  }
  if (InitFlag)
  {
    */
  for (int i = 0; i < leng; i++)
  {
    CurrentSensorState[i] = 0;
    GeneralString += parametr[i] + String(CurrentSensorState[i]);
  }
  /*
    Serial.println("GeneralString succes initialize");
  }
  else
    Serial.println("GeneralString already exist");
*/
  AsyncWiFiManager wifiManager(&server, &dns);
  wifiManager.autoConnect("AutoConnectAP");
  Serial.println("connected...yeey :)");

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor2);
  });
  // Send a GET request to <ESP_IP>/get?ENC=<inputMessage>

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    Serial.print("inputMessage  ");
    Serial.println(inputMessage);

    for (int i = 14; i < leng; i++)
    {
      if (request->hasParam(parametr[i]) && i < leng - 4)
      {
        inputMessage = request->getParam(parametr[i])->value();
        CurrentSensorState[i] = inputMessage.substring(0, 2).toInt();
        SendingValueToString(i, CurrentSensorState[i]);
        /*Serial.print("CurrentSensorState[");
        Serial.print(i);
        Serial.print("]  ");
        Serial.println(CurrentSensorState[i]);*/
        i++;
        CurrentSensorState[i] = inputMessage.substring(3, 5).toInt();
        SendingValueToString(i, CurrentSensorState[i]);
        /*Serial.print("CurrentSensorState[");
        Serial.print(i);
        Serial.print("]  ");
        Serial.println(CurrentSensorState[i]);*/
      }
      else if (request->hasParam(parametr[i]) && i >= leng - 4)
      {
        inputMessage = request->getParam(parametr[i])->value();
        CurrentSensorState[i] = inputMessage.toInt();
        SendingValueToString(i, CurrentSensorState[i]);
        /*Serial.print("CurrentSensorState[");
        Serial.print(i);
        Serial.print("]  ");*/
      }
      if (!Internet_flag)
        Internet_flag = 1;
    }
    Serial.print("Internet_flag  ");
    Serial.println(Internet_flag);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);

  events.onConnect([](AsyncEventSourceClient *client) {
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