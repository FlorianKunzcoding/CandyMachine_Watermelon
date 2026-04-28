#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>  // LiquidCrystal_I2C Bibliothek einbinden.
#include <Wire.h>               // Wire Bibliothek einbinden.
#include <Arduino.h>
#define motorRunningTime 3000

void HandleStateIdle();
void HandleStateMotorActive();
void Printer(String Name, int Row);

unsigned long currentTime;

enum STATES {
  STATE_INVALID = 0,
  STATE_IDLE = 1,
  STATE_MOTOR_ACTIVE = 2,
};

const int BTN_PROD1 = 8;
const int BTN_PROD2 = 9;
const int M1_OUT_A = GPIO_NUM_22;
const int M1_OUT_B = 4;
const int M2_OUT_A = 27;
const int M2_OUT_B = 6;


// Bestimmung des Displays:
LiquidCrystal_I2C lcd(0x3f, 16, 2);   // HEX-Adresse 0x3f, 16 Zeichen in 2 Zeilen.

enum STATES currentState = STATES::STATE_IDLE;
int selectedProduct = 0;

Bounce2::Button button_prod1 = Bounce2::Button();
Bounce2::Button button_prod2 = Bounce2::Button();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Serielle Schnittstelle --> Println

  button_prod1.attach(BTN_PROD1, INPUT_PULLUP);
  button_prod1.interval(10);
  button_prod1.setPressedState(LOW);

  button_prod2.attach(BTN_PROD2, INPUT_PULLUP);
  button_prod2.interval(10);
  button_prod2.setPressedState(LOW);

  pinMode(M1_OUT_A, OUTPUT);
  pinMode(M1_OUT_B, OUTPUT);
  pinMode(M2_OUT_A, OUTPUT);
  pinMode(M2_OUT_B, OUTPUT);
 


  currentState = STATES::STATE_IDLE;

  lcd.init();       // Im Setup wird der LCD gestartet.
  lcd.backlight();  // Hintergrundbeleuchtung einschalten.
                    // ( lcd.noBacklight(); schaltet die Beleuchtung aus).
  digitalWrite(M1_OUT_A,HIGH);
  digitalWrite(M2_OUT_A,HIGH);
  delay(1000);
  digitalWrite(M1_OUT_A,LOW);
  digitalWrite(M2_OUT_A,LOW);

  for (size_t i = 0; i < 38; i++)
  {
    pinMode(i,OUTPUT);
    digitalWrite(i,HIGH);
    Serial.println(i);
    delay(1000);
    digitalWrite(i,LOW);
  }
  

}



void loop() {
  // put your main code here, to run repeatedly:
  button_prod1.update();
  button_prod2.update();

  switch(currentState)
  {
    case (STATES::STATE_IDLE):
      // hier kommt der Code für state Idle
      HandleStateIdle();
      break;
    case (STATES::STATE_MOTOR_ACTIVE):
      // hier kommt der Code für state Motor Active
      HandleStateMotorActive();
      break;
    default:
      Serial.println("Ungültiger Zustand");
      break;
  }
}

void HandleStateIdle()
{
  bool btn_prod1_pressed = button_prod1.pressed();
  bool btn_prod2_pressed = button_prod2.pressed();

  if (btn_prod1_pressed)
  {
    selectedProduct = 1;
    Printer("Produkt", 0);
    Printer("1", 1);
    digitalWrite(M1_OUT_A, HIGH);
    digitalWrite(M1_OUT_B, LOW);
    currentTime = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;
  }

  if (btn_prod2_pressed)
  {
    selectedProduct = 2;
    Printer("Produkt", 0);
    Printer("2", 1);
    digitalWrite(M2_OUT_A, HIGH);
    digitalWrite(M2_OUT_B, LOW);
    currentTime = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;
  }
}

void HandleStateMotorActive()
{
  if(millis() - currentTime >= motorRunningTime)
  {
    currentState = STATES::STATE_IDLE;
    digitalWrite(M1_OUT_A, LOW);
    digitalWrite(M1_OUT_B, LOW);
    digitalWrite(M2_OUT_A, LOW);
    digitalWrite(M2_OUT_B, LOW);
    Printer("Bitte wählen", 0);
    Printer("Sie ein Produkt", 1);
  }
}

void Printer(String text, int row)
{
  lcd.setCursor((16 - text.length())/2, row);
  lcd.print(text);
}