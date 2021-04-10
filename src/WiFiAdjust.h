
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <Hash.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>
#include <LITTLEFS.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ssid = "Beeline_2G_F22959";
const char *password = "dUNFcUEP";

//const char* ssid     = "Mironov AG";
//const char* password = "12345678";
const int mat = 165;
String GeneralString;
const char *MyFile = {"/MyFile.txt"};
String TemporaryFile1;
String TemporaryFile2;

const char *parametr[mat] = {"HOUR", "MIN", "TEMP", "LEAK", "LIGHT",
                             "BOBBER1", "BOBBER2", "BOBBER3", "BOBBER4",
                             "SWITCH1", "SWITCH2", "SWITCH3", "SWITCH4",
                             "OBTACLE1", "OBTACLE2", //15

                             "SOLI", "LASER1", "LASER2",
                             "VALVE1", "VALVE2", "VALVE3", "VALVE4", "VALVE5", "VALVE6",
                             "PUMP1", "PUMP2", "PUMP3", "PUMP4", "BIG_PUMP",
                             "DEVICE1", "DEVICE2", "DEVICE3", "DEVICE4", "DEVICE5", "DEVICE6", "DEVICE7", "DEVICE8",
                             "STEP1", "STEP2", "STEP3", "STEP4", "DRIVE1", "DRIVE2", "DRIVE3", "DRIVE4", //30

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
                             "STEP1_hour_on", "STEP1_min_on", "STEP1_hour_off", "STEP1_min_off",
                             "STEP2_hour_on", "STEP2_min_on", "STEP2_hour_off", "STEP2_min_off",
                             "STEP3_hour_on", "STEP3_min_on", "STEP3_hour_off", "STEP3_min_off",
                             "STEP4_hour_on", "STEP4_min_on", "STEP4_hour_off", "STEP4_min_off",
                             "DRIVE1_hour_on", "DRIVE1_min_on", "DRIVE1_hour_off", "DRIVE1_min_off",
                             "DRIVE2_hour_on", "DRIVE2_min_on", "DRIVE2_hour_off", "DRIVE2_min_off",
                             "DRIVE3_hour_on", "DRIVE3_min_on", "DRIVE3_hour_off", "DRIVE3_min_off",
                             "DRIVE4_hour_on", "DRIVE4_min_on", "DRIVE4_hour_off", "DRIVE4_min_off"};

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
.card_top{ 
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
 <div class="card_top">
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
    Obtacle1 %OBTACLE1%    Obtacle2 %OBTACLE2% 
  </form><br>
  
  <form action="/get" target="hidden-form">
    Bobber_1 %BOBBER1% Bobber_2 %BOBBER2% Bobber_3 %BOBBER3% Bobber_4 %BOBBER4% 
  </form><br>
 
  <form action="/get" target="hidden-form">
    Limit switch_1 %SWITCH1%    Limit switch_2 %SWITCH2%    Limit switch_3 %SWITCH3%     Limit switch_4 %SWITCH4% 
  </form><br>
  </div>

   <div class="content">
 <div class="cards">
 <div class="card">
  <form id="red-form" action="/get" target="hidden-form">
    <p>Solinoid state  %SOLI% </p>
    <p> time on %SOLI_hour_on% : %SOLI_min_on% <input type="time" name="SOLI_hour_on">
        <p> time off %SOLI_hour_off% : %SOLI_min_off% <input type="time" name="SOLI_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"></p> 
  </form><br>
  </div>
  
<div class="card">
  <form id="red-form" action="/get" target="hidden-form">
    <p>Laser 1 state %LASER1%  </p>
    <p> time on %LASER1_hour_on% : %LASER1_min_on% <input type="time" name="LASER1_hour_on">
    <p>time off %LASER1_hour_off% : %LASER1_min_off% <input type="time" name="LASER1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>Laser 2 state %LASER2%  </p>
    <p> time on %LASER2_hour_on% : %LASER2_min_on% <input type="time" name="LASER2_hour_on">
    <p>time off %LASER2_hour_off% : %LASER2_min_off% <input type="time" name="LASER2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE1 state %VALVE1%  </p>
    <p> time on %VALVE1_hour_on% : %VALVE1_min_on% <input type="time" name="VALVE1_hour_on">
    <p>time off %VALVE1_hour_off% : %VALVE1_min_off% <input type="time" name="VALVE1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
  </div>
  
<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE2 state %VALVE2%  </p>
    <p> time on %VALVE2_hour_on% : %VALVE2_min_on% <input type="time" name="VALVE2_hour_on">
    <p>time off %VALVE2_hour_off% : %VALVE2_min_off% <input type="time" name="VALVE2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE3 state %VALVE3%  </p>
    <p> time on %VALVE3_hour_on% : %VALVE3_min_on% <input type="time" name="VALVE3_hour_on">
    <p>time off %VALVE3_hour_off% : %VALVE3_min_off% <input type="time" name="VALVE3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE4 state %VALVE4%  </p>
    <p> time on %VALVE4_hour_on% : %VALVE4_min_on% <input type="time" name="VALVE4_hour_on">
    <p>time off %VALVE4_hour_off% : %VALVE4_min_off% <input type="time" name="VALVE4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE5 state %VALVE5%  </p>
    <p> time on %VALVE5_hour_on% : %VALVE5_min_on% <input type="time" name="VALVE5_hour_on">
    <p>time off %VALVE5_hour_off% : %VALVE5_min_off% <input type="time" name="VALVE5_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
 </div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>VALVE6 state %VALVE6%  </p>
    <p> time on %VALVE6_hour_on% : %VALVE6_min_on% <input type="time" name="VALVE6_hour_on">
    <p>time off %VALVE6_hour_off% : %VALVE6_min_off% <input type="time" name="VALVE6_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP1 state %PUMP1%  </p>
    <p> time on %PUMP1_hour_on% : %PUMP1_min_on% <input type="time" name="PUMP1_hour_on">
    <p>time off %PUMP1_hour_off% : %PUMP1_min_off% <input type="time" name="PUMP1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP2 state %PUMP2%  </p>
    <p> time on %PUMP2_hour_on% : %PUMP2_min_on% <input type="time" name="PUMP2_hour_on">
    <p>time off %PUMP2_hour_off% : %PUMP2_min_off% <input type="time" name="PUMP2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP3 state %PUMP3%  </p>
    <p> time on %PUMP3_hour_on% : %PUMP3_min_on% <input type="time" name="PUMP3_hour_on">
    <p>time off %PUMP3_hour_off% : %PUMP3_min_off% <input type="time" name="PUMP3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
   <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>PUMP4 state %PUMP4%  </p>
    <p> time on %PUMP4_hour_on% : %PUMP4_min_on% <input type="time" name="PUMP4_hour_on">
    <p>time off %PUMP4_hour_off% : %PUMP4_min_off% <input type="time" name="PUMP4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE1 state %DEVICE1%  </p>
    <p> time on %DEVICE1_hour_on% : %DEVICE1_min_on% <input type="time" name="DEVICE1_hour_on">
    <p>time off %DEVICE1_hour_off% : %DEVICE1_min_off% <input type="time" name="DEVICE1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE2 state %DEVICE2%  </p>
    <p> time on %DEVICE2_hour_on% : %DEVICE2_min_on% <input type="time" name="DEVICE2_hour_on">
    <p>time off %DEVICE2_hour_off% : %DEVICE2_min_off% <input type="time" name="DEVICE2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE3 state %DEVICE3%  </p>
    <p> time on %DEVICE3_hour_on% : %DEVICE3_min_on% <input type="time" name="DEVICE3_hour_on">
    <p>time off %DEVICE3_hour_off% : %DEVICE3_min_off% <input type="time" name="DEVICE3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>


 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE4 state %DEVICE4%  </p>
    <p> time on %DEVICE4_hour_on% : %DEVICE4_min_on% <input type="time" name="DEVICE4_hour_on">
    <p>time off %DEVICE4_hour_off% : %DEVICE4_min_off% <input type="time" name="DEVICE4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE5 state %DEVICE5%  </p>
    <p> time on %DEVICE5_hour_on% : %DEVICE5_min_on% <input type="time" name="DEVICE5_hour_on">
    <p>time off %DEVICE5_hour_off% : %DEVICE5_min_off% <input type="time" name="DEVICE5_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>


 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE6 state %DEVICE6%  </p>
    <p> time on %DEVICE6_hour_on% : %DEVICE6_min_on% <input type="time" name="DEVICE6_hour_on">
    <p>time off %DEVICE6_hour_off% : %DEVICE6_min_off% <input type="time" name="DEVICE6_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>


  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE7 state %DEVICE7%  </p>
    <p> time on %DEVICE7_hour_on% : %DEVICE7_min_on% <input type="time" name="DEVICE7_hour_on">
    <p>time off %DEVICE7_hour_off% : %DEVICE7_min_off% <input type="time" name="DEVICE7_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

<div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DEVICE8 state %DEVICE8%  </p>
    <p> time on %DEVICE8_hour_on% : %DEVICE8_min_on% <input type="time" name="DEVICE8_hour_on">
    <p>time off %DEVICE8_hour_off% : %DEVICE8_min_off% <input type="time" name="DEVICE8_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>STEP1 state %STEP1%  </p>
    <p> time on %STEP1_hour_on% : %STEP1_min_on% <input type="time" name="STEP1_hour_on">
    <p>time off %STEP1_hour_off% : %STEP1_min_off% <input type="time" name="STEP1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>STEP2 state %STEP2%  </p>
    <p> time on %STEP2_hour_on% : %STEP2_min_on% <input type="time" name="STEP2_hour_on">
    <p>time off %STEP2_hour_off% : %STEP2_min_off% <input type="time" name="STEP2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  
  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>STEP3 state %STEP3%  </p>
    <p> time on %STEP3_hour_on% : %STEP3_min_on% <input type="time" name="STEP3_hour_on">
    <p>time off %STEP3_hour_off% : %STEP3_min_off% <input type="time" name="STEP3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>STEP4 state %STEP4%  </p>
    <p> time on %STEP4_hour_on% : %STEP4_min_on% <input type="time" name="STEP4_hour_on">
    <p>time off %STEP4_hour_off% : %STEP4_min_off% <input type="time" name="STEP4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE1 state %DRIVE1%  </p>
    <p> time on %DRIVE1_hour_on% : %DRIVE1_min_on% <input type="time" name="DRIVE1_hour_on">
    <p>time off %DRIVE1_hour_off% : %DRIVE1_min_off% <input type="time" name="DRIVE1_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  
  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE2 state %DRIVE2%  </p>
    <p> time on %DRIVE2_hour_on% : %DRIVE2_min_on% <input type="time" name="DRIVE2_hour_on">
    <p>time off %DRIVE2_hour_off% : %DRIVE2_min_off% <input type="time" name="DRIVE2_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>

  <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE3 state %DRIVE3%  </p>
    <p> time on %DRIVE3_hour_on% : %DRIVE3_min_on% <input type="time" name="DRIVE3_hour_on">
    <p>time off %DRIVE3_hour_off% : %DRIVE3_min_off% <input type="time" name="DRIVE3_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
  
 <div class="card">
<form id="red-form" action="/get" target="hidden-form">
    <p>DRIVE4 state %DRIVE4%  </p>
    <p> time on %DRIVE4_hour_on% : %DRIVE4_min_on% <input type="time" name="DRIVE4_hour_on">
    <p>time off %DRIVE4_hour_off% : %DRIVE4_min_off% <input type="time" name="DRIVE4_hour_off"></p>
       <p><input type="submit" value="Submit" onclick="submitMessage()"> </p>
  </form><br>
</div>
</div>
 
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

// HTML web page to handle 3 input fields (inputString, TEMP, inputFloat)

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char *path)
{
  //Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if (!file || file.isDirectory())
  {
    // Serial.println("- empty file or failed to open file");
    return String();
  }
  //Serial.println("- read from file:");
  String fileContent;
  while (file.available())
  {
    fileContent += String((char)file.read());
  }
  // Serial.println(fileContent);
  return fileContent;
  //yield();
}

void writeFile(fs::FS &fs, const char *path, String message)
{
  //Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file)
  {
    //Serial.println("- failed to open file for writing");
    //file.flush();
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
  // yield();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  //Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if (!file)
  {
    //Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    // Serial.println("- file written");
  }
  else
  {
    // Serial.println("- write failed");
  }
  // yield();
}

String processor2(const String &var)
{
  for (int j = 0; j < mat; j++)
  {
    if (var == parametr[j])
    {
      TemporaryFile1 = parametr[j];
      TemporaryFile2 = "";
      //Serial.println(TemporaryFile2);
      if (j != 164)
      {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
        {
          TemporaryFile2 += GeneralString[i];
        }
      }
      else if (j == 164)
      {
        for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
        {
          TemporaryFile2 += GeneralString[i];
        }
      }
      //Serial.println(TemporaryFile2);
      return TemporaryFile2;
    }
  }
  return String();
}

String GettingValueFromString(int j)
{
  TemporaryFile1 = parametr[j];
  TemporaryFile2 = "";
  //Serial.println(TemporaryFile2);
  if (j != 164)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
    {
      TemporaryFile2 += GeneralString[i];
    }
  }
  else if (j == 164)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
    {
      TemporaryFile2 += GeneralString[i];
      //Serial.println(TemporaryFile2);
    }
  }
  return TemporaryFile2;
}

void SendingValueToString(int j, int k)
{
  TemporaryFile1 = parametr[j];
  TemporaryFile2 = "";
  //Serial.println(TemporaryFile2);
  if (j != 164)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.indexOf(parametr[j + 1]); i++)
    {
      TemporaryFile2 += GeneralString[i];
    }
  }
  else if (j == 164)
  {
    for (int i = GeneralString.indexOf(parametr[j]) + TemporaryFile1.length(); i < GeneralString.length(); i++)
    {
      TemporaryFile2 += GeneralString[i];
      //Serial.println(TemporaryFile2);
    }
  }
  GeneralString.replace(parametr[j] + TemporaryFile2, parametr[j] + String(k));
}

void WiFiSetup()
{
  Serial.begin(115200);
  //for (int i = 0; i < leng; i++)
  //{
    //GeneralString += parametr[i] + String(CurrentSensorState[i]);
  //}
  Serial.println();

  Serial.println("ESP8266 INIT");
  Serial.println(F("Inizializing FS..."));
  if (LittleFS.begin())
  {
    Serial.println(F("done."));
  }
  else
  {
    Serial.println(F("fail."));
  }

  GeneralString = readFile(LittleFS, MyFile);
  Serial.println(GeneralString);
  Serial.println("CurrentSensorState[i]  ");
  for (int i = 0; i < mat; i++)
  {
    CurrentSensorState[i] = GettingValueFromString(i).toInt();
    Serial.print(CurrentSensorState[i]);
  }
  Serial.println(GeneralString);

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
    request->send_P(200, "text/html", index_html, processor2);
  });
  // Send a GET request to <ESP_IP>/get?ENC=<inputMessage>

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    for (int i = 14; i < mat; i++)
    {
      if (request->hasParam(parametr[i]))
      {
        inputMessage = request->getParam(parametr[i])->value();
        CurrentSensorState[i] = inputMessage.substring(0, 2).toInt();
        SendingValueToString(i, CurrentSensorState[i]);
        i++;
        CurrentSensorState[i] = inputMessage.substring(3, 5).toInt();
        SendingValueToString(i, CurrentSensorState[i]);
        change = i;
        if (!Internet_flag)
          Internet_flag = 1;
      }
    }
    Serial.print("Internet_flag  ");
    Serial.println(Internet_flag);
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