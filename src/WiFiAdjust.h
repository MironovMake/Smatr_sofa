
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#else
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ssid = "Beeline_2G_F22959";
const char *password = "dUNFcUEP";

const char *parametr[]={"MIN","HOUR","TEMP","ENCODER","BUTTON"};
String param[]={"MIN","HOUR","TEMP","ENCODER","BUTTON"};

/*
sensor[0] minute
sensor[1] houre
sensor[2] temperature
sensor[3] encoder
sensor[4] button
*/
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
    Current time %MIN%:%HOUR% 
  </form><br>

  <form action="/get" target="hidden-form">
    inputString (current value %inputString%): <input type="text" name="inputString">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>

<form action="/get" target="hidden-form">
    Encoder %Encoder%: 
  </form><br>

<form action="/get" target="hidden-form">
    TEMP %TEMP%: 
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
    //Serial.printf("Reading file: %s\r\n", path);
    File file = fs.open(path, "r");
    if (!file || file.isDirectory())
    {
        //Serial.println("- empty file or failed to open file");
        return String();
    }
    //Serial.println("- read from file:");
    String fileContent;
    while (file.available())
    {
        fileContent += String((char)file.read());
    }
    //Serial.println(fileContent);
    return fileContent;
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\r\n", path);
    File file = fs.open(path, "w");
    if (!file)
    {
        //Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        //Serial.println("- write failed");
    }
}

void writeFile(fs::FS &fs, const char *path, String message)
{
    //Serial.printf("Writing file: %s\r\n", path);
    File file = fs.open(path, "w");
    if (!file)
    {
       // Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
       // Serial.println("- file written");
    }
    else
    {
        //Serial.println("- write failed");
    }
}

// Replaces placeholder with stored values
String processor(const String &var)
{
    //Serial.println(var);
    if (var == "TEMP")
    {
        return readFile(SPIFFS, "/TEMP.txt");
    }
    else if (var == "Encoder")
    {
        return readFile(SPIFFS, "/Encoder.txt");
    }
    else if (var =="MIN")
    {
        return readFile(SPIFFS, "/MIN.txt");
    }
    else if (var == "HOUR")
    {
        return readFile(SPIFFS, "/HOUR.txt");
    }
    return String();
}
