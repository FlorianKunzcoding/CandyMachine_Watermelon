#include <Arduino.h>
#include <Bounce2.h>
#include <LiquidCrystal_I2C.h>
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

const int BTN_PROD1 = 33;
const int BTN_PROD2 = 25;

const int M1_OUT_A = 26;
const int M1_OUT_B = 26;
const int M2_OUT_A = 27;
const int M2_OUT_B = 27;

enum STATES currentState = STATES::STATE_IDLE;
int selectedProduct = 0;

Bounce2::Button button_prod1 = Bounce2::Button();
Bounce2::Button button_prod2 = Bounce2::Button();

 // Bestimmung des Displays:
  LiquidCrystal_I2C lcd(0x27, 16, 2);   // HEX-Adresse 0x3f, 16 Zeichen in 2 Zeilen.



void setup()
{
  pinMode(M1_OUT_A, OUTPUT);
  pinMode(M2_OUT_A, OUTPUT);

  button_prod1.attach(BTN_PROD1, INPUT_PULLUP);
  button_prod1.interval(10);
  button_prod1.setPressedState(LOW);

  button_prod2.attach(BTN_PROD2, INPUT_PULLUP);
  button_prod2.interval(10);
  button_prod2.setPressedState(LOW);

 
  lcd.init();       // Im Setup wird der LCD gestartet.
  lcd.backlight();  // Hintergrundbeleuchtung einschalten.
                    // ( lcd.noBacklight(); schaltet die Beleuchtung aus).
  lcd.clear();
  Printer("Bitte w\xE1hlen", 0);
  Printer("Sie ein Produkt", 1);

  currentState = STATES::STATE_IDLE;
}

void loop(){
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

void HandleStateIdle(){
  button_prod1.update();
  button_prod2.update();

  if (button_prod1.pressed()){
    selectedProduct = 1;
    lcd.clear();
    Printer("Produkt", 0);
    Printer("1", 1);
    digitalWrite(M1_OUT_A, HIGH);
    digitalWrite(M1_OUT_B, LOW);
    currentTime = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;

  }

  if(button_prod2.pressed()){
    selectedProduct = 2;
    lcd.clear();
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
    lcd.clear();
    Printer("Bitte w\xE1hlen", 0);
    Printer("Sie ein Produkt", 1);
  }
}

void Printer(String text, int row)
{
  lcd.setCursor((16 - text.length())/2, row);
  lcd.print(text);
}