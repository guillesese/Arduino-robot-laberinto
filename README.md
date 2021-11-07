# Proyecto de control de un robot con Arduino.
Módulo 3. Asignatura 2. Curso UNIR programación y robótica con Arduino.

## INTRODUCCIÓN
Se busca crear un robot capaz de moverse por un laberinto hasta llegar a un punto determinado. 

## FUNCIONAMIENTO
Tras colocar el robot en el suelo no se pondrá en marcha hasta que se pulse el botón. Por defecto se moverá hacia adelante hasta que encuentre un obstáculo. En ese momento, decidirá cuál será la primera comprobación de giro en función del último realizado, de este modo, el robot irá alternando la comprobación de giro. El movimiento finalizará si se vuelve a pulsar el botón o si el robot llega al punto final del recorrido marcado con una línea negra. 

## ESTRUCTURA DEL CÓDIGO
El código se ha dividido en varias partes, en primer lugar, en la zona superior están las definiciones de las librerías y las variables: 
-	Variables constantes: variables que contienen los pines de conexión de los componentes menos los servos. A parte, también he declarado como constante la distancia límite para considerar un obstáculo, de esta manera sólo se debe modificar un punto del código para hacer los recálculos en vez de tener que revisar cada comprobación de la distancia. 
-	Variables: variables que tienen un valor cambiante, es decir, no constante, como puede ser la velocidad del robot, la distancia desde el lector hasta el obstáculo y la comprobación de si el anterior giro ha sido hacia la derecha. 
-	Variable volátil: contiene la variable del estado del robot que puede ser modificada por las IRQs que se pueden producir en el desarrollo. 
-	Servos y detector de ultrasonidos: Se definen los servos de rotación continua, uno por cada rueda y el servo encargado de girar la cabeza del robot y a su vez, el sensor de ultrasonidos. 

A continuación, están las funciones que se ejecutan cuando se producen las IRQs:
-	Función cambio_estado_robot. Esta función se ejecuta al producirse la IRQ al ser pulsado el botón. Tras la pulsación y en función del estado del robot, cambiará el estado del mismo. 
- Función deteccion_fin. Se ejecuta al producirse la IRQ cuando se produce un cambio en el sensor de infrarrojos. En el caso en el que el sensor detecte el fin del circuito, cambiará el estado del robot al último estado, el de fin. 
- Otra función importante es la de mover_robot. Esta función mueve los servos de rotación continua durante un tiempo determinado. Tanto la velocidad/sentido de giro de cada uno, como el tiempo, son valores que se le pasan a la función como parámetros. 

**Función setup**:
En este punto se inicializan cada uno de los componentes asignándoles los valores de entrada/salida y los pines a los que están conectados. 
Dentro de la función setup se encuentran las dos definiciones de las interrupciones que hay en el código del programa. 
-	La primera IRQ se registra cuando en el pin 2 (botón) se produce un cambio del estado de 1 a 0. En ese momento se ejecutará la función cambio_estado_robot. 
-	La segunda IRQ se registra cuando en el pin 3 (sensor de infrarrojos) hay un cambio de estado independientemente de si es de 1 a 0 o de 0 a 1.

**Función loop**:
En función del estado del robot se ejecutarán una serie de instrucciones.
-	Caso 0. Caso inicial del robot. En este estado, el robot estará parado. Los servos estarán parados, es decir a 90º y deberemos cerciorarnos de que el led de fin sigue apagado. 
- Caso 1. Tras la pulsación del botón, entraremos en este estado. En primer lugar, se obtiene la distancia que hay hasta el obstáculo. Mientras la distancia sea menor que la fijada como límite, deberemos mover el robot hacia adelante comprobando cada segundo la distancia hasta el próximo obstáculo. Junto a la distancia, deberemos también comprobar en el bucle si seguimos en estado 1. 
En cuanto se cumple una de estas dos condiciones paramos el robot y cambiaríamos de estado en función de cuál ha sido el último giro. Esto solo deberíamos comprobarlo en el caso de no haberse producido una IRQ. 
-	Caso 2. En primer lugar, movemos el servo hacia la derecha. Si obtenemos una distancia mayor a la configurada como límite, el robot se giraría hacia este lado y cambiaría de estado. Si se obtiene una distancia inferior o igual, iríamos al caso 3 para comprobar si hay algún obstáculo a la izquierda. Por último, habría que centrar la cabeza del robot.
-	Caso 3. En este caso, el funcionamiento sería similar al caso 2 pero con las particularidades de este caso. 
-	Caso 4. A este caso llegamos cuando se produce la interrupción de detección de fin. En el caso de que el detector detecte que realmente ha llegado al fin, encenderá el led. Sin embargo, cuando el sensor pierda la detección es posible que lo hayamos levantado por lo que el robot pasará a un estado 0. 

### ROBOT ###
Aquí podemos ver cómo ha quedado la maqueta del robot. Para la elaboración de la misma, he utilizado una caja de zapatos de cartón, piezas de lego para la rueda delantera, celo y una pistola termoselladora. 

![image](https://user-images.githubusercontent.com/16133041/140657987-43b2a465-624b-4a81-acf0-eeaf4f2f7fe0.png)

#### VÍDEO ####
Recorrido y explicación del funcionamiento del robot. Vídeo oculto, se ruega la no difusión:
[Vídeo](https://youtu.be/ROgkOrGq1fE)
