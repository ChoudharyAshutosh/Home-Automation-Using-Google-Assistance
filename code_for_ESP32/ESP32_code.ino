#include<WiFi.h>
#include<WiFiClient.h>
#include<PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

void  setup_wifi(){
  delay(10);
  Serial.println();
  Serial.println("Connecting to ....");
  Serial.println("your_wifi_ssid");  //ssid is hotspot name i renamed my hotspot with 'your_wifi_ssid'
  WiFi.begin("your_wifi_ssid","your_wifi_password");  //ssid is hotspot name i renamed my hotspot with 'your_wifi_password'
  while(WiFi.status()!= WL_CONNECTED){
    delay(500);Serial.print(".");
   }
  randomSeed(micros());
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP()); 
}
void callback(char* topic, byte* payload, unsigned int length){
  Serial.print("Message Arrived[");
  Serial.print(topic);
  Serial.print("] : ");
  for(int i=0;i<length;i++)
  Serial.print((char)payload[i]);
  Serial.println();
  if((char)payload[0]=='a'){  //here 'a' means On
  digitalWrite(5,HIGH);
  Serial.println("a");
  }
  else if((char)payload[0]=='b'){     //here 'b' means Off
  digitalWrite(5,LOW);
  Serial.println("b");  }
    }
void reconnect(){
  while(!client.connected()){
    Serial.println("Attemting MQTT connection...");
    String clientid="NodeMCU-ESP32";
if(client.connect(clientid.c_str(),"Test", 0,0,"Mqtt disconnected") )
{ 
  Serial.println("connected");
        client.publish("Test","MQTT SUCCESSFULLY CONNECTED",true);
       client.subscribe("Test");
  } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
     
}
  }
void setup() {
  Serial.begin(115200);
  pinMode(5,OUTPUT);
  setup_wifi();
client.setServer("mqtt.fluux.io",1883);  //'mqtt.fluux.io' is mqtt host
client.setCallback(callback);
}

void loop() {
  if(!client.connected())
  reconnect();
  client.loop();
}