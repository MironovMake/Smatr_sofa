
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <FS.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char *ssid = "Beeline_2G_F22959";
const char *password = "dUNFcUEP";

const char *parametr[]={"MIN","HOUR","TEMP","ENCODER","BUTTON","inputString"};
//String param[]        ={"MIN","HOUR","TEMP","ENCODER","BUTTON"};

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
    Current time %HOUR% : %MIN% 
  </form><br>

  <form action="/get" target="hidden-form">
    Send time %inputString%: <input type="text" name="inputString">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>

<form action="/get" target="hidden-form">
    Encoder %ENCODER%: 
  </form><br>

<form action="/get" target="hidden-form">
    temperature %TEMP%: 
  </form><br>

  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

// HTML web page to handle 3 input fields (inputString, TEMP, inputFloat)

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, String message){
 // Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
   // Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    //Serial.println("- file written");
  } else {
    //Serial.println("- write failed");
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
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
    else if (var == "ENCODER")
    {
        return readFile(SPIFFS, "/ENCODER.txt");
    }
    else if (var =="MIN")
    {
        return readFile(SPIFFS, "/MIN.txt");
    }
    else if (var == "HOUR")
    {
        return readFile(SPIFFS, "/HOUR.txt");
    }
    else if (var == "inputString")
    {
        return readFile(SPIFFS, "/inputString.txt");
    }
    return String();
}

void WiFiSetup(){
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

  Serial.begin(115200);
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
 if (request->hasParam(parametr[5])) {
      inputMessage = request->getParam(parametr[5])->value();
      writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.begin();

  // other adjust
#ifndef ESP8266
  while (!Serial)
    ; // wait for serial port to connect. Needed for native USB
#endif
}