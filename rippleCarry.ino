/*
 * Final version of the Ripple Carry 5-bit Full Adder
 */
#include <LiquidCrystal.h>

//pins for the LCD connections
const int rst = 26, en = 27, d4 = 22, d5 = 23, d6 = 24, d7 = 25;
LiquidCrystal lcd(rst, en, d4, d5, d6, d7);

//clock frequency
int clk = 1;

//ISR flag variables
int relayFlag = 0;
int lcdFlag = 0;
int clockFlag = 0;
int aFlag = 0;
int bFlag = 0;
int sumFlag = 0;

//bit inputs
int C_in = 43;
int A_in_one = 44;
int B_in_one = 45;
int A_in_two = 46;
int B_in_two = 47;
int A_in_three = 48;
int B_in_three = 49;
int A_in_four = 50;
int B_in_four = 51;
int A_in_five = 52;
int B_in_five = 53;

//pins to read sum bits
int bit_zero = 34;
int bit_one = 33;
int bit_two = 32;
int bit_three = 31;
int bit_four = 30;
int sum[5] = {0};
int variableSum = 0;

//test numbers
int a = 0;
int b = 0;
int var = 0;
int TEST_FREQ;

//User numbers
int user_A = 0;
int user_B = 0;
int userSum = 0;

//interrupt can be pin 2,3,18,19,20,21
int interruptMode = 2;
int interruptFreq = 3;
int interruptIntA = 18;
int interruptIntB = 19;
int interruptSum = 20;
int modeSel = 0;
int freq = 1;

void setup() {
  cli();
  
  //bit input pins on arduino uno
  pinMode(A_in_one, OUTPUT);
  pinMode(B_in_one, OUTPUT);
  pinMode(A_in_two, OUTPUT);
  pinMode(B_in_two, OUTPUT);
  pinMode(A_in_three, OUTPUT);
  pinMode(B_in_three, OUTPUT);
  pinMode(A_in_four, OUTPUT);
  pinMode(B_in_four, OUTPUT);
  pinMode(A_in_five, OUTPUT);
  pinMode(B_in_five, OUTPUT);
  pinMode(C_in, OUTPUT);

  //reading the sum bits
  pinMode(bit_zero, INPUT);
  pinMode(bit_one, INPUT);
  pinMode(bit_two, INPUT);
  pinMode(bit_three, INPUT);
  pinMode(bit_four, INPUT);
  

  //Liquid Crystal Display
  lcd.begin(16, 2);
  lcd.clear();
  
  //interrupt
  pinMode(interruptFreq, INPUT);
  pinMode(interruptMode, INPUT);
  pinMode(interruptIntA, INPUT);
  pinMode(interruptIntB, INPUT);
  pinMode(interruptSum, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptFreq), Clock_Frequency, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptMode), Mode_Select, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptIntA), A_Select, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptIntB), B_Select, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptSum), Calculate_Sum, RISING);


  
  //set Timer1 for the relay frequency
  TCCR1A = 0; //set entire TCCR1A register to 0
  TCCR1B = 0; //set entire TCCR1B register to 0
  TCNT1 = 0;  //Initialize counter value to 0
  //set compare match register for 1 hz increments
  //compare match = [(clk freq / (prescaler * desired interrupt frequency)] - 1
  // 1 hz = 15624, 2 hz = 7812, 4hz = 3905, 8 hz = 1952
  // 16 hz = 976, 32 hz = 487
  OCR1A = 15624; //[(16,000,000/ (1 * 1024)]-1  (must be < 65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  //set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  //enable compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  //set Timer4 for the LCD frequency
  TCCR4A = 0; //set entire TCCR0A register to 0
  TCCR4B = 0; //set entire TCCR0B register to 0
  TCNT4 = 0;  //Initialize counter value to 0
  //set compare match register for 1 hx increments
  //compare match = [(clk freq / (prescaler * desired interrupt frequency)] - 1
  //seven seg freq is 144 hz = OCR0A = ~109
  OCR4A = 976; //must be < 256
  // turn on CTC mode
  TCCR4B |= (1 << WGM12);
  //set CS00 and CS02 bits for 1024 prescaler
  TCCR4B |= (1 << CS12) | (1 << CS10);
  //enable compare interrupt
  TIMSK4 |= (1 << OCIE4A);
  
  sei();  //allow interrupts
}

void loop() {
  switch(modeSel){
    //TEST MODE
    case 0:
      if(relayFlag){
        switch(var){
         case 0:
                a = 0;
                b = 0;
                var = 1;
                break;
         case 1:
                a = 1;
                b = 0;
                var = 2;
                break;    
         case 2:
                a = 1;
                b = 1;
                var = 3;
                break;
         case 3:
                a = 0;
                b = 3;
                var = 4;
                break; 
         case 4:
                a = 2;
                b = 2;
                var = 5;
                break;
         case 5:
                a = 1;
                b = 4;
                var = 6;
                break; 
         case 6:
                a = 4;
                b = 2;
                var = 7;
                break;
         case 7:
                a = 3;
                b = 4;
                var = 8;
                break; 
         case 8:
                a = 5;
                b = 3;
                var = 9;
                break; 
         case 9:
                a = 5;
                b = 4;
                var = 10;
                break; 
         case 10:
                a = 8;
                b = 2;
                var = 11;
                break; 
         case 11:
                a = 6;
                b = 5;
                var = 12;
                break; 
         case 12:
                a = 3;
                b = 9;
                var = 13;
                break; 
         case 13:
                a = 10;
                b = 3;
                var = 14;
                break; 
         case 14:
                a = 7;
                b = 7;
                var = 15;
                break; 
         case 15:
                a = 12;
                b = 3;
                var = 16;
                break; 
         case 16:
                a = 0;
                b = 16;
                var = 17;
                break;
         case 17:
                a = 9;
                b = 8;
                var = 18;
                break;
         case 18:
                a = 2;
                b = 16;
                var = 19;
                break;
         case 19:
                a = 10;
                b = 9;
                var = 20;
                break;
         case 20:
                a = 5;
                b = 15;
                var = 21;
                break;
         case 21:
                a = 8;
                b = 13;
                var = 22;
                break;
         case 22:
                a = 2;
                b = 20;
                var = 23;
                break;
         case 23:
                a = 4;
                b = 19;
                var = 24;
                break;
         case 24:
                a = 1;
                b = 23;
                var = 25;
                break;
         case 25:
                a = 25;
                b = 0;
                var = 26;
                break;
         case 26:
                a = 16;
                b = 10;
                var = 27;
                break;
         case 27:
                a = 14;
                b = 13;
                var = 28;
                break;
         case 28:
                a = 17;
                b = 11;
                var = 29;
                break;
         case 29:
                a = 4;
                b = 25;
                var = 30;
                break;
         case 30:
                a = 15;
                b = 15;
                var = 31;
                break;
         case 31:
                a = 16;
                b = 15;
                var = 0;
                break;
    }//end switch case
        
        digitalWrite(A_in_one, bitRead(a, 0));
        digitalWrite(B_in_one, bitRead(b, 0));
        digitalWrite(C_in, LOW);
        
        digitalWrite(A_in_two, bitRead(a, 1));
        digitalWrite(B_in_two, bitRead(b, 1));
      
        digitalWrite(A_in_three, bitRead(a, 2));
        digitalWrite(B_in_three, bitRead(b, 2));
    
        digitalWrite(A_in_four, bitRead(a, 3));
        digitalWrite(B_in_four, bitRead(b, 3));
    
        digitalWrite(A_in_five, bitRead(a, 4));
        digitalWrite(B_in_five, bitRead(b, 4));
    
        relayFlag = 0;
      }//end if relayFlag
      
      if(lcdFlag){
        int bitPin = 30;
        for(int i = 0; i < 5; i++){
          sum[i] = digitalRead(bitPin);
          bitPin++;
        }
    
        for(int j = 0; j < 5; j++){
          bitWrite(variableSum, j, sum[j]);
        }
    
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print(String(a) + String(" + ") + String(b) + String(" = ") + String(variableSum));
        lcd.setCursor(11,0);
        lcd.print(String(clk) + String(" Hz"));
        lcd.setCursor(0,0);
        lcd.print("TEST MODE");
            
        lcdFlag = 0;
      }//end if lcdFlag

        if(clockFlag){
          switch(freq){
            case 0:
                OCR1A = 15624; //1 hz
                clk = 1;
                freq = 1;
                break;
            case 1:
                OCR1A = 7812; //2 hz
                clk = 2;
                freq = 2;
                break;
            case 2:
                OCR1A = 3905; //4 hz
                clk = 4;
                freq = 3;
                break;
            case 3:
                OCR1A = 1952; //8 hz
                clk = 8;
                freq = 4;
                break;
            case 4:
                OCR1A = 976; //16 hz
                clk = 16;
                freq = 0;
                break;
          }
          clockFlag = 0;
        }//end if clockFlag
      break;

      //USER MODE
      case 1:
        if(relayFlag){
          if(aFlag){
            if(user_A < 16){
              user_A++;
            }
            else{
              user_A = 0;
            }
            aFlag = 0;
          }
          
          if(bFlag){
            if(user_B < 16){
              user_B++;
            }
            else{
              user_B = 0;
            }
            bFlag = 0;
          }

          if(sumFlag){
            
            lcd.clear();
            
            digitalWrite(A_in_one, bitRead(user_A, 0));
            digitalWrite(B_in_one, bitRead(user_B, 0));
            digitalWrite(C_in, LOW);
            
            digitalWrite(A_in_two, bitRead(user_A, 1));
            digitalWrite(B_in_two, bitRead(user_B, 1));
          
            digitalWrite(A_in_three, bitRead(user_A, 2));
            digitalWrite(B_in_three, bitRead(user_B, 2));
        
            digitalWrite(A_in_four, bitRead(user_A, 3));
            digitalWrite(B_in_four, bitRead(user_B, 3));
        
            digitalWrite(A_in_five, bitRead(user_A, 4));
            digitalWrite(B_in_five, bitRead(user_B, 4));

            sumFlag = 0;
          }

          relayFlag = 0;
        }
        
        if(lcdFlag){
          int bitPin = 30;
          for(int i = 0; i < 5; i++){
            sum[i] = digitalRead(bitPin);
            bitPin++;
          }
      
          for(int j = 0; j < 5; j++){
            bitWrite(userSum, j, sum[j]);
          }
      
          //lcd.clear();
          lcd.setCursor(0,1);
          lcd.print(String(user_A) + String(" + ") + String(user_B) + String(" = ") + String(userSum));
          lcd.setCursor(11,0);
          lcd.print(String(clk) + String(" Hz"));
          lcd.setCursor(0,0);
          lcd.print("USER MODE");
        }//end if lcdFlag

        if(clockFlag){
          switch(freq){
            case 0:
                OCR1A = 15624; //1 hz
                clk = 1;
                freq = 1;
                break;
            case 1:
                OCR1A = 7812; //2 hz
                clk = 2;
                freq = 2;
                break;
            case 2:
                OCR1A = 3905; //4 hz
                clk = 4;
                freq = 3;
                break;
            case 3:
                OCR1A = 1952; //8 hz
                clk = 8;
                freq = 4;
                break;
            case 4:
                OCR1A = 976; //16 hz
                clk = 16;
                freq = 0;
                break;
          }
          clockFlag = 0;
        }
      break;
  }//end switch mode
}//end loop

//debounced pushbutton to control relay frequency
void Clock_Frequency(){

  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();

  if( interrupt_time - last_interrupt > 200){
    clockFlag = 1;
    last_interrupt = interrupt_time;
  }
}

void Mode_Select(){
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();

  if( interrupt_time - last_interrupt > 200){
    if(!modeSel){
      modeSel = 1;
    }
    else{
      modeSel = 0;
    }
    last_interrupt = interrupt_time;
  }
}

void A_Select(){
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();

  if( interrupt_time - last_interrupt > 200){
    aFlag = 1;
    last_interrupt = interrupt_time;
  }
}

void B_Select(){
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();

  if( interrupt_time - last_interrupt > 200){
    bFlag = 1;
    last_interrupt = interrupt_time;
  }
}

void Calculate_Sum(){
  static unsigned long last_interrupt = 0;
  unsigned long interrupt_time = millis();

  if( interrupt_time - last_interrupt > 200){
    sumFlag = 1;
    last_interrupt = interrupt_time;
  }
}
//timer 4 for LCD
ISR(TIMER4_COMPA_vect){
  lcdFlag = 1;
}

//TImer 1 ISR function
ISR(TIMER1_COMPA_vect){
  relayFlag = 1;
}
