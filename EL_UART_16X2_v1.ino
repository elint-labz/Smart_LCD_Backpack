/*

	Elint Labz - 16x2 Serial LCD Backpack v1.0 

	This is the firmware (arduino code) contained in the Atmega (8, 168 or 328P) IC in the 
	Elint Labz - 16x2 Serial LCD Backpack. 

Connections between the 16x2 & the Atmega (Arduino) IC
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 14
   LCD D5 pin to digital pin 15
   LCD D6 pin to digital pin 16
   LCD D7 pin to digital pin 17
   LCD backlight to digital pin 5

	Date of Creation 01 September 2014
	Created by Elint Labz (www.elintlabz.in)
	Developer:: Harsha

  You can reach us by writing to hello@elintlabz.in 
*/



char cmd[20]={0};
//Commands stored in array for comparision/recognition
char clr[3]={'c','l','r'}; // clear screen
char hom[3]={'h','o','m'}; // Home (set cursor to (0,0)
char set[3]={'s','e','t'}; // set cursor position
char pri[3]={'p','r','i'}; // print on LCD
char cur[3]={'c','u','r'}; // underline cursor ON/OFF
char bli[3]={'b','l','i'}; // blinking block cursor ON/OFF
char dis[3]={'d','i','s'}; // Display ON/OFF
char sdp[3]={'s','d','p'}; // scroll display left/right
char ltr[3]={'l','t','r'}; // print on LCD left to right/ right to left
char aut[3]={'a','u','t'}; // autoscroll ON/OFF
char bau[3]={'b','a','u'}; // select baud rate
char bak[3]={'b','a','k'}; // back light ON/OFF
char gap[3]={'g','a','p'}; // space

int l=0, i=0, a=0, j=0;
int x=0, y=0, col=0, row=0, n=0, m=0;
int baud=0, value=0;
char search = 0;


#include <EEPROM.h>                    
#include <LiquidCrystal.h>            
LiquidCrystal lcd(12,11,14,15,16,17);  
 

void setup()
{
   pinMode(13,OUTPUT);        
   pinMode(5,OUTPUT);         
   lcd.begin(16,2);           
   digitalWrite(5,LOW);       
   lcd.print("ElintLabz");    
   lcd.setCursor(12,1);       
   lcd.print("V1.0");         
   digitalWrite(5,HIGH);      
   delay(2000);              
   lcd.clear();               
   value = EEPROM.read(12);   
 
 /*
    Here the byte taken from the specified address of the EEPROM
    is compared in the 'if else' loops to select the particular baudrate.
    Suppose if the byte taken from the EEPROM is different from the values
    used (ie, 1,2,3,4,5 & 6) to compare in loops then by default baudrate
    9600 is selected.
  */
  
  if(value == 1)
  {
    Serial.begin(4800);              // initialize the serial communication with baudrate 4800
    lcd.print("BaudRate 4800");      
  }
 
 else if(value == 2)
  {
    Serial.begin(9600);             
    lcd.print("BaudRate 9600");     
  }
  
 else if(value == 3)
  {
    Serial.begin(19200);             
    lcd.print("BaudRate 19200");     
  }
 
 else if(value == 4)
  {
    Serial.begin(38400);            
    lcd.print("BaudRate 38400");     
  }
 
 else if(value == 5)
  {
    Serial.begin(57600);             
    lcd.print("BaudRate 57600");     
  }
 
 else if(value == 6)
  {
    Serial.begin(115200);            
    lcd.print("BaudRate 115200");    
  }
  else
  {
    Serial.begin(9600);              
    lcd.print("BaudRate 9600");      
  }
  
  delay(3000);                       
  lcd.clear();                      
}

void loop()
{
  if (Serial.available() > 1)          
     {
       digitalWrite(13,LOW);           
       
       while(l<20)                     
          {
            cmd[l] = Serial.read();    // read the most recent byte and place it in a particular array position
            l++;                       // increment the value of l (ie, l = l+1)
          }
        
        l=0;                           // set l value to zero
        operations();                  // execute this function
     }
 else
     {
       digitalWrite(13,HIGH);       // turn the on board LED ON by making the voltage HIGH  
       delay(100);                  // wait for 100 milliseconds
     }
}

/* This function consists of many small LCD functions.
   In these small LCD functions the first three elements of the array is 
   compared with the first three elements of the pre defined command arrays.
   If the first three elements of both the arrays are equal then only a
   particular LCD function which is defined in that loop is executed.
 */  
   
void operations()    
{
  Autoscroll();
  Backlight();
  BaudRate();
  Blink();
  clearscreen();
  Cursor();
  Display();
  Home();
  LeftRight();
  PRINT();
  setcursor();
  space();
  ScrollDisplay();
}
 
// This function is used to clear the Array in which new incoming bytes are saved 
void clearArray()
{
   for(int p=0;p<21;p++)
      {
        cmd[p]=0;
      }
}

// This function is used for autosctoll.
void Autoscroll()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == aut[a])
      {
        i++;
        if(i==3)
        {
          i=0;
          int z = cmd[4] -48;
            if(z == 1)
            {
              lcd.autoscroll();
            }
            if(z == 0)
            {
              lcd.noAutoscroll();
            }
          clearArray();               // This function is used to clear the Array
        }
      }
     a++;
  }
}

// This function is used to control the backlight of LCD.
void Backlight()
{
   a=i=0;    
    while(a<3)
    {      
       if(cmd[a] == bak[a])
        { 
         i++;
           if(i==3)
            {
              i=0;
              int z = cmd[4] -48;
            if(z == 1)
            {
              digitalWrite(5,HIGH);
            }
            if(z == 0)
            {
              digitalWrite(5,LOW);
            }
              clearArray();             // This function is used to clear the Array
             }
         }
        a++;
    }
}

// This function is used to select the baudrates for serial communication.
void BaudRate()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == bau[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            baud = cmd[4];
            value = baud - 48;        // to convert ASCII no. to integer value
            EEPROM.write(12,value);   // write the value to the appropriate byte of the EEPROM
            clearArray();             // This function is used to clear the Array
            setup();
            
           }
       }
     a++;
  }
}  
 
// This function helps us to control the block type of cursor. 
void Blink()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == bli[a])
      {
        i++;
        if(i==3)
          {
           i=0;
           int z = cmd[4] -48;        // to convert ASCII no. to integer value
            if(z == 1)
            {
               lcd.blink();
            }
            if(z == 0)
            {
               lcd.noBlink();
            }
           clearArray();              // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function clears the LCD screen.
void clearscreen()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == clr[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            lcd.clear();
            clearArray();                  // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function helps us to control the underline type of cursor.
void Cursor()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == cur[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            int z = cmd[4] -48;     // to convert ASCII no. to integer value
            if(z == 1)
            {
              lcd.cursor();
            }
            if(z == 0)
            {
              lcd.noCursor();
            }
            clearArray();           // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function helps us to control the LCD display.
void Display()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == dis[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            int z = cmd[4] -48;           // to convert ASCII no. to integer value
            if(z == 1)
            {
              lcd.display();
            }
            if(z == 0)
            {
              lcd.noDisplay();
            }
            clearArray();                  // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function sets the cursor position to (0,0)
void Home()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == hom[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            lcd.home();
            clearArray();                 // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function is used to print the data in forward & reverse direction. 
void LeftRight()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == ltr[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            int z = cmd[4] -48;         // to convert ASCII no. to integer value
            if(z == 1)
            {
              lcd.leftToRight();
            }
            if(z == 0)
            {
              lcd.rightToLeft();
            }
            clearArray();                // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function is used to print the user data on the screen.reverse 
void PRINT()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == pri[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            for(int f=4;f<21;f++)
            {
              if(cmd[f] == ';')
                {
                  search = f;
                  for( int v=4;v<search;v++)
                     {
                      lcd.print(cmd[v]);
                     }
               
                }
            }
            clearArray();              // This function is used to clear the Array
           }
      }
     a++;
  }
}  

// This function is used to set the cursor position. 
void setcursor()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == set[a])
      {
        i++;
        if(i==3)
          {
            i=0;
             x = cmd[4];
             y = cmd[5];
             n = cmd[7];
             m = cmd[8];
             col = (x*10) + (y) - 528;       // to convert ASCII no. to integer value
             row = (n*10) + (m) - 528;       // to convert ASCII no. to integer value
             lcd.setCursor(col,row);
             x=y=n=m=row=col=0;
             clearArray();                  // This function is used to clear the Array
           }
      }
     a++;
  }
}

// This function helps us to provide a space on the screen.
void space()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == gap[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            lcd.print(" ");
            clearArray();                  // This function is used to clear the Array
          }
      }
     a++;
  }
}

// This function is used to shift the data on the screen to right/left by one position.
void ScrollDisplay()
{
  i=a=0;
  while(a<3)
  {
    if(cmd[a] == sdp[a])
      {
        i++;
        if(i==3)
          {
            i=0;
            int z = cmd[4] -48;               // to convert ASCII no. to integer value
            if(z == 1)
            {
               lcd.scrollDisplayRight();
            }
            if(z == 0)
            {
               lcd.scrollDisplayLeft();
            }
            clearArray();                      // This function is used to clear the Array
           }
      }
     a++;
  }
}  

