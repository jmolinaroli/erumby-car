#define __MOTORCOMMAND__
#include <EEPROM.h>
#include <arduino.h>
#include <Wire.h>

#define STEERING 51   //3° register bit 0x04  
#define MOTOR 52      //2° register bit 0x02
#define MODE 53       //1° register bit 0x01

#define ESC 11
#define SERVO 12

// STERZO - VALORI DI DUTY CYCLE (ms, periodo in cui sta alta l'onda quadra)
#define DUTY_STEERING_RIGHT 1064
#define DUTY_STEERING_NEUTRAL 1476
#define DUTY_STEERING_LEFT 1890
// Proporzione dei valori precedenti in 16 bit
// Valore nominale = 6881
// Valori limite ottenuti = +/- 20% valore nominale
#define DUTY_SERVO_RIGHT 5505   //4915
#define DUTY_SERVO_NEUTRAL 6881
#define DUTY_SERVO_LEFT 8257    //8847

// MOTORE - VALORI DI DUTY CYCLE (ms, periodo in cui sta alta l'onda quadra)
#define DUTY_MOTOR_MAX 2032
#define DUTY_MOTOR_NEUTRAL 1496
#define DUTY_MOTOR_MIN 1000
// Proporzione dei valori precedenti in 16 bit
// Valore nominale = 7012
// Valori limite ottenuti = +/- 15% valore nominale
#define DUTY_ESC_MAX 8064
#define DUTY_ESC_NEUTRAL 7010
#define DUTY_ESC_MIN 5960

// MODALITA - VALORI DI DUTY CYCLE (ms, periodo in cui sta alta l'onda quadra)
#define DUTY_MODE_HIGH 2032
#define DUTY_MODE_CENTRAL 1496
#define DUTY_MODE_LOW 1000

int val = 0;
int valS = 0;

void SetMotors(int pin_ESC, int pin_Servo) {
  SetPinFrequency(ESC, 71.4);
  SetPinFrequency(SERVO, 71.4);
}
void UpdateMotors() {
  valS = map(pinData[0].pulseWidth, DUTY_STEERING_RIGHT, DUTY_STEERING_LEFT, DUTY_SERVO_RIGHT, DUTY_SERVO_LEFT);
  if (pinData[1].pulseWidth <= DUTY_MOTOR_NEUTRAL - 50) {
    pwmWriteHR(ESC, DUTY_ESC_NEUTRAL);
  }
  if (pinData[1].pulseWidth >= DUTY_MOTOR_NEUTRAL + 50) {
    val = map(pinData[1].pulseWidth, DUTY_MOTOR_MIN, DUTY_MOTOR_MAX, DUTY_ESC_NEUTRAL, DUTY_ESC_MIN);
    pwmWriteHR(ESC, val);
  }
  if ((pinData[1].pulseWidth <= DUTY_MOTOR_NEUTRAL + 50) && (pinData[1].pulseWidth >= DUTY_MOTOR_NEUTRAL - 50))
    pwmWriteHR(ESC, DUTY_ESC_NEUTRAL);
  pwmWriteHR(SERVO, valS);
}
void Sicurezza() {
    pwmWriteHR(ESC, DUTY_ESC_NEUTRAL);
    pwmWriteHR(SERVO, DUTY_SERVO_NEUTRAL);
}