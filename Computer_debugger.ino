#include <Silego.h>
#include "Wire.h"
#include <avr/pgmspace.h>
/*
 * Luke Thomas
 * COMPUTER reader
 * a way to read all nodes of computer project via i2c
*/

//PINMAP definition: each index of the array[20] defines the location of the pin value for given chip
// for ex. idx =0 is for pin1; idx = 1 is for pin2 and might equal mat_input_1 (IO0 Digital input) from pg145
//              PIN  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20
byte  CTRL_map[] = {63,  1,  2,  3,  4,  5,  6,  0,  0,  46,   0,  43,  42,   0,  22,  45,  17,  39,  41,  19};  //verified
byte   OUT_map[] = {63,  1,  2, 39, 40, 41, 42,  0,  0,  43,   0,  44,   7,   0,   8,   9,  10,  11,  12,   0};  //verified
byte    DP_map[] = {63,  1,  2,  3,  4,  5,  6,  0,  0,   0,   0,   0,   7,   0,   0,   0,  21,  20,  19,  18};  //verified
byte    PC_map[] = {63,  1,  2, 39, 28,  5,  6,  0,  0,  15,   0,  17,  24,   0,  25,  26,  29,  33,  34,  36};  //updated per r006
byte ALU_B_map[] = {63,  1,  2,  3,  4,  5,  6,  0,  0,   0,   0,   0,   7,   0,   0,  17,  16,  15,  14,   0};  //verified
byte   ALU_map[] = {63,  1,  2,  3,  4,  5,  6,  0,  0,   0,   0,  19,   7,   0,   0,   9,  43,  42,  20,  13};  //verified


Silego gpak(0x00);

//CHIP ADDRESSES
byte CTRL_addr = 8;
byte OUT_addr = 96;
byte DP_addr = 48;
byte PC_addr = 32;
byte ALU_B_addr= 40;
byte ALU_one_addr = 72;
byte ALU_two_addr = 80;
byte ALU_three_addr = 88;

class Chip {
  public:
    byte matrix[8];
    boolean mat_valid = false;
    byte address;
    byte pin_map[20];
    char codename[11];
    Chip(byte address_, byte pin_map_[], char codename_[]) {
      this->mat_valid= false;
      this->address = address_;
      for(int i = 0; i<10; i++)
        codename[i] = codename_[i];
      for(int i= 0; i<20; i++)
        pin_map[i] = pin_map_[i];
    }
    void update_mat() {
      //Serial.println("updating mat...");
      gpak.set_address(this->address);
      for(int i = 0; i < 7; i++){
         int myData = gpak.readI2C(i+0x74);
         if(myData == -1){
            Serial.print("NACKED.  Addr: ");
            Serial.print(address);
            Serial.println(" on update Mat");
            break;
         }
         matrix[i] = myData;
      }
    }
    void print_mat() 
    {
      Serial.print(F("Address Matrix: "));
      for(int i = 0; i < 7; i++){
         Serial.println(matrix[i], BIN);
         }
     }
     byte get_pin(byte pin) 
    {
      if(pin >= 1 && pin <= 20) { pin--; }
      else{ 
        Serial.println(F("Pin error"));
        return 0;}
      return (bitRead(matrix[pin_map[pin]/8], pin_map[pin]%8));
       
     }
     void force_pin(byte pin_num, bool value)
     {
      byte VDD = 63;
      byte GND =  0;

      //Serial.print(F("Forcing ")); Serial.print(codename); Serial.print(F(" pin ")); Serial.print(pin_num); Serial.print(F(" to ")); Serial.println(value);
      gpak.set_address(this->address);
      switch (pin_num){
        case 4 : 
          if(value){ //write a one
            set_connection(VDD, 4, 0x16);
            }
          else{
            set_connection(GND, 4, 0x16);
          }
          break;
        
        case 5 : 
          if(value){ //write a one
            set_connection(VDD, 0, 0x2A);
          }
          else{
            set_connection(GND, 0, 0x2A);
          }
          break;
           
        case 12 : 
          if(value){ //write a one
            set_connection(VDD, 6, 0x39);
          }
          else{
            set_connection(GND, 6, 0x39);
          }
          break;
         case 16 : 
           if(value){ //write a one
             set_connection(VDD, 4, 0x3D);
           }
           else           {
             set_connection(GND, 4, 0x3D);
           }
           break;
         case 17 : 
           if(value){ //write a one
            set_connection(VDD, 0, 0x3F);
           }
           else           {
            set_connection(GND, 0, 0x3F);
           }
           break;
           
         case 18 : 
          if(value){ //write a one
           set_connection(VDD, 4, 0x40); 
           }
           else           {
           set_connection(GND, 4, 0x40);
           }
           break;
         case 19 : 
           if(value){ //write a one
            set_connection(VDD, 0, 0x42);
           }
           else{
            set_connection(GND, 0, 0x42);
           }
           break;
               
        default: Serial.println(F("invalid pin force")); break;
      }     
      
     }
     void restore_pin(byte pin_num)
     {
      byte VDD = 63;
      byte GND =  0;

      Serial.print(F("Restoring ")); Serial.print(codename); Serial.print(F(" pin ")); Serial.println(pin_num); 
      gpak.set_address(this->address);
      switch (pin_num){
        case 12 : 
          set_connection(43, 6, 0x39);
          break;
         case 16 :
          set_connection(45, 4, 0x3D);
          break;
         case 17 : 
          set_connection(43, 0, 0x3F);
          break; 
         case 18 : 
          set_connection(42, 4, 0x40); 
          break;
         case 19 : 
          set_connection(20, 0, 0x42);
          break;
               
        default: Serial.println(F("invalid pin restore")); break;
      }     
      
     }
    void set_connection(byte in, byte offset, byte target)
    {
      byte GP_MASK_LOC = 0xC9;
      
      short mask = 0b111111;
      short full_in = in<<offset;
      //Serial.print(F("mask before is ")); Serial.println(mask, BIN);
      mask = mask<<offset;
      //Serial.print(F("mask after  is "));Serial.println(mask, BIN);
      mask = ~mask;
      //Serial.print(F("mask inv    is "));Serial.println(mask, BIN);
      byte gp_mask = mask;
      byte gp_in = full_in;
      //Serial.print(F("gp_mask    is "));Serial.println(gp_mask, BIN);
      //Serial.print(F("gp_in      is "));Serial.println(gp_in, BIN);
      gpak.writeI2C(GP_MASK_LOC, gp_mask);
      gpak.writeI2C(target, gp_in);
      target++;
      gp_mask = mask>>8;
      gp_in = full_in>>8;
      //Serial.print(F("gp_mask    is ")) ;Serial.println(gp_mask, BIN);
      //Serial.print(F("gp_in      is ")) ;Serial.println(gp_in, BIN);
      gpak.writeI2C(GP_MASK_LOC, gp_mask);
      gpak.writeI2C(target, gp_in);
      
      
    }
};

/*
 * Global Variables
 */
Chip CTRL(CTRL_addr, CTRL_map, "CTRL");
Chip OUT(OUT_addr, OUT_map, "OUT");
Chip DP(DP_addr, DP_map, "DP");
Chip PC(PC_addr, PC_map, "PC");
Chip ALU_B(ALU_B_addr, ALU_B_map, "ALU_B");
Chip ALU_one(ALU_one_addr, ALU_map, "ALU_one");
Chip ALU_two(ALU_two_addr, ALU_map, "ALU_two");
Chip ALU_three(ALU_three_addr, ALU_map, "ALU_three");
boolean writepending = true;  // whether the string is complete
int id = 0;
byte current_clock = 0;
const byte numPins = 8;
const byte num_test_bytes = 18;

byte powers[] = {1, 2, 4, 8, 16, 32, 64, 128};
///  pins              LSB                        MSB
byte io_pins[]      = {2, 3, 4, 5, 6, 7, 8, 9};
#define nOE   10
#define nWE   11
#define CLK   12
#define nRST  13
#define CTRL_nOE 12
#define CTRL_nWE 16
//connection locations

const byte test_data[]    = {
B00110000, //LDa_C
B01100000, //OUTA
B00110001, //LDa_C
B01100000, //OUTA
B00110010, //LDa_C 4
B01100000, //OUTA   5
B00110011, //LDa_C  6
B01100000, //OUTA   7
B00110100, //LDa_C  8
B01100000, //OUTA   9
B00110101, //LDa_C  10 
B01100000, //OUTA   11
B00110110, //LDa_C  12
B01100000, //OUTA   13
B00110111, //LDa_C
B01100000, //OUTA
B00111000, //LDa_C
B01100000, //OUTA
B00111001, //LDa_C
B01100000, //OUTA
B00111010, //LDa_C
B01100000, //OUTA
B00111011, //LDa_C
B01100000, //OUTA   23
B00000000, //nop
B00111100, //LDa_C
B01100000, //OUTA
B00000000, //nop
B00111101, //LDa_C
B01100000, //OUTA
B00000000, //nop
B00111110, //LDa_C
B01100000, //OUTA
B01110001, //LDb with 1
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B00010000, //nop
B01100000, //OUTA
/*
B00110000, //LDa_C
B00110000, //LDa_C
B00110000, //LDa_C
B00110110, //LDa_C
B10000010, //STa_P
B11000010, //LDb_P
B00000000, //nop
B01100000, //OUTA
B00110011, //LDa_C
B11100001, //Add and STp at 0001
B10100001, //LDa from 0001
B10000001, //STa_P
B11000001, //LDb_P
B01100000, //OUTA
B00110011, //LDa_C
B10000001, //STa_P
B11000001, //LDb_P
B01100000, //OUTA
B00110101, //LDa_C
B10000001, //STa_P
B10100001, //LDa_P
B01100000, //OUTA

*/


B00000000 //NOP



};

byte test_readback[num_test_bytes]= {0,0,0,0};
byte temp_val = 0;
//CS is GND'ed
//A10,9,8 are GND'ed

/*
 * Functions 
 */
void force_ALU_data(byte data){
  ALU_three.force_pin(19, bitRead(data, 0));
  ALU_three.force_pin(18, bitRead(data, 1));
  ALU_three.force_pin(17, bitRead(data, 2));
  ALU_two.force_pin(19, bitRead(data, 3));
  ALU_two.force_pin(18, bitRead(data, 4));
  ALU_two.force_pin(17, bitRead(data, 5));
  ALU_one.force_pin(19, bitRead(data, 6));
  ALU_one.force_pin(18, bitRead(data, 7));
}
void restore_ALU_pins(){
  ALU_three.restore_pin(19);
  ALU_three.restore_pin(18);
  ALU_three.restore_pin(17);
  ALU_two.restore_pin(19);
  ALU_two.restore_pin(18);
  ALU_two.restore_pin(17);
  ALU_one.restore_pin(19);
  ALU_one.restore_pin(18);
}
void toggle_nRST(){
  PC.force_pin(5, 0);
  delay(10);
  PC.force_pin(5, 1);
}
void toggle_CLK(){
  PC.update_mat();
  if(PC.get_pin(4)){
    PC.force_pin(4, 0);
    current_clock = 0;
  }
  else{
    PC.force_pin(4,1);
    current_clock = 1;
  }
}
void program_load(byte program[], byte num_bytes)
{
  //io to output for writing
  toggle_nRST(); //reset address to 0;
  PC.force_pin(4, 0);
  CTRL.force_pin(CTRL_nOE, 1);
  CTRL.force_pin(CTRL_nWE, 1);
  for(int i =0; i<sizeof(test_data); i++)
  {
    force_ALU_data(test_data[i]);
    CTRL.force_pin(CTRL_nWE, 0);//toggle(nWE, HIGH);
    delay(1);
    CTRL.force_pin(CTRL_nWE, 1);
    toggle_CLK();
    delay(1);
    toggle_CLK(); //toggle(CLK, LOW);
  }
}
void pins_restore()
{
    restore_ALU_pins();
    CTRL.restore_pin(12);
    CTRL.restore_pin(16);
}

/*
 * Main()
 */
void test_force()
{
  delay(500);
  for(int i = 0; i < 16; i++)
  {
    toggle_CLK();
    delay(100);
  }
  toggle_nRST();
  
}
void setup() {
  Serial.begin(115200);
  Serial.println(F("connected"));
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println(F("Starting program load..."));
  Serial.println(sizeof(test_data));
  program_load(test_data, num_test_bytes);
  Serial.println(F("Finished program load."));
  Serial.println(F("skipping program read..."));
  //program_read(num_test_bytes);
  //Serial.println(F("Starting Readback output..."));
  delay(500);
  
  toggle_nRST();//toggle(nRST, HIGH);
  
  pins_restore();//  pins_destroy();
  Serial.println(F("Ready."));
  }

void loop() 
{
      //digitalWrite(A0, HIGH);   // turn the LED on (HIGH is the voltage level)
      //delay(100);                       // wait for a second
      //digitalWrite(A0, LOW);    // turn the LED off by making the voltage LOW
      //read_all();
      //read_all();
      //  test_force();

      //
      //Current State: I'm looking at getting read_all to work.  I need a way to reduce the footprint of the 
      // write to something more akin to a single line output.
      // ok got that working. just need to add actual I2C lines to OUT, and continue debugging!!!
      // good work so far...
      //

}

void verify_pins(Chip &test)
{
      Serial.print(test.codename);
      if(test.codename == ALU_one.codename | test.codename ==ALU_two.codename | test.codename ==ALU_three.codename)
      {
        Serial.print(F(": \t"));  //send a tab less to justify the long name of ALU_X versus others
      }
      else{
        Serial.print(F(": \t\t"));
      }
      
      //Serial.println(id);
                            // wait for a second
      //id++;
      test.update_mat();
      //test.print_mat();
      Serial.print(test.get_pin(2));
      for(int i = 3; i < 21; i++)
      {
        Serial.print(F(","));
        Serial.print(test.get_pin(i));
      }
      Serial.println();
      
}
void read_all()
{
    Serial.print(F("update: "));
    Serial.print(id);
    Serial.println(F("\t2,3,4,5,6,7,8,9,0,G,2,3,4,5,6,7,8,9,2"));

    verify_pins(CTRL);
    verify_pins(OUT);
    verify_pins(DP);
    verify_pins(PC);
    verify_pins(ALU_B);
    verify_pins(ALU_one);
    verify_pins(ALU_two);
    verify_pins(ALU_three);
    
    get_clock();
    verify_clock();
    get_current_PC();
    get_current_data();
    get_current_OP();
    get_current_DP();
    get_current_ALU();
    get_CTRL_status();
    Serial.println(F("\n\n\n"));
}
void get_current_PC()
{
  Serial.print(F("Prog Cnt: "));
  PC.update_mat();
  Serial.print(PC.get_pin(12));
  Serial.print(PC.get_pin(20));
  Serial.print(PC.get_pin(19));
  Serial.print(PC.get_pin(18));
  
  Serial.print(PC.get_pin(17));
  Serial.print(PC.get_pin(16));
  Serial.print(PC.get_pin(15));
  Serial.print(PC.get_pin(13));
  Serial.println();
}
void get_clock()
{
  Serial.print(F("Clock: "));
  Serial.println(current_clock);
}
void verify_clock() //this checks the clock pin of all parts to make sure they are receiveing the clock as expected
{
  if(CTRL.get_pin(6) != current_clock) {Serial.print(CTRL.codename); Serial.println(F(" failed clock test"));Serial.print(CTRL.get_pin(6)); Serial.print(F(" should have been ")); Serial.println(current_clock);}
  if(DP.get_pin(6) != current_clock) {Serial.print(DP.codename); Serial.println(F(" failed clock test"));  Serial.print(DP.get_pin(6)); Serial.print(F(" should have been ")); Serial.println(current_clock); }
  if(PC.get_pin(4) != current_clock) {Serial.print(PC.codename); Serial.println(F(" failed clock test"));}
  if(ALU_one.get_pin(7) != current_clock) {Serial.print(ALU_one.codename); Serial.println(F(" failed clock test"));}
  if(ALU_two.get_pin(7) != current_clock) {Serial.print(ALU_two.codename); Serial.println(F(" failed clock test"));}
  if(ALU_three.get_pin(7) != current_clock) {Serial.print(ALU_three.codename); Serial.println(F(" failed clock test"));}
  if(OUT.get_pin(2) != current_clock) {Serial.print(OUT.codename); Serial.println(F(" failed clock test"));}
  
}

void get_current_OP()
{
  byte OP_map[] = {18, 16, 15, 14};
  byte OP_out[] = {0, 0, 0, 0};
  byte OP_code = 0;
  Serial.print(F("current_OP: "));
  for(int i = 0; i < 4; i++)
  {
    OP_out[i] = bitRead(CTRL.matrix[OP_map[i]/8], OP_map[i]%8);
    Serial.print(OP_out[i]);
    OP_code += OP_out[i] * powers[3-i];
  }
  Serial.print(F("  :  "));
  Serial.print(OP_code);
  Serial.print(F("  :  "));

  switch (OP_code){
    case 3 : Serial.println(F("LDa C")); break;
    case 6 : Serial.println(F("OUTA")); break;
    case 7 : Serial.println(F("LDb C")); break;
    case 8 : Serial.println(F("STa P")); break;
    case 10 : Serial.println(F("LDa P")); break;
    case 12 : Serial.println(F("LDb P")); break;
    case 14 : Serial.println(F("ADD & STaP")); break;
    default: Serial.println(F("NOP")); break;

  }
}
void get_current_ALU()
{
  byte A_map[] = {21, 16, 45, 21, 16, 45, 21, 16};
  byte B_map[] = {15, 23, 46, 15, 23, 46, 15, 23};
  byte A_out[] = { 0,  0,  0,  0,  0,  0,  0,  0};
  byte B_out[] = { 0,  0,  0,  0,  0,  0,  0,  0};
  byte A_dec = 0;
  byte B_dec = 0;
  


  //Get A
  A_out[0] = bitRead(ALU_three.matrix[A_map[0]/8], A_map[0]%8);
  A_out[1] = bitRead(ALU_three.matrix[A_map[1]/8], A_map[1]%8);
  A_out[2] = bitRead(ALU_three.matrix[A_map[2]/8], A_map[2]%8);
  A_out[3] = bitRead(ALU_two.matrix[A_map[3]/8], A_map[3]%8);
  A_out[4] = bitRead(ALU_two.matrix[A_map[4]/8], A_map[4]%8);
  A_out[5] = bitRead(ALU_two.matrix[A_map[5]/8], A_map[5]%8);
  A_out[6] = bitRead(ALU_one.matrix[A_map[6]/8], A_map[6]%8);
  A_out[7] = bitRead(ALU_one.matrix[A_map[7]/8], A_map[7]%8);
  //Get B
  B_out[0] = bitRead(ALU_three.matrix[B_map[0]/8], B_map[0]%8);
  B_out[1] = bitRead(ALU_three.matrix[B_map[1]/8], B_map[1]%8);
  B_out[2] = bitRead(ALU_three.matrix[B_map[2]/8], B_map[2]%8);
  B_out[3] = bitRead(ALU_two.matrix[B_map[3]/8], B_map[3]%8);
  B_out[4] = bitRead(ALU_two.matrix[B_map[4]/8], B_map[4]%8);
  B_out[5] = bitRead(ALU_two.matrix[B_map[5]/8], B_map[5]%8);
  B_out[6] = bitRead(ALU_one.matrix[B_map[6]/8], B_map[6]%8);
  B_out[7] = bitRead(ALU_one.matrix[B_map[7]/8], B_map[7]%8);

  //convert and print A
  Serial.print(F("current_A: "));
  for(int i = 7; i >= 0; i--)
  {
    Serial.print(A_out[i]);
    A_dec += A_out[i] * powers[i];
  }
  Serial.print(F("  :  "));
  Serial.println(A_dec);
  
  //convert and print B
  Serial.print(F("current_B: "));  
  for(int i = 7; i >= 0; i--)
  {
    Serial.print(B_out[i]);
    B_dec += B_out[i] * powers[i];
  }
  Serial.print(F("  :  "));
  Serial.println(B_dec);
  
}
void get_current_DP()
{
  //save some memory, just use hardcode: byte DP_map[] = {18, 19, 20, 21};
  byte DP_out[] = {0, 0, 0, 0};
  Serial.print(F("current_DP: "));
  for(int i = 0; i < 4; i++)
  {
    DP_out[i] = bitRead(DP.matrix[(21-i)/8], (21-i)%8);
    Serial.print(DP_out[i]);
  }
  Serial.print(F("  :  "));
  Serial.println();
}
void get_current_data(){
  byte DATA_out[8];
  byte DATA_out_decimal = 0;
  DATA_out[0] = DP.get_pin(2);
  DATA_out[1] = DP.get_pin(3);
  DATA_out[2] = DP.get_pin(4);
  DATA_out[3] = DP.get_pin(5);

  DATA_out[4] = ALU_B.get_pin(5);
  DATA_out[5] = ALU_B.get_pin(4);
  DATA_out[6] = ALU_B.get_pin(3);
  DATA_out[7] = ALU_B.get_pin(2);

  //print values
  Serial.print(F("DATA_out : "));
  for(int i = 7; i >= 0; i--)
  {
    Serial.print(DATA_out[i]);
    DATA_out_decimal +=DATA_out[i] * powers[i];
  }
  Serial.print(F("  :  "));
  Serial.print(DATA_out_decimal);
  Serial.println(); 
}
void get_CTRL_status()
{
  Serial.print(F("CTRL Status: "));
  get_ptr_st();
  if(CTRL.get_pin(20))
    Serial.print(F("Full_Write "));
  if(CTRL.get_pin(19))
    Serial.print(F("A_WE "));
  if(CTRL.get_pin(18))
    Serial.print(F("B_WE "));
  if(CTRL.get_pin(17))
    Serial.print(F("DP_WE "));
  if(!CTRL.get_pin(16))
    Serial.print(F("nRAM_WE "));
  if(CTRL.get_pin(15))
    Serial.print(F("OUT_WE "));
  if(CTRL.get_pin(13))
    Serial.print(F("ADD_WE "));
  if(CTRL.get_pin(12))
    Serial.print(F("ALU_OE "));
  if(!CTRL.get_pin(12))
    Serial.print(F("RAM_nOE "));
  if(CTRL.get_pin(10))
    Serial.print(F("PC_OE "));
  if(!CTRL.get_pin(10))
    Serial.print(F("DP_nOE "));
  Serial.println();
}
void get_ptr_st()
{
  Serial.print(F("Ptr_St = "));
  Serial.print(bitRead(CTRL.matrix[24/8], 24%8));
  Serial.print(F(" "));
}
/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  int k = 0;
  int indx = 0;
  int temp_id=0;
  char temp_number [5];
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if(inChar >= 48 && inChar <=57)
    {
      temp_number[k]= inChar;
      k++;
      //Serial.println(k);
      delay(2); //dont know why i need it but it doesnt work without
    }
    if(inChar == '\n') 
    {
      while(k>0)  //essentially if we found a number, now we have to convert it from unicode digits to a binary rep
      {
        k--;
        temp_id += (temp_number[k]-48)*pow(10,indx++);
//        Serial.print("Temp_id is: ");
//        Serial.println(temp_id);
        //id = temp_id;
      }
//      SRAM_wr(id, io_pins); 
      break;
    }
    if(inChar == 'u')
      {
         id++;
      }
    if(inChar == 'i')
      {
          toggle_CLK();
          id++;
          delay(10);
          read_all();
      }
    if(inChar == 'd')
    {
      if(id!=0)id--;
    }
    if(inChar == 'n')
    {
      Serial.println(F("NRST"));
      toggle_nRST();
      id = 0;
    }
    if(inChar == 'c')
    {
      Serial.println(F("Clock"));
      toggle_CLK();
    }
    if(inChar == 'x')
    {
      Serial.println(F("X clocking (10)"));
      for(int i = 0; i < 10; i++)
      {
        toggle_CLK();
        
        delay(10);
        toggle_CLK();
      }
    }
    writepending = true;

  
  }   
}
