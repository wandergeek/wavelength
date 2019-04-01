import processing.serial.*;

Serial serial;  // Create object from Serial class
boolean firstContact = false;

void setup() {  // setup() runs once
  size(640, 360);    
  serial = new Serial(this, "/dev/cu.usbmodem1411", 9600);
  serial.bufferUntil('\n'); 
 
}

float rad = 0.1;
int dir = 1;
String val;

void draw() {
  background(102);
  stroke(255);
  translate(width/2, height/2);
  line(0, 0, cos(rad)*100, sin(rad)*100);
  
  if(rad > PI || rad <= 0) { 
    dir *= -1;
  }
    rad = rad + 0.01 * dir;
    //println(degrees(rad));
   

}

void keyPressed() {
  //int i = 100;
  //serial.write(0);
  //serial.write(" ");
  //serial.write(50);
  println(key);
  
  if(key == 'q') {
    serial.write("0 100\r");
  } else if(key == 'w') {
    serial.write("1 100\r");
  }
}

void serialEvent( Serial serial) {
//put the incoming data into a String - 
//the '\n' is our end delimiter indicating the end of a complete packet
val = serial.readStringUntil('\n');
//make sure our data isn't empty before continuing
if (val != null) {
  //trim whitespace and formatting characters (like carriage return)
  val = trim(val);
  println(val);

  //look for our 'A' string to start the handshake
  //if it's there, clear the buffer, and send a request for data
  if (firstContact == false) {
    if (val.equals("A")) {
      serial.clear();
      firstContact = true;
      serial.write("A");
      println("contact");
    }
  }
  else { //if we've already established contact, keep getting and parsing data
    //println(val);

    if (mousePressed == true) 
    {                           //if we clicked in the window
      serial.write('1');        //send a 1
      println("1");
    }

    // when you've parsed the data you have, ask for more:
    serial.write("A");
    }
  }
}
