#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Pins Configuration
int IN1 = 12, IN2 = 13, IN3 = 14, IN4 = 27;
int trigPin = 5, echoPin = 18, servoPin = 19;

// Speed Settings
int maxSpeed = 150; 
WebServer server(80);
Servo myServo;
bool autoMode = false;

// HTML with No selectable text and Symbols
const char* html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no'><style>body{background:#222;color:white;font-family:sans-serif;text-align:center;-webkit-user-select:none;user-select:none;touch-action:none;}button{width:90px;height:90px;font-size:40px;margin:5px;border-radius:20px;background:#4CAF50;color:white;border:none;touch-action:manipulation;cursor:pointer;outline:none;}.mode-btn{width:140px;height:60px;font-size:18px;background:#f44336;margin-top:20px}</style></head><body><h1>Ankit Smart Car</h1><div><button onmousedown='f(\"/F\")' onmouseup='f(\"/S\")' ontouchstart='f(\"/F\")' ontouchend='f(\"/S\")'>&#9650;</button><br><button onmousedown='f(\"/L\")' onmouseup='f(\"/S\")' ontouchstart='f(\"/L\")' ontouchend='f(\"/S\")'>&#9664;</button><button onclick='f(\"/S\")'>&#9209;</button><button onmousedown='f(\"/R\")' onmouseup='f(\"/S\")' ontouchstart='f(\"/R\")' ontouchend='f(\"/S\")'>&#9654;</button><br><button onmousedown='f(\"/B\")' onmouseup='f(\"/S\")' ontouchstart='f(\"/B\")' ontouchend='f(\"/S\")'>&#9660;</button></div><hr><h3>Control Mode</h3><button class='mode-btn' onclick='f(\"/AUTO_ON\")'>AUTO ON</button><button class='mode-btn' style='background:#2196F3' onclick='f(\"/AUTO_OFF\")'>MANUAL</button><script>function f(path){fetch(path);}</script></body></html>";

void stopCar() { analogWrite(IN1, 0); analogWrite(IN2, 0); analogWrite(IN3, 0); analogWrite(IN4, 0); }

void moveSmoothly(int s1, int s2, int s3, int s4) {
  for (int i = 0; i <= maxSpeed; i += 25) { 
    analogWrite(IN1, s1 ? i : 0); analogWrite(IN2, s2 ? i : 0);
    analogWrite(IN3, s3 ? i : 0); analogWrite(IN4, s4 ? i : 0);
    delay(10); 
  }
}

void forward() { moveSmoothly(1, 0, 1, 0); }
void backward() { moveSmoothly(0, 1, 0, 1); }
void left() { moveSmoothly(1, 0, 0, 1); }
void right() { moveSmoothly(0, 1, 1, 0); }

int getDistance() {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int d = duration * 0.034 / 2;
  return (d == 0) ? 400 : d;
}

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);
  myServo.setPeriodHertz(50); myServo.attach(servoPin, 500, 2400); myServo.write(90);
  WiFi.softAP("Ankit_Smart_Car", "12345678");
  WiFi.setSleep(false); 
  server.on("/", []() { server.send(200, "text/html", html); });
  server.on("/F", forward); server.on("/B", backward);
  server.on("/L", left); server.on("/R", right); server.on("/S", stopCar);
  server.on("/AUTO_ON", []() { autoMode = true; server.send(200); });
  server.on("/AUTO_OFF", []() { autoMode = false; stopCar(); server.send(200); });
  server.begin();
  stopCar();
}

void loop() {
  server.handleClient();
  if (autoMode) {
    int distance = getDistance();
    if (distance <= 12) {
      stopCar(); delay(400); backward(); delay(400); stopCar();
      myServo.write(170); delay(600); int dL = getDistance();
      myServo.write(10); delay(600); int dR = getDistance();
      myServo.write(90); delay(400);
      if (dL >= dR) { left(); delay(500); } else { right(); delay(500); }
      stopCar();
    } else { analogWrite(IN1, maxSpeed); analogWrite(IN3, maxSpeed); }
  }
}