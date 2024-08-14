//#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "DHT.h"
#ifndef STASSID
#define STASSID "TECNO POVA Neo"
#define STAPSK  "123456789"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;
#define LED 2       //On board LED
#define DHTTYPE DHT11 // DHT 11
uint8_t DHTPin = D1;
DHT dht(DHTPin, DHTTYPE);
float humidity, temperature;

const int Rainsensor=D3;
int Rainpin=0;
String Rain;
String N="None", R="Raining";

ESP8266WebServer server(80);
const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
  <title>Weather Station IOT</title>
  <h1 style="text-align:center; color:green;">Weather Station USing Internet Of Things</h1>
    <h4 style="text-align: center;"> By </h4>

  <h3 style="text-align:center;">LUMUN, C.T. (16/37176/UE) AND AGBO, B. (16/37125/UE)</h3>
      <h4 style="text-align: center; color: blue;""> <i> (Department of Electrical/Electronic Engineering, JOSTUM)</h4>

    <h4 style="text-align: center;"> <i> Supervisor:</i> Engr. Dr. D. Agbo</h4>
  <style>
 canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }
  /* Data Table Styling*/ 
  #dataTable {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
    text-align: center;
  }
  #dataTable td, #dataTable th {
    border: 1px solid #ddd;
    padding: 8px;
  }
  #dataTable tr:nth-child(even){background-color: #f2f2f2;}
  #dataTable tr:hover {background-color: #ddd;}
  #dataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: center;
    background-color: #050505;
    color: white;
  }
 </style>
</head>
<body>
<div>
  <table id="dataTable">
    <tr><th>Time</th><th>Temperature(<span>&#8451;</span>)</th><th>Humidity (%)</th><th>Rain_Status</th></tr>
  </table>
</div>
<br>
<br>  
<script>
var Tvalues = [];
var Hvalues = [];
var Rvalues = [];
var timeStamp = [];
setInterval(function() {
  // Call a function repetatively with 5 Second interval
  getData();
}, 5000); //5000mSeconds update rate
 function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
  var time = new Date().toLocaleTimeString();
  var txt = this.responseText;
  var obj = JSON.parse(txt); 
      Tvalues.push(obj.Temperature);
      Hvalues.push(obj.Humidity);
            Rvalues.push(obj.Rain_Status);

 timeStamp.push(time);
  //Update Data Table
    var table = document.getElementById("dataTable");
    var row = table.insertRow(1); //Add after headings
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
        var cell4 = row.insertCell(3);

    cell1.innerHTML = time;
    cell2.innerHTML = obj.Temperature;
    cell3.innerHTML = obj.Humidity;
        cell4.innerHTML = obj.Rain_Status;

    }
  };
  xhttp.open("GET", "readData", true); //Handle readData server on ESP8266
  xhttp.send();
}
</script>
</body>
</html>
)=====";
void handleRoot()
{
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
void readData()
{
String data = "{\"Temperature\":\""+ String(temperature) +"\", \"Humidity\":\""+ String(humidity) +"\", \"Rain_Status\":\""+ String(Rain) +"\"}";
digitalWrite(LED,!digitalRead(LED)); 
server.send(200, "text/plane", data); 
delay(2000);
temperature = dht.readTemperature(); 
humidity = dht.readHumidity(); 
Rainpin=digitalRead(Rainsensor);

 Serial.print(humidity, 1);
 Serial.print(temperature, 1);

 if(Rainpin==HIGH)
 {
  Rain=N;
 }
 else
 {
  Rain=R;
 }
 
}

void setup(void) {
  Serial.begin(115200);
pinMode(DHTPin, INPUT);
pinMode(Rainsensor, INPUT);
dht.begin();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

server.on("/", handleRoot);     
server.on("/readData", readData); 
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
