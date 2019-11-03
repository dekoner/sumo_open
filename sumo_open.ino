/*
 *  Powered by dekoner 03.11.19
 * 
 *  Программа для управления сумо-роботом. Задача
 *  Задача: вытолкнуть противника с ринга.
 *  
 *  В роботе используются три датчика: инфракрасный дальномер, ультразвуковой дальномер
 *  и инфракрасный датчик линии. Кроме того для запуска робота и определения вращения применется кнопка и переключатель
 *  
 *  Подключения:
 *      Ультразвуковой дальномер:
 *         TRIG -> 12
 *         ECHO -> 11
 *      Инфракрасный дальномер:
 *         OUT -> A6
 *      Инфракрасный датчик линии:
 *         OUT -> 7 (LINE)
 *      Кнопка включения:
 *         OUT -> A3 (SW_ON)
 *      Переключатель направления:
 *         OUT -> A0 (SW_DIR)
 *         
 *  Описание работы:
 *    После вкючения программа ожидает высокого сигнала на входе SW_ON. При его появлении
 *    робот выходит из режима ожидания и начинает действовать согласно полученным сигналам с датчиков:
 *    - при появлении сигнала с датчика линии робот движется назад 1 сек.
 *    - если измеренное расстояние дальномерами меньше уставки SP_DIST робот начинает движение вперед
 *    - если расстояние больше SP_DIST робот вращается в направлении заданном переключателем SW_DIR
 */

#include <SharpIR.h>
#include <Ultrasonic.h>
#include <MotorControl.h>

//#define OUTSERIAL 1

#define SW_ON A3
#define SW_DIR A0

#define SP_DIST 40
#define LINE 7

Ultrasonic us(12, 11);
SharpIR ir( SharpIR::GP2Y0A21YK0F, A6 );
Motors moto (5, 3, 10, 6);

void setup() {
#ifdef OUTSERIAL
  Serial.begin(115200);
#endif
}

bool started = false;
int us_dist = 0;
int ir_dist = 0;
int dist = 0;

void loop() {
  while ((!digitalRead(SW_ON)) && (!started)) {}
  started = true;
  us_dist = us.read();
  ir_dist = ir.getDistance();
  if (us_dist < ir_dist) {
    dist = us_dist;
  } else {
    dist = ir_dist;
  }

  if (digitalRead(LINE)) {
    moto.Move(-255, -255);
    delay(1000);
  } else {
    if (dist < SP_DIST) {
      moto.Move(255, 255);
    } else {
      if (digitalRead(SW_DIR)) {
        moto.Move(-255, 255);
      } else {
        moto.Move(255, -255);
      }
    }
  }

  if (digitalRead(SW_ON)) {
    started = false;
    moto.Move(0, 0);
  }
  

  delay(40);
#ifdef OUTSERIAL
  Serial.print("L: ");
  Serial.print(digitalRead(LINE));
  Serial.print(", US: ");
  Serial.print(us_dist);
  Serial.print(", IR: ");
  Serial.print(ir_dist);
  Serial.print(", ON: ");
  Serial.print(digitalRead(SW_ON));
  Serial.print(", DIR: ");
  Serial.print(digitalRead(SW_DIR));
  Serial.println("");
#endif
}
