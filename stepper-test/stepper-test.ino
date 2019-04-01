#include <SparkFunAutoDriver.h>
#include <SPI.h>


// Create our AutoDriver instance. The parameters are the position in the chain of
//  boards (with board 0 being located at the end of the chain, farthest from the
//  controlling processor), CS pin, and reset pin.
AutoDriver boardA(0, 10, 7);

void dSPINConfig(void)
{
  boardA.SPIPortConnect(&SPI);      // Before doing anything else, we need to
                                    //  tell the object which SPI port to use.
                                    //  Some devices may have more than one.

  boardA.configSyncPin(BUSY_PIN, 0);// BUSY pin low during operations;
                                    //  second paramter ignored.
  boardA.configStepMode(STEP_FS);   // 0 microsteps per step
  boardA.setMaxSpeed(200);        // 10000 steps/s max
  boardA.setFullSpeed(200);       // microstep below 10000 steps/s
  boardA.setAcc(200);             // accelerate at 10000 steps/s/s
  boardA.setDec(200);
  boardA.setSlewRate(SR_530V_us);   // Upping the edge speed increases torque.
  boardA.setOCThreshold(OC_1875mA);  // OC threshold 750mA
  boardA.setPWMFreq(PWM_DIV_2, PWM_MUL_2); // 31.25kHz PWM freq
  boardA.setOCShutdown(OC_SD_ENABLE); // don't shutdown on OC
  boardA.setVoltageComp(VS_COMP_DISABLE); // don't compensate for motor V
  boardA.setSwitchMode(SW_USER);    // Switch is not hard stop
  boardA.setOscMode(INT_16MHZ_OSCOUT_16MHZ); // for boardA, we want 16MHz
                                    //  internal osc, 16MHz out.
  boardA.setAccKVAL(128);           // We'll tinker with these later, if needed.
  boardA.setDecKVAL(128);
  boardA.setRunKVAL(128);
  boardA.setHoldKVAL(32);           // This controls the holding current; keep it low.
}


void setup()
{
  Serial.begin(9600);
  delay(1000);
  // Start by setting up the SPI port and pins. The
  //  Autodriver library does not do this for you!
  pinMode(7, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(7, LOW);       // This low/high is a reset of the L6470 chip on the
  digitalWrite(7, HIGH);      //  Autodriver board, and is a good thing to do at
                              //  the start of any Autodriver sketch, to be sure
                              //  you're starting the Autodriver from a known state.
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  dSPINConfig();  
//  establishContact();  // send a byte to establish contact until receiver responds 

}


byte runDir = 1;

void loop() {
 int numSteps = 0;
  if (Serial.available()) {
    runDir = Serial.parseInt();
//    numSteps = map(Serial.parseInt(),0,360,0,200); #this sends it into a frenzy if going reverse direction??
    numSteps = Serial.parseInt();
    Serial.print("numsteps: ");
    Serial.println(numSteps);
  
  if(runDir == 0) {
    boardA.goToDir(FWD, numSteps);
  } else if(runDir == 1) {
    boardA.goToDir(REV, numSteps);
  }
//  boardA.softStop();
  while (boardA.busyCheck());
  }
 }


void establishContact() {
  while (Serial.available() <= 0) {
  Serial.println("A");   // send a capital A
  delay(300);
  }
}

