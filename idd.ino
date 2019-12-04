// Lois Lee
// IDD Final Project 
// Prototype Version
// Last Modified Dec 3/2019


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// for samples
int sample = 0;
int sample_counter = 0;
int num_samples = 20;
int samples[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int freq = 0;

// for volume
int pos_previous = 0;
int neg_previous = 0;

// volume levels
int volume_level = 0;
int volume_inc = 0;

// pins | INPUTS 
// # A3 is for the mic
#define mic  A3
// # A2, A0 is for the capacitive sensor to increase "volume"
#define inc  A2
#define dec  A0

// pins | OUTPUTS
// # 1, 2, 7, 8, 12, 13 are the led pins
int vol_1 = 1;
int vol_2 = 2;
int vol_3 = 7;
int vol_4 = 8;
int vol_5 = 12;
int vol_6 = 13;
// # 3, 5, 6, 9, 10, 11 are the pwms on the metro for the vibration motors
int vib_1 = 3;
int vib_2 = 5;
int vib_3 = 6;
int vib_4 = 9;
int vib_5 = 10;
int vib_6 = 11;
// # A1 also for vibration motors
#define vib_7  A1 
// # A4, A5 are for the OLED display


void setup() {
    // setup monitor
    Serial.begin(9600);
    // pins
    pinMode(mic, INPUT); 
    pinMode(inc, INPUT); 
    pinMode(dec, INPUT); 
    pinMode(vol_1, OUTPUT); 
    pinMode(vol_2, OUTPUT); 
    pinMode(vol_3, OUTPUT); 
    pinMode(vol_4, OUTPUT); 
    pinMode(vol_5, OUTPUT); 
    pinMode(vol_6, OUTPUT); 
    pinMode(vib_1, OUTPUT); 
    pinMode(vib_2, OUTPUT); 
    pinMode(vib_3, OUTPUT); 
    pinMode(vib_4, OUTPUT); 
    pinMode(vib_5, OUTPUT); 
    pinMode(vib_6, OUTPUT); 
    pinMode(vib_7, OUTPUT); 

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    display.display();
    delay(1); // Pause for 2 seconds

    // Clear the buffer
    display.clearDisplay();

    testfillrect();  
    delay(2000); 
    display.clearDisplay(); 
    intro();  
    delay(2000); 
    display.clearDisplay(); 
}


void loop() {
    // check for user input on the FSRs
    user_feedback();
    // calculate the freq
    frequ();
    // get sample and get volume level
    sample_and_volume();
    // cap the volume and add user input
    volume_cap();
    

    // show on the leds the vol
    change_vol_led();
    // play the vibration motors with those freqs
    play();

}

void print_info(){
    Serial.println('samples:');
    Serial.println(' ');
    for (int i = 0; i < num_samples; i++){
        Serial.print(samples[i]);
        Serial.print(',');
    }
    Serial.println("sample: ");
    Serial.println(sample);
    Serial.println("volume_level: ");
    Serial.println(volume_level);
    

}

void frequ(){
    int high = find_max(samples);
    int low = find_min(samples, num_samples);
//    Serial.println("high/low");
//    Serial.println(high);
//    Serial.println(low);
    
    // map(value, fromLow, fromHigh, toLow, toHigh)
    int hi = samples[sample_counter];
    freq = map(hi,low, high, 0, 1023);
}

void user_feedback(){
    int pos = analogRead(inc);
    int neg = analogRead(dec);

    if (pos_previous < 800 && pos > 800){
        if (volume_inc <= 6){
            volume_inc = volume_inc + 1;   
            volume_up();        
        }
    }
    else if (neg_previous < 800 && neg > 800){
        if (volume_inc >= 1){
            volume_inc = volume_inc - 1;
            volume_down();
        }
    }
    pos_previous = pos;
    neg_previous = neg;
}


void play(){
    sample = analogRead(mic);
    if(volume_level == 1){
        play1(freq);
    }
    else if(volume_level == 2){
        play2(freq);
    }
    else if(volume_level == 3){
        play3(freq);
    }
    else if(volume_level == 4){
        play4(freq);
    }
    else if(volume_level == 5){
        play5(freq);
    }
    else if(volume_level == 6){
        play6(freq);
    }
    else{
        Serial.println("the volume_level is out of range");
    }
}
// change_vol_led
// samples and calculates the volume level from the maximum.
void change_vol_led(){
    if(volume_level == 1){
        digitalWrite(vol_1,HIGH);
        digitalWrite(vol_2,LOW);
        digitalWrite(vol_3,LOW);
        digitalWrite(vol_4,LOW);
        digitalWrite(vol_5,LOW);
        digitalWrite(vol_6,LOW);
    }
    else if(volume_level == 2){
        digitalWrite(vol_1,LOW);
        digitalWrite(vol_2,HIGH);
        digitalWrite(vol_3,LOW);
        digitalWrite(vol_4,LOW);
        digitalWrite(vol_5,LOW);
        digitalWrite(vol_6,LOW);
    }
    else if(volume_level == 3){
        digitalWrite(vol_1,LOW);
        digitalWrite(vol_2,LOW);
        digitalWrite(vol_3,HIGH);
        digitalWrite(vol_4,LOW);
        digitalWrite(vol_5,LOW);
        digitalWrite(vol_6,LOW);
    }
    else if(volume_level == 4){
        digitalWrite(vol_1,LOW);
        digitalWrite(vol_2,LOW);
        digitalWrite(vol_3,LOW);
        digitalWrite(vol_4,HIGH);
        digitalWrite(vol_5,LOW);
        digitalWrite(vol_6,LOW);
    }
    else if(volume_level == 5){
        digitalWrite(vol_1,LOW);
        digitalWrite(vol_2,LOW);
        digitalWrite(vol_3,LOW);
        digitalWrite(vol_4,LOW);
        digitalWrite(vol_5,HIGH);
        digitalWrite(vol_6,LOW);
    }
    else if(volume_level == 6){
        digitalWrite(vol_1,LOW);
        digitalWrite(vol_2,LOW);
        digitalWrite(vol_3,LOW);
        digitalWrite(vol_4,LOW);
        digitalWrite(vol_5,LOW);
        digitalWrite(vol_6,HIGH);
    }
    else{
        Serial.println("the volume_level is out of range");
    }

}


// volume_cap
// the user input increments the volume up and down, and make
// sure it's in the right range
void volume_cap(){
    if(volume_level + volume_inc > 6){
        volume_level = 6;
        volume_too_high();
    }
    else if(volume_level + volume_inc < 1){
        volume_level = 1;
        volume_too_low();
    }
    else{
        volume_level = volume_level + volume_inc;
    }
}


// sample_and_volume
// samples and calculates the volume level from the maximum.
void sample_and_volume(){
    // read the sample from mic and put into sample list
    samples[sample_counter] = analogRead(mic);
    // if the array did not overfill, find the max to use
    // for the LED reference
    if(sample_counter < num_samples ){
        int intermediate = find_max(samples);
        // map(value, fromLow, fromHigh, toLow, toHigh)
        volume_level = map(intermediate, 0, 1023, 1, 6);
    }
    else{
    // if it did overfill, set counter to zero to restart
    // find max for LED reference
        sample_counter = 0;
        int intermediate =  find_max(samples);
        // map(value, fromLow, fromHigh, toLow, toHigh)
        volume_level = map(intermediate, 0, 1023, 1, 6);
    }
    // increment counter
    sample_counter = sample_counter + 1;

}

// find_max
// returns maximum value of an array
int find_max(int* arr){
    int max = 0;
    for (int i = 0; i < num_samples; i++){
        if(arr[i]>max){
            max = arr[i];
        }
    }
    return max;
}
// find_min
// returns maximum value of an array
int find_min(int arr[], int n){
   int temp = arr[0];
   for(int i=0; i<n; i++) {
      if(temp>arr[i]) {
         temp=arr[i];
      }
   }
   return temp;
}
// testfillrect
// draws
void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}


// volume_up
// tells volume
void volume_up(void) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.print(F("Volume ")); display.println(volume_level);
    display.display();
    delay(1000);
    display.clearDisplay();

}
// volume_down
// tells volume
void volume_down(void) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.print(F("Volume ")); display.println(volume_level);
    display.display();
    delay(1000);
    display.clearDisplay();

}

// volume_too_high
// tells user volume_too_high
void volume_too_high(void) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Volume too high"));
    display.display();
    delay(1000);
    display.clearDisplay();

}
// volume_too_low
// tells user volume_too_low
void volume_too_low(void) {
    display.clearDisplay();
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Volume too low"));
    display.display();
    delay(1000);
    display.clearDisplay();

}


// intro
// does the intro 
void intro(void) {
    display.clearDisplay();
    display.setTextSize(1.2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("                     This is the haptic   feedback project."));
    display.display();
    delay(3000);
    display.clearDisplay();
    display.setTextSize(1.2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("                     Put your hand on the clear surface."));
    display.display();
    delay(3000);
    display.clearDisplay();
    display.setTextSize(1.2);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("                     Now lean back and    feel sound."));
    display.display();
    delay(3000);
    display.clearDisplay();
}
// testdrawstyles
// draws words
void testdrawstyles(void) {
    display.clearDisplay();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Hello, world!"));

    display.setTextColor(BLACK, WHITE); // Draw 'inverse' text
    display.println(3.141592);

    display.setTextSize(2);             // Draw 2X-scale text
    display.setTextColor(WHITE);
    display.print(F("0x")); display.println(0xDEADBEEF, HEX);

    display.display();
    delay(2000);
}


// play1 
// plays at volume level 1
void play1(int amp){
    analogWrite(vib_1, amp);
    analogWrite(vib_2, 0);
    analogWrite(vib_3, 0);
    analogWrite(vib_4, 0);
    analogWrite(vib_5, 0);
    analogWrite(vib_6, 0);
    analogWrite(vib_7, 0);
}
// play2 
// plays at volume level 2
void play2(int amp){
    analogWrite(vib_1, amp);
    analogWrite(vib_2, amp);
    analogWrite(vib_3, 0);
    analogWrite(vib_4, 0);
    analogWrite(vib_5, 0);
    analogWrite(vib_6, 0);
    analogWrite(vib_7, 0);
}
// play3 
// plays at volume level 3
void play3(int amp){
    analogWrite(vib_1, amp);
    analogWrite(vib_2, amp);
    analogWrite(vib_3, amp);
    analogWrite(vib_4, 0);
    analogWrite(vib_5, 0);
    analogWrite(vib_6, 0);
    analogWrite(vib_7, 0);
}
// play4 
// plays at volume level 4
void play4(int amp){
    analogWrite(vib_1, amp);
    analogWrite(vib_2, amp);
    analogWrite(vib_3, amp);
    analogWrite(vib_4, amp);
    analogWrite(vib_5, 0);
    analogWrite(vib_6, 0);
    analogWrite(vib_7, 0);
}
// play5
// plays at volume level 5
void play5(int amp){
    analogWrite(vib_1, amp);
    analogWrite(vib_2, amp);
    analogWrite(vib_3, amp);
    analogWrite(vib_4, amp);
    analogWrite(vib_5, amp);
    analogWrite(vib_6, 0);
    analogWrite(vib_7, 0);
}
// play6
// plays at volume level 6
void play6(int amp){
    analogWrite(vib_1, amp);
    analogWrite(vib_2, amp);
    analogWrite(vib_3, amp);
    analogWrite(vib_4, amp);
    analogWrite(vib_5, amp);
    analogWrite(vib_6, amp);
    analogWrite(vib_7, amp);
}