/*
  SDWebServer - Example WebServer with SD Card backend for esp8266

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Have a FAT Formatted SD Card connected to the SPI port of the ESP8266
  The web root is the SD Card root folder
  File extensions with more than 3 charecters are not supported by the SD Library
  File Names longer than 8 charecters will be truncated by the SD library, so keep filenames shorter
  index.htm is the default index (works on subfolders as well)

  upload the contents of SdRoot to the root of the SDcard and access the editor by going to http://esp8266sd.local/edit

*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <SD.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DBG_OUTPUT_PORT Serial

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* APssid = "MicroNas";
const char* APpassword = "qwerty123";
const char* ssid = "YITNO";
const char* password = "radityaptr";
const char* host = "esp8266sd";

// 'linkzoledsudah', 128x32px
const unsigned char lzelinkzoledsudah [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xe7, 0xff, 0xfc, 0xc1, 0x98, 0x44, 0x6f, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0xbf, 0xff, 0xc7, 0xff, 0xfe, 0xc1, 0x1c, 0x4c, 0xcf, 0xc0, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0xbf, 0xff, 0xc7, 0xff, 0xfc, 0xc3, 0x1c, 0xcd, 0xc1, 0x80, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0xbf, 0xff, 0xcf, 0xff, 0xfc, 0x83, 0x34, 0xcd, 0x83, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x80, 0x0f, 0x8f, 0x80, 0x01, 0x83, 0x36, 0xcf, 0x06, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0x1f, 0x0f, 0x80, 0x01, 0x83, 0x36, 0x8f, 0x0e, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x0f, 0x00, 0x3e, 0x0f, 0x80, 0x01, 0x82, 0x22, 0x9b, 0x1c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0x00, 0x7e, 0x0f, 0x00, 0x01, 0x86, 0x63, 0x99, 0x98, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0x00, 0xfc, 0x1f, 0x00, 0x03, 0xe6, 0x63, 0x99, 0xbf, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1f, 0x01, 0xf8, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1e, 0x03, 0xf0, 0x1f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x1e, 0x03, 0xe0, 0x1f, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3e, 0x07, 0xc0, 0x1f, 0xff, 0xe0, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3e, 0x0f, 0xc0, 0x3e, 0x00, 0x01, 0x80, 0xe1, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3e, 0x1f, 0x80, 0x3e, 0x00, 0x01, 0x80, 0xa1, 0x8c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3c, 0x3f, 0x00, 0x3e, 0x00, 0x01, 0x81, 0xb1, 0x8c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00, 0x3c, 0x00, 0x01, 0x81, 0xb1, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x7c, 0xfc, 0x00, 0x7c, 0x00, 0x01, 0x81, 0x19, 0xf8, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x7c, 0xf8, 0x00, 0x7c, 0x00, 0x01, 0x83, 0xf9, 0x8c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x79, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x83, 0x19, 0x8c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x79, 0xff, 0xff, 0x7f, 0xff, 0xe1, 0xfb, 0x0d, 0xfc, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x7b, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xfe, 0x0d, 0xf0, 0x80, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x10, 0x22, 0x88, 0x48, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0xff, 0xff, 0x7f, 0xff, 0xff, 0xc0, 0x08, 0x22, 0x44, 0x2a, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x01, 0xff, 0xff, 0x7f, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

ESP8266WebServer server(80);

static bool hasSD = false;
File uploadFile;


void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.htm";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";

  File dataFile = SD.open(path.c_str());
  if(dataFile.isDirectory()){
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile)
    return false;

  if (server.hasArg("download")) dataType = "application/octet-stream";

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
    DBG_OUTPUT_PORT.println("Sent less data than expected!");
  }

  dataFile.close();
  return true;
}

void handleFileUpload(){
  if(server.uri() != "/edit") return;
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    if(SD.exists((char *)upload.filename.c_str())) SD.remove((char *)upload.filename.c_str());
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);
    DBG_OUTPUT_PORT.print("Upload: START, filename: "); DBG_OUTPUT_PORT.println(upload.filename);
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(uploadFile) uploadFile.write(upload.buf, upload.currentSize);
    DBG_OUTPUT_PORT.print("Upload: WRITE, Bytes: "); DBG_OUTPUT_PORT.println(upload.currentSize);
  } else if(upload.status == UPLOAD_FILE_END){
    if(uploadFile) uploadFile.close();
    DBG_OUTPUT_PORT.print("Upload: END, Size: "); DBG_OUTPUT_PORT.println(upload.totalSize);
  }
}

void deleteRecursive(String path){
  File file = SD.open((char *)path.c_str());
  if(!file.isDirectory()){
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while(true) {
    File entry = file.openNextFile();
    if (!entry) break;
    String entryPath = path + "/" +entry.name();
    if(entry.isDirectory()){
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if(path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate(){
  if(server.args() == 0) return returnFail("BAD ARGS");
  String path = server.arg(0);
  if(path == "/" || SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if(path.indexOf('.') > 0){
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if(file){
      file.write((const char *)0);
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void printDirectory() {
  if(!server.hasArg("dir")) return returnFail("BAD ARGS");
  String path = server.arg("dir");
  if(path != "/" && !SD.exists((char *)path.c_str())) return returnFail("BAD PATH");
  File dir = SD.open((char *)path.c_str());
  path = String();
  if(!dir.isDirectory()){
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry)
    break;

    String output;
    if (cnt > 0)
      output = ',';

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
 }
 server.sendContent("]");
 dir.close();
}

void handleNotFound(){
  if(hasSD && loadFromSdCard(server.uri())) return;
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  DBG_OUTPUT_PORT.print(message);
}

void setup(void){
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.drawBitmap(0, 0,  lzelinkzoledsudah, 128, 32, WHITE);// menggambar logo
  display.display();
  delay(2000);
  display.clearDisplay();

  display.display();
  
  DBG_OUTPUT_PORT.begin(115200);
  DBG_OUTPUT_PORT.setDebugOutput(true);
  DBG_OUTPUT_PORT.print("\n");
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(APssid, APpassword);
  WiFi.begin(ssid, password);
  DBG_OUTPUT_PORT.print("Connecting to ");
  DBG_OUTPUT_PORT.println(ssid);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Connecting to ");
  display.println(ssid);
  display.println("Copyright (c)");
  display.println("2021 Rivaldi");
  display.display();
  delay(1000);
  display.clearDisplay();

  // Wait for connection
  uint8_t i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 20) {//wait 10 seconds
    delay(500);
  }
  DBG_OUTPUT_PORT.print("Connected! IP address: ");
  DBG_OUTPUT_PORT.println(WiFi.localIP());
  IPAddress IP = WiFi.softAPIP();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Connected! IP address: ");
  display.println(WiFi.localIP());
  display.println(IP);
  display.display();
  delay(1000);
  display.clearDisplay();
  server.begin();

  if (MDNS.begin(host)) {
    MDNS.addService("http", "tcp", 80);
    DBG_OUTPUT_PORT.println("MDNS responder started");
    DBG_OUTPUT_PORT.print("You can now connect to http://");
    DBG_OUTPUT_PORT.print(host);
    DBG_OUTPUT_PORT.println(".local");
  }
  
  server.on("/list", HTTP_GET, printDirectory);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, [](){ returnOK(); }, handleFileUpload);
  server.onNotFound(handleNotFound);

  server.begin();
  DBG_OUTPUT_PORT.println("HTTP server started");

  if (SD.begin(SS)){
     DBG_OUTPUT_PORT.println("SD Card initialized.");
     hasSD = true;
  }
}

void loop(void){
  server.handleClient();
}
