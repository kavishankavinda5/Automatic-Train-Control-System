![Screenshot 2023-05-16 132822](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/6de19f5b-e13d-407f-823b-af1260392295)

# Automatic Train Control System

A new system proposed as an answer to the problem described above can be introduced as an Automatic Train Control System(ATCS). This system is completely automatic and without a driver the train runs. According to the data and instructions, motion speed and the places where stop is decide. The live location of the train and the path can be observed using mobile
application. By using this system the train is moving safely at high speed.

## Overview
- Wireless communication is used for Automatic Train Control System(ATCS).
- Divide the railway line into several control areas and by transmission of each
control area, the data and instructions transmitted by the radio waves. By using
receivers in the train those data and instructions are catch up.
- The data and instructions received by the receiver are processed by the control
unit and forwarded to the relevant devices.
- Other that these things by using the receivers in the train new update data and
instructions are transmitted to the closet control area. They are speed, train
location, etc. By using GPS, train location can be identify.
- By using the introducing mobile application, we can see the live location of where
the train is and also where the train stopped.
- People who are in the train can know about the places where train stop and they
can hear all the details also.


## Component Diagram

![Components Diagram](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/5bc9e1e0-8975-424f-a8dc-a81c5bcf3867)

## Components Needed

- Arduino UNO & Nano
- NRF24L01 2.4GHz Wireless Transceiver Modules
- L298N Motor Driver
- GPS Module
- GSM Module
- Speed Sensor Module
- DC Motors
- LCD Display
- 12V Power Supply

## Simulator Review
### Train (Receiver)
![20221212_101905](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/3cac8e60-0983-4205-9166-cd8ba52d0ec4)
![20221212_101907](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/20b9deac-8b3f-4276-b518-d2766d17d0b3)
![WhatsApp Image 2023-05-16 at 17 52 44](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/2c9b971d-f751-4918-8fbb-f4ba533aac96)

### Control Unit (Sender)
![20221212_101855](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/be9bafd4-2ef5-46af-b6f7-8fb6d4b26ba5)
![20221212_101857](https://github.com/kavishankavinda5/Automatic-Train-Control-System/assets/75110057/df95a47d-43e1-4a7f-9f2b-d502977aa3b2)


## Related

Here is a related project

[Mobile Application](https://github.com/kavishankavinda5/Live-Railway)

