#include <BitbloqUS.h>
#include <Servo.h>

const int pulsador = 2;
const int sensor_fin = 3;
const int led_fin = 5;
const int dist_limite_obsaculo = 20;

int velocidad_robot = 180;
int distancia_obstaculo = 0;
bool anterior_giro_dcha = false;

volatile int estado_robot = 0;

Servo servo_rueda_derecha;
Servo servo_rueda_izquierda;
Servo servo_sensor_ultrasonidos;
US detector_ultrasonidos(7, 7);

// Función que cambia el estado del robot cuando es pulsado el pulsador
void cambio_estado_robot() {
  //sólo activaremos el robot cuando el estado esté a 0.
  // En cualquier otro caso, tras pulsación dejo el estado a 0.
  if (estado_robot == 0)
    estado_robot = 1;
  else {
    estado_robot = 0;
  }
}

//Función que cambia el estado del robot cuando detecta el final del camino
void deteccion_fin() {
  // en cuanto haya un cambio en la detección del sensor, pasamos al estado 4 de fin,
  // siempre y cuando no estemos en un estado 0, ya que no tiene sentido.
  if (estado_robot != 0) {
    estado_robot = 4;
  }
}

// Función que asigna una velocidad concreta a cada una de las ruedas durante un tiempo.
//  giro_rueda_derecha: ángulo de rotación del servo para la rueda derecha.
//  giro_rueda_izquierda: ángulo de rotación del servo para la rueda izquierda
//  tiempo: cantidad de tiempo (delay) que estará moviendo la rueda.
void mover_robot(int giro_rueda_derecha, int giro_rueda_izquierda, int tiempo) {
  servo_rueda_derecha.write(giro_rueda_derecha);
  servo_rueda_izquierda.write(giro_rueda_izquierda);
  delay(tiempo);
}

void setup() {
  //Declaración de las interrupciones
  attachInterrupt(digitalPinToInterrupt(2), cambio_estado_robot, FALLING);
  attachInterrupt(digitalPinToInterrupt(3), deteccion_fin, CHANGE);

  //establecer el modo de los pines
  pinMode(sensor_fin, INPUT);
  pinMode(led_fin, OUTPUT);
  pinMode(pulsador, INPUT);

  //establecer los servos
  servo_rueda_izquierda.attach(12);
  servo_rueda_derecha.attach(13);
  servo_sensor_ultrasonidos.attach(11);

  digitalWrite(led_fin, LOW);

  // por defecto dejo el servo del sensor a 90º
  servo_sensor_ultrasonidos.write(90);
}

void loop() {
  switch (estado_robot) {
    case 0:
      //robot parado/apagado. Dejamos los servos parados y apagamos el led
      mover_robot(90, 90, 200);
      digitalWrite(led_fin, LOW);
      break;
    case 1:
      //mover hacia adelante hasta encontrar obstáculo y decidir siguiente movimiento.
      servo_sensor_ultrasonidos.write(90);
      delay(500);
      distancia_obstaculo = detector_ultrasonidos.read();
      //hay que controlar también el estado del robot por si la IRQ ha saltado en medio del bucle.
      while ((distancia_obstaculo >= dist_limite_obsaculo) && (estado_robot == 1)) {
        mover_robot(0, velocidad_robot, 200);
        distancia_obstaculo = detector_ultrasonidos.read();
      }
      mover_robot(90, 90, 100);
      // decisión alterna de giro comprobando previamente si no ha habido una IRQ en el bucle.
      if ((estado_robot != 0) && (estado_robot != 4)) {
        if (not anterior_giro_dcha) {
          estado_robot = 2;
        } else {
          estado_robot = 3;
        }
      }
      break;
    case 2:
      // Mover el sensor a 175º para ver si hay obstáculo. Si no hay obstáculo, girar robot e ir
      // al caso 1. Si lo hay, ir al caso 3.
      servo_sensor_ultrasonidos.write(175);
      delay(500);

      if (detector_ultrasonidos.read() >= dist_limite_obsaculo) {
        mover_robot(90, velocidad_robot, 300);
        anterior_giro_dcha = true;
        estado_robot = 1;
      } else {
        estado_robot = 3;
      }
      servo_sensor_ultrasonidos.write(90);
      delay(500);
      break;
    case 3:
      // Mover el sensor a 0º para ver si hay obstáculo. Si no, girar robot e ir al caso 1.
      // Si lo hay, ir al caso 2.
      servo_sensor_ultrasonidos.write(5);
      delay(500);
      if (detector_ultrasonidos.read() >= dist_limite_obsaculo) {
        mover_robot(0, 90, 300);
        anterior_giro_dcha = false;
        estado_robot = 1;
      } else {
        estado_robot = 2;
      }
      servo_sensor_ultrasonidos.write(90);
      delay(500);
      break;
    case 4:
      mover_robot(90, 90, 110);
      //Si me encuentro en el final, enciendo el led, pero si la pierdo, apago el robot yendo al estado 0.
      if (digitalRead(sensor_fin)) {
        digitalWrite(led_fin, HIGH);
      } else {
        estado_robot = 0;
      }
      break;
  }
}
