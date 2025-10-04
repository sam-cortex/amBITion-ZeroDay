#define ENABLE_USER_AUTH
#define ENABLE_DATABASE

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>

// Network and Firebase credentials
#define WIFI_SSID "KOLIGE HOUSE 4G"
#define WIFI_PASSWORD "SAVI#3jenu"

#define Web_API_KEY "AIzaSyDpqjicaEHzrPYMbO9VdgM9AI-ksqrLqI0"
#define DATABASE_URL "https://sensor-reading-6a414-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define USER_EMAIL "abcd@gmail.com"
#define USER_PASS "123456"

// User function
void processData(AsyncResult &aResult);

// Authentication
UserAuth user_auth(Web_API_KEY, USER_EMAIL, USER_PASS);

// Firebase components
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// Timer variables for sending data every 10 seconds
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 10000; // 10 seconds in milliseconds

// Variables to send to the database
int intValue ;
float floatValue ;
String stringValue;

const int echo=35;
const int trigger= 27;
#define SOUND_SPEED 0.034//in cm/micro s
long duration;
float distance;

String uid;

void setup(){
 Serial.begin(115200);
 pinMode(echo,INPUT);
 pinMode(trigger,OUTPUT);
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  
  // Configure SSL client
  ssl_client.setInsecure();
  ssl_client.setConnectionTimeout(1000);
  ssl_client.setHandshakeTimeout(5);
  
  // Initialize Firebase
  initializeApp(aClient, app, getAuth(user_auth), processData, "🔐 authTask");
  app.getApp<RealtimeDatabase>(Database);
  Database.url(DATABASE_URL);
  }

void loop(){
  // Maintain authentication and async tasks
  app.loop();
  // Check if authentication is ready
  if (app.ready()){ 
    if(uid.length() == 0){
       uid = app.getUid();
       Serial.print("User UID: ");
       Serial.println(uid);
    }
    // Periodic data sending every 10 seconds
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval){
      // Update the last send time
      lastSendTime = currentTime;

  digitalWrite(trigger,LOW);
  delayMicroseconds(2);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  duration=pulseIn(echo,HIGH);
  distance = duration * SOUND_SPEED/2;
  Serial.print("distance = ");
  Serial.println(distance);
  delay(70);
      String dbPathdist = "/UsersData/" + uid + "/dist";
      Database.set(aClient, dbPathdist, distance, processData, "dist_Send_Float");
    }
  }
}

void processData(AsyncResult &aResult) {
  if (!aResult.isResult())
    return;

  if (aResult.isEvent())
    Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

  if (aResult.isDebug())
    Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

  if (aResult.isError())
    Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

  if (aResult.available()){
    Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    String payload= aResult.c_str();
    if(aResult.uid()=="distGetInt"){
        intValue= payload.toInt();
        Firebase.printf("int value obtained is %d",intValue);
    }
    else if(aResult.uid()=="distGetFloat"){
        floatValue= payload.toFloat();
        Firebase.printf("float value obtained is %f",floatValue);}
    else if(aResult.uid()=="distGetString"){
        stringValue= payload;
        Firebase.printf("string value obtained is %s",stringValue);
    }
    
}}