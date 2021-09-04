# Edge-Driving-Monitor

<img src="./Images/logo.png">

# Index
 
- [Edge-Driving-Monitor](#edge-driving-monitor)
- [Index](#index)
- [Introduction:](#introduction)
- [Solution:](#solution)
- [Materials:](#materials)
- [Connection Diagram:](#connection-diagram)
- [Edge Impulse:](#edge-impulse)

# Introduction:

As a driver, it’s super important for you to know where the blind spots are on your own vehicle as well as other drivers' vehicles. Knowing this will help protect you and those around you from an easily-avoidable accident (no one wants to get sideswiped, really). 

<img src="./Images/blind.png">

Blind spots are the areas to the sides of your car that can’t be seen in your rear mirror or side mirrors.

Apesar de ser muy notorio en camiones (1) los autos tambien tienen este tipo de puntos ciegos, los cuales pueden generar hasta 800,000 accidentes al año (2).

1. https://www.fmcsa.dot.gov/ourroads/large-blind-spots
2. https://www.natlawreview.com/article/what-if-my-car-accident-was-caused-blind-spot

# Solution:

We will create a system that will be able to detect small vehicles, pedestrians and motorcycles in the automobile’s blind spots.

<img src="https://thumbsnap.com/s/Wy5w7JPR.jpg?1205" width="1000">

Current Solutions:

- Mercedes-Benz Attention Assist uses the car's engine control unit to monitor changes in steering and other driving habits and alerts the driver accordingly.

- Lexus placed a camera in the dashboard that tracks the driver's face, rather than the vehicle's behavior, and alerts the driver if his or her movements seem to indicate sleep.

- Volvo's Driver Alert Control is a lane-departure system that monitors and corrects the vehicle's position on the road, then alerts the driver if it detects any drifting between lanes.

- Saab uses two cameras in the cockpit to monitor the driver's eye movement and alerts the driver with a text message in the dash, followed by a stern audio message if he or she still seems sleepy.

As you can see these are all premium brands and there is not a single plug and play system that can work for every car. This, is our opportunity as most cars in the road are not on that price range and do not have these systems.

# Materials:

Hardware:

- ESP32 Cam. [Link](https://www.amazon.com/dp/B07S4DRLYM/ref=cm_sw_em_r_mt_dp_GJTGMVVNSE5PY656G3JQ?_encoding=UTF8&psc=1)
- M5core2. [Link](https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit-for-aws-iot-edukit)

Software:

- Edge Impulse. [Link](https://www.edgeimpulse.com/)
- Arduino IDE. [Link](https://www.arduino.cc/en/software)

Project:

EdgeDriverMonitor. [Link](pending)

# Connection Diagram:

This is the connection diagram of the system:

<img src="./Images/diagram.png">

# Edge Impulse:

Para poder utilizar Machine Learning en un microcontrolador hay que decidir el framework a utilizar, en este caso se aprovecho el servicio ofrecido por [Edge Impulse](https://www.edgeimpulse.com/) para acelerar el desarrollo de la solucion.

<img src="./Images/edge.png">

Lo primero que debemos hacer es plantear que es lo que queremos detectar con nuestro modelo, en este caso serian carros, personas y motocicletas. Por lo tanto la primera etapa es obtener el dataset para alimentar al modelo.

El proyecto es publico, asi que puedes analizar el dataset y utilizarlo como gustes. [LINK](https://studio.edgeimpulse.com/studio/46428/versions)

<img src="./Images/dataset.png">

Para mi Impulse, subi 1224 imagenes en total, siendo 958 para train y 266 para test, con las labels Cars, Motorcycle and Pedestrians.

<img src="./Images/labels.png">

Lo siguiente sera configurar el Impuse que se va a crear, esto con el fin de poderlo leer desde la camara del ESP32 y poder mandarlo a la red neuronal.

<img src="./Images/impulse.png">

NOTA: NO CAMBIES ESTOS AJUSTES SI VAS A REPLICAR ESTE PROYECTO EN UN ESP32 CAM.

Ahora debemos pasar a la pestaña de Image y generar los features del modelo.

Parameters:
<img src="./Images/image1.png">
Features:
<img src="./Images/image2.png">

Ahora debemos configurar la red neuronal que va a ser entrenada con nuestros datos, en este caso Edge Impulse utiliza la tecnica de transfer learning para facilitar el entrenamiento de las redes, entonces para este ejemplo es conveniente utilizar una MobileNetV2 0.05

<img src="./Images/transfer.png">

No olvides presionar el boton de Start Training.

<img src="./Images/result.png">

Recordar que tener una red que obtenga valores muy cercanos a 100% en entrenamiento puede ser una señalde overfitting de datos, asi que siempre evitar que ocurra esto, sin embargo para eso deberemos realizar el tesing del modelo en la pestaña de Model testing.

INSERTAR IMAGEN DE MODEL TESTING y resultados.
<img src="./Images/testing1.png">

Ya por ultimo ya que vimos que el modelo funciona, no cae en overfitting ni underfitting, deberemos elegir el device donde vamos a desplegar el modelo para su utilizacion, ya que yo ocupo el modelo para ESP32 y usare el Arduino IDE, seleccionare de la lista esa libreria.

<img src="./Images/deploy1.png">

En la seccion de abajo podremos hacer el build y descargar el modelo, Edge Impuse recomienda dejar enable el EON Compiler, asi que justo asi lo dejaremos, sobre todo no olvides utilizar la opcion de Modelo Quantized para mejor rendimiento.

<img src="./Images/deploy2.png">
IMAGEN DE LA OPTIMIZACION

Ahora seguiremos las instrucciones para agregar el proyecto a Arduino IDE, puedes usar la version directamente desde Edge Impuse o utilizar la version [ei-edgedrivermonitor-arduino-1.0.3.zip]() que yo utilice en mi proyecto.






