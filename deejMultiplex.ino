#include <Keyboard.h>

const int NUM_SLIDERS=6;
int analogInputs[NUM_SLIDERS];
int analogSliderValues[NUM_SLIDERS];
int multiplex[4]={2,3,4,5};   //pin numbers the multiplexer is connected to
bool pause=false;

void setup(){
  //initialise multiplexer pins
  for(int x=0;x<4;x++){
    pinMode(multiplex[x],OUTPUT);
  }

  for(int i=0;i<NUM_SLIDERS;i++){
    analogInputs[i]=i;
  }
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  updateSliderValues();
  if(analogSliderValues[0]==0&&pause==false){
    Keyboard.press(KEY_F13);
    Keyboard.releaseAll();
    pause=true;
  }
  if(analogSliderValues[0]>0&&pause==true){
    Keyboard.press(KEY_F13);
    Keyboard.releaseAll();
    pause=false;
  }

  sendSliderValues(); // Actually send data (all the time)
  //printSliderValues(); // For debug
  //delay(10); delay should be irrrelivent becase using the multiplexer takes more time
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    writeMultiplex(analogInputs[i]);
    analogSliderValues[i] = analogRead(A0);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

//switches which pin the multiplexer routes too
void writeMultiplex(int num){
  for(int i=0;i<4;i++){
    if((num&(1<<i))>0){
      digitalWrite(multiplex[i],HIGH);
    }
    else{
      digitalWrite(multiplex[i],LOW);
    }
  }
}