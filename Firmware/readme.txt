======= Aplicación Firmware para tarjetas de Elibatt =======

Estructura de los directorios:

  +Hardware
     ...
  +Firmware
     +FirmwareMaster : contiene el .ino que se carga en la tarjeta master
     +FirmwareSlave  : contiene el .ino que se carga en la tarjeta slave
     +hardware       : contiene definiciones adicionales de tarjetas de arduino (no se usa en
                       E-libatt)
     +libraries      : contiene las distintas librerías hechas para Elibatt y algunas
                       otras descargadas desde internet, no se utilizan directorios fuera
                       de Firmware
     +pruebas        : contiene los .ino de cada prueba particular (en general son
                       pequeños de forma que todo el setup y loop se invoque en código
                       de las librerías)

Los archivos .ino están programados de tal forma que se utilice la menor cantidad de código
posible en el .ino (los #include de las librerías y un par de llamadas). Sirven de ejemplo
de uso de las librerías y contienen los parámetros de cada librería, como los pines de
funcionamiento, tiempos de espera, etc.

Los archivos .ino contienen solo librerías de Arduino. Revisar
  http://arduino.cc/en/pmwiki.php?n=Hacking/LibraryTutorial
para más detalles de cómo crear estas librerías. Referirse a libraries/readme.txt para detalles de
cómo configurar Arduino para poder cargar el firmware a una tarjeta arduino.
