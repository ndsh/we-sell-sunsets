/*
todo:
- gradient map die die dichte der atmosphäre wiederspiegelt
*/

RGBLED led;
String[] colors2 = {

"0,0,12,0",
"2,1,17,0",
"2,1,17,0",
"2,1,17,0",
"32,32,44,0.5",
"64,64,92,.8",
"74,73,105,255",
"117,122,191,255",
"130,173,219,255",
"148,197,248,255",
"183,234,255,255",
"144,223,254,255",
"87,193,235,255",
"45,145,194,255",
"36,115,171,255",
"30,82,142,255",
"30,82,142,255",
"21,66,119,255",
"22,60,82,0.8",
"7,27,38,.5",
"1,10,16,.3",
"9,4,1,0",
"0,0,12,0",
"0,0,12,0"};

String[] colors = {
"0,0,12,0",
"25,22,33,.3",
"32,32,44,.8",
"58,58,82",
"81,81,117",
"138,118,171",
"205,130,160",
"234,176,209",
"235,178,177",
"177,181,234",
"148,223,255",
"103,209,251",
"56,163,209",
"36,111,168",
"30,82,142",
"91,121,131",
"157,166,113",
"233,206,93",
"178,99,57",
"47,17,7",
"36,14,3",
"47,17,7",
"75,29,6,.4",
"21,8,0,0",
"0,0,12,0"};

int colorIndex = 0;

void setup() {
  size(600, 600);
  surface.setLocation(0, 0);
  
  led = new RGBLED();
}

void draw() {
  
  background(160);
  led.display();
}