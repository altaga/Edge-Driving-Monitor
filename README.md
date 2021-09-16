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
- [Arduino Setup and Sketch Compilation:](#arduino-setup-and-sketch-compilation)
- [Energy Consumption:](#energy-consumption)
- [M5 Display.](#m5-display)
- [Prototype Images:](#prototype-images)
- [EPIC DEMO:](#epic-demo)

# Introduction:

As a driver, it’s quite important for you to know where the blind spots are on your own vehicle as well as other drivers' vehicles. Knowing this will help protect you and those around you from an easily-avoidable accident (no one wants to get sideswiped, really). 

<img src="./Images/blind.png">

Blind spots are the areas to the sides of your car that can’t be seen in your rear mirror or side mirrors.

Despite being very noticeable in trucks (1), cars also have this type of blind spots, which can generate up to 800,000 accidents per year (2).

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

- EdgeDriverMonitor. [Link](https://studio.edgeimpulse.com/public/46428/latest)

# Connection Diagram:

This is the connection diagram of the system:

<img src="./Images/diagram.png">

# Edge Impulse:

To be able to use Machine Learning in a microcontroller, you have to decide the framework to use, in this case the service offered by [Edge Impulse] (https://www.edgeimpulse.com/) was used to accelerate the development of the solution.

<img src="./Images/edge.png">

The first thing we must do is put forward what we want to detect with our model, in this case it would be cars, people and motorcycles. Therefore the first stage is to obtain the dataset to feed the model.

The project is public, so you can analyze the dataset and use it as you like. [LINK](https://studio.edgeimpulse.com/public/46428/latest)

<img src="./Images/dataset.png">

For my Impulse, I uploaded 1224 images in total, being 958 for train and 266 for test, with the labels Cars, Motorcycle and Pedestrians.

<img src="./Images/labels.png">

The next thing will be to configure the Impuse that is going to be created, this in order to be able to read it from the ESP32 camera and be able to send it to the neural network.

<img src="./Images/impulse.png">

NOTE: DO NOT CHANGE THESE SETTINGS IF YOU ARE GOING TO REPLICATE THIS PROJECT ON AN ESP32 CAM.

Now we must go to the Image tab and generate the features of the model.

Parameters:
<img src="./Images/image1.png">
Features:
<img src="./Images/image2.png">

Now we must configure the neural network that is going to be trained with our data, in this case Edge Impulse uses the transfer learning technique to facilitate the training of the networks, so for this example it is convenient to use a MobileNetV2 0.05

<img src="./Images/transfer.png">

Don't forget to press the Start Training button.

<img src="./Images/result.png">

Remember that having a network that obtains values very close to 100% in training can be a sign of data overfitting, so always avoid this from happening, however for that we will have to test the model in the Model testing tab.

<img src="./Images/testing1.png">

Finally, since we saw that the model works, it does not fall into overfitting or underfitting, we must choose the device where we are going to display the model for its use, since I use the model for ESP32 and I will use the Arduino IDE, I will select from the list that library.

<img src="./Images/deploy1.png">

In the section below we will be able to build and download the model, Edge Impuse recommends leaving the EON Compiler enabled, so we will just leave it like that, especially do not forget to use the Quantized Model option for better performance.

<img src="./Images/deploy2.png">

Now we will follow the instructions to add the project to Arduino IDE, you can use the version directly from Edge Impuse or use the version [ei-edgedrivermonitor-arduino-1.0.3.zip] (https://github.com/altaga/Edge-Driving-Monitor/blob/main/Once we have the library downloaded, we will open the example in the [Edge_Impulse_ESP32_Cam] folder (https://github.com/altaga/Edge-Driving-Monitor/tree/main/Edge_Impulse_ESP32_Cam), once open we will add the .zip library as shown shown in the picture.
ei-edgedrivermonitor-arduino-1.0.3.zip) that I use in my project.

<img src="./Images/deploy3.png">

# Arduino Setup and Sketch Compilation:

Once we have the library downloaded, we will open the example in the [Edge_Impulse_ESP32_Cam] folder (https://github.com/altaga/Edge-Driving-Monitor/tree/main/Edge_Impulse_ESP32_Cam), once open we will add the .zip library as shown shown in the picture.

<img src="./Images/arduino1.png">

Once the library is added, we will select the correct board for which we are going to compile, in this almost the ESP32 CAM.

<img src="./Images/arduino2.png">

Already selected the board now if you select the serial port of the board and flash the code.

<img src="./Images/arduino3.jpg">

If the compilation succeeds we should see the following result in the IDE.

<img src="./Images/arduino4.png">

# Energy Consumption:

The code tried to take care of the ESP32's power consumption as much as possible, so the considerations to keep the consumption to a minimum were:

1. Use an inference network like the one Edge Impulse displays.
2. Send the microcontroller to Deep sleep while it is not making the inference in the neural network, the deep sleep is generated by a timer and you can lower or raise it according to your needs, in my case the device falls asleep for 2 seconds between readings.

Here is an analysis of the energy consumed by the device in 10 min, I will speed up the video 10 times so that you can see the entire analysis in 1 min, this analysis was performed with the Nordic Power Profiler Kit II.

Video: Click on the image
[![PPK](./Images/ppk1.jpg)](https://youtu.be/to5gXl_dCmc)
Sorry github does not allow embed videos.

With this analysis we can extrapolate the energy consumption that would be 50mAh, the analysis data will be in the file: [ppk-20210904T064657.csv](https://github.com/altaga/Edge-Driving-Monitor/blob/main/Data/ppk-20210904T064657.csv) para que puedas analizar el consumo mas a detalle.

<img src="./Images/ppk2.png">

# M5 Display.

As part of the solution it was decided that to show the results of the device a screen or some device with a display was necessary, it is not necessary to use a screen if you do not require it, it could be only an LED and thus keep energy consumption even lower , but for demonstration purposes the M5core2 was used and the serial communication was used to send the results from the ESP32 to the M5core2.

ESP32 Cam: Get Image, Analyze and Sleep.

    fb = esp_camera_fb_get();
    if (!fb) {
      esp_deep_sleep_start();
    }
    else {
      Serial.println(classify());
      Serial.flush();
    }
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // 2 seconds
    esp_deep_sleep_start();


M5Core2: Display result.

    String serial = Serial2.readString();
    if (serial.indexOf("Start") != -1) {  // Filter Noise from ESP32 Cam Module Sleep
      flag = true;
    } 
    else if (flag) { // Convert Serial to Float Array
      serial = serial.substring(serial.indexOf("Star") + 4, serial.indexOf("Stop"));
      float array[3] = { serial.substring(0, serial.indexOf(",")).toFloat(), serial.substring(serial.indexOf(",") + 1, serial.indexOf(",", serial.indexOf(",") + 1)).toFloat(), serial.substring(serial.indexOf(",", serial.indexOf(",") + 1) + 1).toFloat() 
      };

Communication as indicated was done by serial from ESP32 (GPIO3 RX and GPIO1 TX) to PORTC (Serial2) of M5core2

<img src="./Images/m5esp.jpg">

I did some static tests on the device to make sure the network was working properly.

Car:
<img src="./Images/test1.jpg">

Pedestrian:
<img src="./Images/test2.jpg">

Motorcycle:
<img src="./Images/test3.jpg">

# Prototype Images:

<img src="./Images/product1.jpg">
<img src="./Images/product2.jpg">
<img src="./Images/product3.jpg">

# EPIC DEMO:

[![DEMO](./Images/logo.png)](https://youtu.be/3sFcXpSt5cY)
