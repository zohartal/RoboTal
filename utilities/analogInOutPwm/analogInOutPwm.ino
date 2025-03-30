const int AIN = A0;
const int AOUT = 9;
int in;
int out;

void setup() {
    Serial.begin(9600);
    Serial.println("Begin.");  
    pinMode(AIN, INPUT);
    pinMode(AOUT, OUTPUT);

}

void loop() {
      in = analogRead(AIN);
      out = map(in,0,1023,0,255);
      analogWrite(AOUT, out);

      Serial.print("in=");      
      Serial.print(in);
      Serial.print("out=");
      Serial.println(out);
      delay(250);    
}
