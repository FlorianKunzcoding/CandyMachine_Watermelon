#include <Bounce2.h>

#define outputTime 3000

void HandleStateIdle();
void HandleStateMotorActive();

unsigned long time;

enum STATES {
  STATE_INVALID = 0,
  STATE_IDLE = 1,
  STATE_MOTOR_ACTIVE = 2,
};

const int BTN_PROD1 = 3;
const int BTN_PROD2 = 4;
const int BTN_PROD1_REFILL = 5;
const int BTN_PROD2_REFILL = 6;
const int M1_OUT_A = 10;
const int M1_OUT_B = 11;
const int M2_OUT_A = 12;
const int M2_OUT_B = 13;

enum STATES currentState = STATES::STATE_IDLE;
int selectedProduct = 0;

Bounce2::Button button_prod1 = Bounce2::Button();
Bounce2::Button button_prod2 = Bounce2::Button();
Bounce2::Button button_prod1_REFILL = Bounce2::Button();
Bounce2::Button button_prod2_REFILL = Bounce2::Button();

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
}



void loop() {
  // put your main code here, to run repeatedly:
  button_prod1.update();
  button_prod2.update();
  button_prod1_REFILL.update();
  button_prod2_REFILL.update();

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

  bool btn_prod1_REFILL_pressed = button_prod1_REFILL.pressed();
  bool btn_prod2_REFILL_pressed = button_prod2_REFILL.pressed();

  if (btn_prod1_pressed)
  {
    selectedProduct = 1;
    digitalWrite(M1_OUT_A, HIGH);
    digitalWrite(M1_OUT_B, LOW);
    time = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;
  }

  if (btn_prod2_pressed)
  {
    selectedProduct = 2;
    digitalWrite(M2_OUT_A, HIGH);
    digitalWrite(M2_OUT_B, LOW);
    time = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;
  }

  if (btn_prod1_REFILL_pressed)
  {
    selectedProduct = 1;
    digitalWrite(M1_OUT_A, LOW);
    digitalWrite(M1_OUT_B, HIGH);
    time = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;
  }

  if (btn_prod2_REFILL_pressed)
  {
    selectedProduct = 2;
    digitalWrite(M2_OUT_A, LOW);
    digitalWrite(M2_OUT_B, HIGH);
    time = millis();
    currentState = STATES::STATE_MOTOR_ACTIVE;
  }
}

void HandleStateMotorActive()
{
  if(millis() - time >= outputTime)
  {
    currentState = STATES::STATE_IDLE;
    digitalWrite(M1_OUT_A, LOW);
    digitalWrite(M1_OUT_B, LOW);
    digitalWrite(M2_OUT_A, LOW);
    digitalWrite(M2_OUT_B, LOW);
  }
}