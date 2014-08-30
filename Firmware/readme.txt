======= Aplicación Firmware para tarjetas de Elibatt =======

Estructura de los directorios:

  +Hardware
     ...
  +Firmware
     +firmware_master : contiene el .ino que se carga en la tarjeta master
     +firmware_slave  : contiene el .ino que se carga en la tarjeta slave
     +libraries       : contiene las distintas librerías hechas para Elibatt y algunas
                        otras descargadas desde internet, no se utilizan directorios fuera
                        de Firmware
     +pruebas         : contiene los .ino de cada prueba particular (en general son
                        pequeños de forma que todo el setup y loop se invoque en código
                        de las librerías)

Los archivos .ino están programados de tal forma que se utilice la menor cantidad de código
posible en el .ino (los #include de las librerías y un par de llamadas). Sirven de ejemplo
de uso de las librerías y contienen los parámetros de cada librería, como los pines de
funcionamiento, tiempos de espera, etc.

Los archivos .ino contienen solo rutas relativas dentro de Firmware, por ejemplo, para un
test del led 13 de arduino, habría que crear el archivo Firmware/pruebas/led13/led13.ino
y dentro acceder a la librería LedTester de la siguiente forma (la librería es ficticia,
para propósitos educativos):

  #include ".../.../libraries/LedTester/led_tester.h"
  LedTester lt;
  void setup() {
    lt.preparePin(13);
  }
  void loop() {
    lt.test();
  }