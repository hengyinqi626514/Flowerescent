 // Include the required Wire library for I2C<br>
#include<Wire.h>

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#define DATA_PIN    13
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    40
CRGB leds[NUM_LEDS];
int randomColor[20];
#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int countUp = 0;
long randNumber;

const int echoPin = 9;
const int trigPin = 8;
int distance = 0;
int x = 0;

int PressurePin0=A0;
int PressurePin1=A1;
int PressurePin2=A2;
int force0;
int force1;
int force2;

int trig[3] = {0, 0 ,0};

int trigPrv[3] = {0, 0 ,0};
void setup() {
   delay(3000); // 3 second delay for recovery
   pinMode(echoPin, INPUT);  
   pinMode(trigPin, OUTPUT);
   Serial.begin(9600); 

   FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  randomSeed (analogRead(0));
  // Start the I2C Bus as Master
  Wire.begin(); 

  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(trig[0]);              // sends 0  
  Wire.write(trig[1]);              // sends 1  
  Wire.write(trig[2]);              // sends 2  
  Wire.endTransmission(); 
  delay(500);
}

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {

  //float totaldist , avgDist;
  //totaldist=0;

  force0=analogRead(PressurePin0);                    //pressure sensor 1
  //Serial.println(force0);
  force1=analogRead(PressurePin1);                    //pressure sensor 2
  //Serial.println(force1);
  force2=analogRead(PressurePin2);                    //pressure sensor 3
  //Serial.println(force2);

  if(force0<499 && force1<499 && force2<499)//if no one step 
  {
   
     for(int i = 0; i<3;i++)
     {
        trig[i] = 0; 
     }
     sinelon();
     FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  
   
  }
  else
  {
    if(force0>500)//If person step on rock 1
    {     
      trig[0]=1;
      
    }
    else
    {
      trig[0]=0;
    }
    //Serial.println( trig[0]);
    if(force1>500)//If person step on rock 2
    {        
      trig[1]=1;
    }
    else
    {
      trig[1]=0;
    }
    //Serial.println( trig[1]);
    if(force2>500)//if person step on rock 3
    {       
      trig[2]=1;  
    }
    else
    {
      trig[2]=0;
    }
      //Serial.println( trig[2]);

      for( int i = 0 ; i<3 ; i++)//saves current input into prev input 
    {
       //Serial.print( trig[i]);
    }
  }
  
  if(trig[0] == trigPrv[0] && trig[1] == trigPrv[1] && trig[2] == trigPrv[2])//compair to previous input 
  {
    //Serial.println( "no change");
    for( int i = 0 ; i<3 ; i++)//saves current input into prev input 
    {
      trigPrv[i] = trig[i];
      //Serial.println(trigPrv[i]);
    }
      
  }
  else
  {
    
    
    if(trig[0] == 0 && trig[1] == 0 && trig[2] == 1)//priority to last stone
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(0);    
        Wire.write(0); 
        Wire.write(trig[2]);  
        Wire.endTransmission(); 
        OrRedFlowers();
        Serial.println( "red play");
    }
    else if(trig[0] == 0 && trig[1] == 1 && trig[2] == 0)
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(0);    
        Wire.write(trig[1]); 
        Wire.write(0); 
        Wire.endTransmission(); 
        YellowFlowers();
         Serial.println( "yellow play ");
    }
    else if(trig[0] == 1 && trig[1] == 0 && trig[2] == 0)
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(trig[0]);    
        Wire.write(0); 
        Wire.write(0); 
        Wire.endTransmission(); 
        BuPuFlowers();
         Serial.println( "blue play");
    }
    else if(trig[0] == 0 && trig[1] == 0 && trig[2] == 0)
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(0);    
        Wire.write(0); 
        Wire.write(0); 
        Wire.endTransmission(); 
        
    }
    
    for( int i = 0 ; i<3 ; i++)//saves current input into prev input 
    {
      trigPrv[i] = trig[i];
      Serial.println(trigPrv[i]);
    }
    

  }
  
}

void randomColors()
{
  Serial.println( "random play");
  
  for (int i = 0; i <= 20; i++)//generates a list of 20 random numbers 
  {
    randNumber = random(0,40);
    
    randomColor[i] = randNumber;
    Serial.print(i);
    Serial.println(randNumber);
    Serial.println(randomColor[i]);
  }

  for (int j = 0; j <= 20; j++)//marks the leds of those numbers as on 
  {
    if (j >= 0 && j < 6)
    {
      leds[randomColor[j]] = CRGB(222,10 ,10); //Rossa
      
    } 
    else if (j >= 6 && j < 12)
    {
      leds[randomColor[j]] = CRGB(243, 91, 4); //Tangerine
      
    }
    else if (j >= 12 && j < 20)
    {
      leds[randomColor[j]] = CRGB(10, 10, 255); // Blue
     
    }
  }
   FastLED.show();
  delay(100);
  FastLED.clear();//clear all leds
  FastLED.show();
}

void BuPuFlowers()
{
  Serial.println( "blue is play");
  FastLED.clear();//clear all leds
  FastLED.show();


      leds[0] = CRGB(50, 10, 200); //purple;
      leds[9] = CRGB(10, 10, 255); //blu
      leds[10] = CRGB(50, 10, 200); //purple;
      leds[11] = CRGB(10, 10, 255); //blu
      leds[25] = CRGB(50, 10, 200); //purple;
      leds[26] = CRGB(10, 10, 255); //blu
      leds[26] = CRGB(50, 10, 200); //purple;
      leds[27] = CRGB(10, 10, 255); //blu
      leds[28] = CRGB(50, 10, 200); //purple;
      leds[29] = CRGB(10, 10, 255); //blu
      leds[30] = CRGB(50, 10, 200); //purple;
      leds[27] = CRGB(10, 10, 255); //blu
      leds[28] = CRGB(50, 10, 200); //purple;
      FastLED.show();//wave step 1
      delay(200);
      
      leds[1] = CRGB(10, 10, 255); //blu
      leds[2] = CRGB(50, 10, 200); //purple;
      leds[3] = CRGB(10, 10, 255); //blu
      leds[4] = CRGB(50, 10, 200); //purple;
      leds[5] = CRGB(10, 10, 255); //blu
      leds[6] = CRGB(50, 10, 200); //purple;
      leds[7] = CRGB(10, 10, 255); //blu
      leds[8] = CRGB(50, 10, 200); //purple;
      leds[20] = CRGB(10, 10, 255); //blu
      leds[24] = CRGB(50, 10, 200); //purple;
      leds[31] = CRGB(10, 10, 255); //blu
      leds[38] = CRGB(50, 10, 200); //purple;
      FastLED.show();//wave step 2
      delay(200);
      
      leds[12] = CRGB(10, 10, 255); //blu
      leds[13] = CRGB(50, 10, 200); //purple;
      leds[14] = CRGB(10, 10, 255); //blu
      leds[15] = CRGB(50, 10, 200); //purple;
      leds[16] = CRGB(10, 10, 255); //blu
      leds[17] = CRGB(50, 10, 200); //purple;
      leds[18] = CRGB(10, 10, 255); //blu
      leds[19] = CRGB(50, 10, 200); //purple;
      leds[21] = CRGB(10, 10, 255); //blu
      leds[22] = CRGB(50, 10, 200); //purple;
      leds[23] = CRGB(10, 10, 255); //blu
      leds[32] = CRGB(50, 10, 200); //purple;
      leds[37] = CRGB(10, 10, 255); //blu
      leds[39] = CRGB(50, 10, 200); //purple;
      FastLED.show();//wave step 3
      delay(200);
      leds[33] = CRGB(10, 10, 255); //blu
      leds[34] = CRGB(10, 10, 255); //blu
      leds[35] = CRGB(50, 10, 200); //purple;
      leds[36] = CRGB(10, 10, 255); //blu
      FastLED.show();//wave step 4
      delay(200);

      Serial.println( "blue stop");
}

void YellowFlowers()
{

  Serial.println( "yellow is play");
  FastLED.clear();//clear all leds
  FastLED.show();
  delay(200);
  leds[0] = CRGB(243, 91, 4); //tangerine
  leds[1] = CRGB(247, 184, 1); //selective yellow
  leds[2] = CRGB(243, 91, 4); //tangerine
  leds[3] = CRGB(247, 184, 1); //selective yellow
  leds[4] = CRGB(243, 91, 4); //tangerine  
  leds[5] = CRGB(247, 184, 1); //selective yellow
  leds[6] = CRGB(243, 91, 4); //tangerine
  leds[7] = CRGB(247, 184, 1); //selective yellow
  leds[9] = CRGB(243, 91, 4); //tangerine
  leds[1] = CRGB(247, 184, 1); //selective yellow
  leds[2] = CRGB(243, 91, 4); //tangerine
    leds[10] = CRGB(247, 184, 1); //selective yellow
  leds[11] = CRGB(243, 91, 4); //tangerine
  FastLED.show();//wave step 1
  delay(200);
  
  leds[8] = CRGB(247, 184, 1); //selective yellow
  leds[12] = CRGB(243, 91, 4); //tangerine
  leds[13] = CRGB(247, 184, 1); //selective yellow
  leds[14] = CRGB(243, 91, 4); //tangerine  
  leds[15] = CRGB(247, 184, 1); //selective yellow
  leds[16] = CRGB(243, 91, 4); //tangerine
  leds[17] = CRGB(247, 184, 1); //selective yellow
  leds[18] = CRGB(243, 91, 4); //tangerine
  leds[20] = CRGB(247, 184, 1); //selective yellow
  leds[24] = CRGB(243, 91, 4); //tangerine
  leds[25] = CRGB(247, 184, 1); //selective yellow
  leds[26] = CRGB(243, 91, 4); //tangerine
  FastLED.show(); //wave step 2
  delay(200);
  
  leds[19] = CRGB(247, 184, 1); //selective yellow
  leds[21] = CRGB(243, 91, 4); //tangerine
  leds[22] = CRGB(247, 184, 1); //selective yellow
  leds[23] = CRGB(243, 91, 4); //tangerine  
  leds[27] = CRGB(247, 184, 1); //selective yellow
  leds[30] = CRGB(243, 91, 4); //tangerine
  leds[31] = CRGB(247, 184, 1); //selective yellow
  FastLED.show(); //wave step 3
  delay(200);
  
  leds[28] = CRGB(243, 91, 4); //tangerine
  leds[32] = CRGB(247, 184, 1); //selective yellow
  leds[33] = CRGB(243, 91, 4); //tangerine
  leds[34] = CRGB(247, 184, 1); //selective yellow
  leds[37] = CRGB(243, 91, 4); //tangerine
  leds[38] = CRGB(247, 184, 1); //selective yellow
  FastLED.show(); //wave step 4 
  delay(200);
  
  leds[29] = CRGB(247, 184, 1); //selective yellow
  leds[39] = CRGB(243, 91, 4); //tangerine
  leds[36] = CRGB(247, 184, 1); //selective yellow
  leds[35] = CRGB(243, 91, 4); //tangerine  
  FastLED.show(); //wave step 5
  delay(200);
 Serial.println( "yellow stop"); 
}

void OrRedFlowers()
{
  Serial.println( "red  is play");
  FastLED.clear();
  FastLED.show();
  leds[6] = CRGB(220, 47, 2); //persimmon
  leds[7] = CRGB(222,10 ,10); //Rosso corsa
  leds[8] = CRGB(220, 47, 2); //persimmon
  leds[9] = CRGB(222,10 ,10); //Rosso corsa
  leds[20] = CRGB(220, 47, 2); //persimmon
  leds[21] = CRGB(222,10 ,10); //Rosso corsa
  leds[22] = CRGB(220, 47, 2); //persimmon
  leds[23] = CRGB(222,10 ,10); //Rosso corsa
  FastLED.show();
  delay(200);

  leds[6] = CRGB(220, 47, 2); //persimmon
  leds[10] = CRGB(222,10 ,10); //Rosso corsa
  leds[17] = CRGB(220, 47, 2); //persimmon
  leds[18] = CRGB(222,10 ,10); //Rosso corsa
  leds[19] = CRGB(220, 47, 2); //persimmon
  leds[24] = CRGB(222,10 ,10); //Rosso corsa
  leds[33] = CRGB(220, 47, 2); //persimmon
  leds[34] = CRGB(222,10 ,10); //Rosso corsa  
  FastLED.show();
  delay(200);

  leds[0] = CRGB(222,10 ,10); //Rosso corsa
  leds[1] = CRGB(220, 47, 2); //persimmon
  leds[2] = CRGB(222,10 ,10); //Rosso corsa
  leds[3] = CRGB(220, 47, 2); //persimmon
  leds[4] = CRGB(222,10 ,10); //Rosso corsa
  leds[5] = CRGB(220, 47, 2); //persimmon
  leds[16] = CRGB(222,10 ,10); //Rosso corsa
  leds[25] = CRGB(220, 47, 2); //persimmon
  leds[31] = CRGB(222,10 ,10); //Rosso corsa  
  leds[32] = CRGB(220, 47, 2); //persimmon
  FastLED.show();
  delay(200);
  leds[11] = CRGB(222,10 ,10); //Rosso corsa  
  leds[12] = CRGB(220, 47, 2); //persimmon
  leds[13] = CRGB(222,10 ,10); //Rosso corsa
  leds[14] = CRGB(220, 47, 2); //persimmon
  leds[15] = CRGB(222,10 ,10); //Rosso corsa
  leds[26] = CRGB(220, 47, 2); //persimmon
  leds[30] = CRGB(222,10 ,10); //Rosso corsa
  leds[35] = CRGB(220, 47, 2); //persimmon
  leds[36] = CRGB(222,10 ,10); //Rosso corsa  
  leds[37] = CRGB(220, 47, 2); //persimmon
  FastLED.show();
  delay(200);

  leds[27] = CRGB(222,10 ,10); //Rosso corsa  
  leds[28] = CRGB(220, 47, 2); //persimmon
  leds[29] = CRGB(222,10 ,10); //Rosso corsa
  leds[38] = CRGB(220, 47, 2); //persimmon
  leds[39] = CRGB(222,10 ,10); //Rosso corsa
  FastLED.show();
  delay(200);

  Serial.println( "red stop");
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
   FastLED.show();  
    delay(200);
}


void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}
