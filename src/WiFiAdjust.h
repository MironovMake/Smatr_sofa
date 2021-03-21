
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Hash.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ssid = "Beeline_2G_F22959";
const char *password = "dUNFcUEP";
const int mat = 165;

const char *parametr[mat] = {"HOUR", "MIN", "TEMP", "LEAK", "LIGHT",
                             "BOBBER1", "BOBBER2", "BOBBER3", "BOBBER4",
                             "SWITCH1", "SWITCH2", "SWITCH3", "SWITCH4",
                             "OBTACLE1", "OBTACLE2",

                             "SOLI", "LASER1", "LASER2", 
                             "VALVE1", "VALVE2", "VALVE3", "VALVE4", "VALVE5", "VALVE6",
                             "PUMP1", "PUMP2", "PUMP3", "PUMP4","BIG_PUMP",
                             "DEVICE1", "DEVICE2", "DEVICE3", "DEVICE4", "DEVICE5", "DEVICE6", "DEVICE7", "DEVICE8",
                             "STEP1", "STEP2", "STEP3", "STEP4", "DRIVE1", "DRIVE2", "DRIVE3", "DRIVE4",

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
                             "BIG_PUMP_hour_on","BIG_PUMP_min_on","BIG_PUMP_hour_off","BIG_PUMP_min_off",
                             "DEVICE1_hour_on", "DEVICE1_min_on", "DEVICE1_hour_off", "DEVICE1_min_off",
                             "DEVICE2_hour_on", "DEVICE2_min_on", "DEVICE2_hour_off", "DEVICE2_min_off",
                             "DEVICE3_hour_on", "DEVICE3_min_on", "DEVICE3_hour_off", "DEVICE3_min_off",
                             "DEVICE4_hour_on", "DEVICE4_min_on", "DEVICE4_hour_off", "DEVICE4_min_off",
                             "DEVICE5_hour_on", "DEVICE5_min_on", "DEVICE5_hour_off", "DEVICE5_min_off",
                             "DEVICE6_hour_on", "DEVICE6_min_on", "DEVICE6_hour_off", "DEVICE6_min_off",
                             "DEVICE7_hour_on", "DEVICE7_min_on", "DEVICE7_hour_off", "DEVICE7_min_off",
                             "DEVICE8_hour_on", "DEVICE8_min_on", "DEVICE8_hour_off", "DEVICE8_min_off",
                             "STEP1_hour_on", "STEP1_min_on", "STEP1_hour_off", "STEP1_min_off",
                             "STEP2_hour_on", "STEP2_min_on", "STEP2_hour_off", "STEP2_min_off",
                             "STEP3_hour_on", "STEP3_min_on", "STEP3_hour_off", "STEP3_min_off",
                             "STEP4_hour_on", "STEP4_min_on", "STEP4_hour_off", "STEP4_min_off",
                             "DRIVE1_hour_on", "DRIVE1_min_on", "DRIVE1_hour_off", "DRIVE1_min_off",
                             "DRIVE2_hour_on", "DRIVE2_min_on", "DRIVE2_hour_off", "DRIVE2_min_off",
                             "DRIVE3_hour_on", "DRIVE3_min_on", "DRIVE3_hour_off", "DRIVE3_min_off",
                             "DRIVE4_hour_on", "DRIVE4_min_on", "DRIVE4_hour_off", "DRIVE4_min_off"};

const char *files[mat] = {"/HOUR.txt", "/MIN.txt", "/TEMP.txt", "/LEAK.txt", "/LIGHT.txt",
                          "/BOBBER1.txt", "/BOBBER2.txt", "/BOBBER3.txt", "/BOBBER4.txt",
                          "/SWITCH1.txt", "/SWITCH2.txt", "/SWITCH3.txt", "/SWITCH4.txt",
                          "/OBTACLE1.txt", "/OBTACLE2.txt", 

                          "/SOLI.txt", "/LASER1.txt", "/LASER2.txt",
                          "/VALVE1.txt", "/VALVE2.txt", "/VALVE3.txt", "/VALVE4.txt", "/VALVE5.txt", "/VALVE6.txt",
                          "/PUMP1.txt", "/PUMP2.txt", "/PUMP3.txt", "/PUMP4.txt",
                          "/DEVICE1.txt", "/DEVICE2.txt", "/DEVICE3.txt", "/DEVICE4.txt",
                          "/DEVICE5.txt", "/DEVICE6.txt", "/DEVICE7.txt", "/DEVICE8.txt",
                          "/STEP1.txt", "/STEP2.txt", "/STEP3.txt", "/STEP4.txt", "/BIG_PUMP.txt",
                          "/DRIVE1.txt", "/DRIVE2.txt", "/DRIVE3.txt", "/DRIVE4.txt",

                          "/SOLI_hour_on.txt", "/SOLI_min_on.txt", "/SOLI_hour_off.txt", "/SOLI_min_off.txt",
                          "/LASER1_hour_on.txt", "/LASER1_min_on.txt", "/LASER1_hour_off.txt", "/LASER1_min_off.txt",
                          "/LASER2_hour_on.txt", "/LASER2_min_on.txt", "/LASER2_hour_off.txt", "/LASER2_min_off.txt",
                          "/VALVE1_hour_on.txt", "/VALVE1_min_on.txt", "/VALVE1_hour_off.txt", "/VALVE1_min_off.txt",
                          "/VALVE2_hour_on.txt", "/VALVE2_min_on.txt", "/VALVE2_hour_off.txt", "/VALVE2_min_off.txt",
                          "/VALVE3_hour_on.txt", "/VALVE3_min_on.txt", "/VALVE3_hour_off.txt", "/VALVE3_min_off.txt",
                          "/VALVE4_hour_on.txt", "/VALVE4_min_on.txt", "/VALVE4_hour_off.txt", "/VALVE4_min_off.txt",
                          "/VALVE5_hour_on.txt", "/VALVE5_min_on.txt", "/VALVE5_hour_off.txt", "/VALVE5_min_off.txt",
                          "/VALVE6_hour_on.txt", "/VALVE6_min_on.txt", "/VALVE6_hour_off.txt", "/VALVE6_min_off.txt",
                          "/PUMP1_hour_on.txt", "/PUMP1_min_on.txt", "/PUMP1_hour_off.txt", "/PUMP1_min_off.txt",
                          "/PUMP2_hour_on.txt", "/PUMP2_min_on.txt", "/PUMP2_hour_off.txt", "/PUMP2_min_off.txt",
                          "/PUMP3_hour_on.txt", "/PUMP3_min_on.txt", "/PUMP3_hour_off.txt", "/PUMP3_min_off.txt",
                          "/PUMP4_hour_on.txt", "/PUMP4_min_on.txt", "/PUMP4_hour_off.txt", "/PUMP4_min_off.txt",
                          "/BIG_PUMP_hour_on.txt", "/BIG_PUMP_min_on.txt", "/BIG_PUMP_hour_off.txt", "/BIG_PUMP_min_off.txt",
                          "/DEVICE1_hour_on.txt", "/DEVICE1_min_on.txt", "/DEVICE1_hour_off.txt", "/DEVICE1_min_off.txt",
                          "/DEVICE2_hour_on.txt", "/DEVICE2_min_on.txt", "/DEVICE2_hour_off.txt", "/DEVICE2_min_off.txt",
                          "/DEVICE3_hour_on.txt", "/DEVICE3_min_on.txt", "/DEVICE3_hour_off.txt", "/DEVICE3_min_off.txt",
                          "/DEVICE4_hour_on.txt", "/DEVICE4_min_on.txt", "/DEVICE4_hour_off.txt", "/DEVICE4_min_off.txt",
                          "/DEVICE5_hour_on.txt", "/DEVICE5_min_on.txt", "/DEVICE5_hour_off.txt", "/DEVICE5_min_off.txt",
                          "/DEVICE6_hour_on.txt", "/DEVICE6_min_on.txt", "/DEVICE6_hour_off.txt", "/DEVICE6_min_off.txt",
                          "/DEVICE7_hour_on.txt", "/DEVICE7_min_on.txt", "/DEVICE7_hour_off.txt", "/DEVICE7_min_off.txt",
                          "/DEVICE8_hour_on.txt", "/DEVICE8_min_on.txt", "/DEVICE8_hour_off.txt", "/DEVICE8_min_off.txt",
                          "/STEP1_hour_on.txt", "/STEP1_min_on.txt", "/STEP1_hour_off.txt", "/STEP1_min_off.txt",
                          "/STEP2_hour_on.txt", "/STEP2_min_on.txt", "/STEP2_hour_off.txt", "/STEP2_min_off.txt",
                          "/STEP3_hour_on.txt", "/STEP3_min_on.txt", "/STEP3_hour_off.txt", "/STEP3_min_off.txt",
                          "/STEP4_hour_on.txt", "/STEP4_min_on.txt", "/STEP4_hour_off.txt", "/STEP4_min_off.txt",
                          "/DRIVE1_hour_on.txt", "/DRIVE1_min_on.txt", "/DRIVE1_hour_off.txt", "/DRIVE1_min_off.txt",
                          "/DRIVE2_hour_on.txt", "/DRIVE2_min_on.txt", "/DRIVE2_hour_off.txt", "/DRIVE2_min_off.txt",
                          "/DRIVE3_hour_on.txt", "/DRIVE3_min_on.txt", "/DRIVE3_hour_off.txt", "/DRIVE3_min_off.txt",
                          "/DRIVE4_hour_on.txt", "/DRIVE4_min_on.txt", "/DRIVE4_hour_off.txt", "/DRIVE4_min_off.txt"};

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>

<form action="/get" target="hidden-form">
    Current time %HOUR% : %MIN% 
  </form><br>

<form action="/get" target="hidden-form">
    temperature %TEMP% 
  </form><br>

  <form action="/get" target="hidden-form">
    Water leaks %LEAK% 
  </form><br>

  <form action="/get" target="hidden-form">
    Light sensor %LIGHT% 
  </form><br>
  
  <form action="/get" target="hidden-form">
    Obtacle1 %OBTACLE1% 
  </form><br>
  
  <form action="/get" target="hidden-form">
    Obtacle2 %OBTACLE2% 
  </form><br>
  
  <form action="/get" target="hidden-form">
    Bobber_1 %BOBBER1% Bobber_2 %BOBBER2% Bobber_3 %BOBBER3% Bobber_4 %BOBBER4% 
  </form><br>
 
  <form action="/get" target="hidden-form">
    Limit switch_1 %SWITCH1% Limit switch_2 %SWITCH2% Limit switch_3 %SWITCH3% Limit switch_4 %SWITCH4% 
  </form><br>

  <form action="/get" target="hidden-form">
    Solinoid Hour on  %SOLI% %SOLI_hour_on%: <input type="text" name="SOLI_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %SOLI_min_on%: <input type="text" name="SOLI_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %SOLI_hour_off%: <input type="text" name="SOLI_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %SOLI_min_off%: <input type="text" name="SOLI_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    Laser 1 Hour on %LASER1% %LASER1_hour_on%: <input type="text" name="LASER1_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %LASER1_min_on%: <input type="text" name="LASER1_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %LASER1_hour_off%: <input type="text" name="LASER1_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %LASER1_min_off%: <input type="text" name="LASER1_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    Laser 2 Hour on %LASER2% %LASER2_hour_on%: <input type="text" name="LASER2_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %LASER2_min_on%: <input type="text" name="LASER2_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %LASER2_hour_off%: <input type="text" name="LASER2_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %LASER2_min_off%: <input type="text" name="LASER2_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

<form action="/get" target="hidden-form">
    Valve 1 Hour on %VALVE1% %VALVE1_hour_on%: <input type="text" name="VALVE1_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %VALVE1_min_on%: <input type="text" name="VALVE1_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %VALVE1_hour_off%: <input type="text" name="VALVE1_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %VALVE1_min_off%: <input type="text" name="VALVE1_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

<form action="/get" target="hidden-form">
    VALVE 2 Hour on %VALVE2% %VALVE2_hour_on%: <input type="text" name="VALVE2_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %VALVE2_min_on%: <input type="text" name="VALVE2_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %VALVE2_hour_off%: <input type="text" name="VALVE2_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %VALVE2_min_off%: <input type="text" name="VALVE2_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    VALVE 3 Hour on %VALVE3% %VALVE3_hour_on%: <input type="text" name="VALVE3_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %VALVE3_min_on%: <input type="text" name="VALVE3_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %VALVE3_hour_off%: <input type="text" name="VALVE3_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %VALVE3_min_off%: <input type="text" name="VALVE3_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    VALVE 4 Hour on %VALVE4% %VALVE4_hour_on%: <input type="text" name="VALVE4_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %VALVE4_min_on%: <input type="text" name="VALVE4_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %VALVE4_hour_off%: <input type="text" name="VALVE4_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %VALVE4_min_off%: <input type="text" name="VALVE4_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

   <form action="/get" target="hidden-form">
    VALVE 5 Hour on %VALVE5% %VALVE5_hour_on%: <input type="text" name="VALVE5_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %VALVE5_min_on%: <input type="text" name="VALVE5_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %VALVE5_hour_off%: <input type="text" name="VALVE5_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %VALVE5_min_off%: <input type="text" name="VALVE5_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

   <form action="/get" target="hidden-form">
    VALVE 6 Hour on %VALVE6% %VALVE6_hour_on%: <input type="text" name="VALVE6_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %VALVE6_min_on%: <input type="text" name="VALVE6_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %VALVE6_hour_off%: <input type="text" name="VALVE6_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %VALVE6_min_off%: <input type="text" name="VALVE6_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    PUMP 1 Hour on %PUMP1% %PUMP1_hour_on%: <input type="text" name="PUMP1_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %PUMP1_min_on%: <input type="text" name="PUMP1_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %PUMP1_hour_off%: <input type="text" name="PUMP1_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %PUMP1_min_off%: <input type="text" name="PUMP1_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    PUMP 2 Hour on %PUMP2% %PUMP2_hour_on%: <input type="text" name="PUMP2_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %PUMP2_min_on%: <input type="text" name="PUMP2_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %PUMP2_hour_off%: <input type="text" name="PUMP2_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %PUMP2_min_off%: <input type="text" name="PUMP2_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    PUMP 3 Hour on %PUMP3% %PUMP3_hour_on%: <input type="text" name="PUMP3_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %PUMP3_min_on%: <input type="text" name="PUMP3_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %PUMP3_hour_off%: <input type="text" name="PUMP3_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %PUMP3_min_off%: <input type="text" name="PUMP3_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    PUMP 4 Hour on %PUMP4% %PUMP4_hour_on%: <input type="text" name="PUMP4_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %PUMP4_min_on%: <input type="text" name="PUMP4_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %PUMP4_hour_off%: <input type="text" name="PUMP4_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %PUMP4_min_off%: <input type="text" name="PUMP4_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 1 Hour on %DEVICE1% %DEVICE1_hour_on%: <input type="text" name="DEVICE1_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE1_min_on%: <input type="text" name="DEVICE1_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE1_hour_off%: <input type="text" name="DEVICE1_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE1_min_off%: <input type="text" name="DEVICE1_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 2 Hour on %DEVICE2% %DEVICE2_hour_on%: <input type="text" name="DEVICE2_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE2_min_on%: <input type="text" name="DEVICE2_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE2_hour_off%: <input type="text" name="DEVICE2_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE2_min_off%: <input type="text" name="DEVICE2_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 3 Hour on %DEVICE3% %DEVICE3_hour_on%: <input type="text" name="DEVICE3_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE3_min_on%: <input type="text" name="DEVICE3_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE3_hour_off%: <input type="text" name="DEVICE3_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE3_min_off%: <input type="text" name="DEVICE3_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 4 Hour on %DEVICE4% %DEVICE4_hour_on%: <input type="text" name="DEVICE4_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE4_min_on%: <input type="text" name="DEVICE4_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE4_hour_off%: <input type="text" name="DEVICE4_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE4_min_off%: <input type="text" name="DEVICE4_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 5 Hour on %DEVICE5% %DEVICE5_hour_on%: <input type="text" name="DEVICE5_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE5_min_on%: <input type="text" name="DEVICE5_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE5_hour_off%: <input type="text" name="DEVICE5_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE5_min_off%: <input type="text" name="DEVICE5_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 6 Hour on %DEVICE6% %DEVICE6_hour_on%: <input type="text" name="DEVICE6_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE6_min_on%: <input type="text" name="DEVICE6_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE6_hour_off%: <input type="text" name="DEVICE6_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE6_min_off%: <input type="text" name="DEVICE6_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 7 Hour on %DEVICE7% %DEVICE7_hour_on%: <input type="text" name="DEVICE7_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE7_min_on%: <input type="text" name="DEVICE7_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE7_hour_off%: <input type="text" name="DEVICE7_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE7_min_off%: <input type="text" name="DEVICE7_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    DEVICE 8 Hour on %DEVICE8% %DEVICE8_hour_on%: <input type="text" name="DEVICE8_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DEVICE8_min_on%: <input type="text" name="DEVICE8_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DEVICE8_hour_off%: <input type="text" name="DEVICE8_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DEVICE8_min_off%: <input type="text" name="DEVICE8_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    STEP 1 Hour on %STEP1% %STEP1_hour_on%: <input type="text" name="STEP1_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %STEP1_min_on%: <input type="text" name="STEP1_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %STEP1_hour_off%: <input type="text" name="STEP1_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %STEP1_min_off%: <input type="text" name="STEP1_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>

  <form action="/get" target="hidden-form">
    STEP 2 Hour on %STEP2% %STEP2_hour_on%: <input type="text" name="STEP2_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %STEP2_min_on%: <input type="text" name="STEP2_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %STEP2_hour_off%: <input type="text" name="STEP2_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %STEP2_min_off%: <input type="text" name="STEP2_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <form action="/get" target="hidden-form">
    STEP 3 Hour on %STEP3% %STEP3_hour_on%: <input type="text" name="STEP3_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %STEP3_min_on%: <input type="text" name="STEP3_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %STEP3_hour_off%: <input type="text" name="STEP3_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %STEP3_min_off%: <input type="text" name="STEP3_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <form action="/get" target="hidden-form">
    STEP 4 Hour on %STEP4% %STEP4_hour_on%: <input type="text" name="STEP4_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %STEP4_min_on%: <input type="text" name="STEP4_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %STEP4_hour_off%: <input type="text" name="STEP4_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %STEP4_min_off%: <input type="text" name="STEP4_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <form action="/get" target="hidden-form">
    DRIVE 1 Hour on %DRIVE1% %DRIVE1_hour_on%: <input type="text" name="DRIVE1_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DRIVE1_min_on%: <input type="text" name="DRIVE1_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DRIVE1_hour_off%: <input type="text" name="DRIVE1_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DRIVE1_min_off%: <input type="text" name="DRIVE1_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <form action="/get" target="hidden-form">
    DRIVE 2 Hour on %DRIVE2% %DRIVE2_hour_on%: <input type="text" name="DRIVE2_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DRIVE2_min_on%: <input type="text" name="DRIVE2_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DRIVE2_hour_off%: <input type="text" name="DRIVE2_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DRIVE2_min_off%: <input type="text" name="DRIVE2_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <form action="/get" target="hidden-form">
    DRIVE 3 Hour on %DRIVE3% %DRIVE3_hour_on%: <input type="text" name="DRIVE3_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DRIVE3_min_on%: <input type="text" name="DRIVE3_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DRIVE3_hour_off%: <input type="text" name="DRIVE3_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DRIVE3_min_off%: <input type="text" name="DRIVE3_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <form action="/get" target="hidden-form">
    DRIVE 4 Hour on %DRIVE4% %DRIVE4_hour_on%: <input type="text" name="DRIVE4_hour_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Min on %DRIVE4_min_on%: <input type="text" name="DRIVE4_min_on">
      <input type="submit" value="Submit" onclick="submitMessage()"> 
       </form><br>
  <form action="/get" target="hidden-form">
        Hour off %DRIVE4_hour_off%: <input type="text" name="DRIVE4_hour_off">
      <input type="submit" value="Submit" onclick="submitMessage()">
       </form><br>
  <form action="/get" target="hidden-form">
         Min off %DRIVE4_min_off%: <input type="text" name="DRIVE4_min_off"> 
      <input type="submit" value="Submit" onclick="submitMessage()"> 
  </form><br>
  
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

// HTML web page to handle 3 input fields (inputString, TEMP, inputFloat)

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char *path, String message)
{
  // Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file)
  {
    // Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    //Serial.println("- file written");
  }
  else
  {
    //Serial.println("- write failed");
  }
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
}
// Replaces placeholder with stored values
String processor(const String &var)
{
  for (int i = 0; i < mat; i++)
  {
    if (var == parametr[i])
    {
      return readFile(SPIFFS, files[i]);
    }
  }
  return String();
}

void WiFiSetup()
{
  Serial.begin(115200);
  Serial.println("ESP8266 INIT");
#ifdef ESP32
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
#else
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi Failed!");
    return;
  }

  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  // Send a GET request to <ESP_IP>/get?ENC=<inputMessage>

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;

    for (int i = 14; i < mat; i++)
    {
      if (request->hasParam(parametr[i]))
      {
        inputMessage = request->getParam(parametr[i])->value();
        writeFile(SPIFFS, files[i], inputMessage.c_str());
      }
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();

  // other adjust
#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif
}