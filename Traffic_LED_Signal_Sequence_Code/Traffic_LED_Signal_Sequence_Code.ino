const int greenLedPin = 8;
const int redLedPin = 9;
const int yellowLedPin = 10;
const int trigPin = 2;
const int echoPin = 3;
const int five = 5;
const int whitePin = 11;


const int thresholdDistance = 28;

long duration;
int distance;


void setup() {

  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(five,OUTPUT);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(greenLedPin, HIGH);
  digitalWrite(redLedPin, LOW);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(whitePin, LOW);
  digitalWrite(five,HIGH);
  Serial.begin(9600);
}

void loop() {
  
  digitalWrite(greenLedPin, HIGH);
  delay(2000); 
  digitalWrite(greenLedPin, LOW);
  digitalWrite(yellowLedPin, HIGH);
  delay(2000);
  digitalWrite(yellowLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
  int a = 0;
  while(a<2000){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    Serial.println(duration);
    distance = duration * 0.034 / 2;
    // digitalWrite(trigPin, LOW);

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    if (distance < thresholdDistance) {
      Serial.println("captureSignal");
      Serial.println(distance);
      digitalWrite(whitePin, HIGH);
      delay(100);
      digitalWrite(whitePin, LOW);
      
    }
    else{
      delay(100);
    }
    a+=12;
  }
  digitalWrite(redLedPin, LOW);

}