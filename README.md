# Smart-Traffic-Light-System

Our Smart Traffic Light System leverages technology to improve traffic outcomes by introducing a sensing network, which provides feedback to the existing network, so that it can adapt to the changing traffic density patterns and provide necessary signals to the controller in real-time.
  A robust system is able to:
1. Collect data from vehicle detector devices at each intersection
2. Dynamically adapt traffic signal timing in real time
3. Make incremental adjustments to the traffic signal timing based on the changes in traffic flow at each intersection

# Our Prototype:
 We have developed a working model to demonstrate the efficiency of our idea.
In prototype we have used:
1. Ultrasonics Sensors: To detect the density of traffic in various lanes.
2. RFID Tags: To detect the Emergency Vehicles.
3. Arduino Mega: To process the input data and and control the traffic lights.
 
 But in real life, we will be using:
1. Cameras and Image Processing: To detect the density of traffic and emergency vehicles.
2. Servers: For high speed image processing and controlling the lights.

# Functioning:
1. Traffic Density Distribution:- Ultrasonic sensors have been used for detecting traffic density.
2. Switching of traffic lights:- If traffic density is high in any lane, system will increase the green light time for that lane. And when the density is not that much the orange light will glow.
3. Exit of Emergency vehicles:- We have added a clearance mechanism for emergency vehicles like ambulance and fire engines which basically works on RFID tags. RFID tags are allotted to a emergency vehicles. As soon as the RFID reader detects emergency vehicle, system will display the message on LCD screens to ask people to make way. And the system will also turn on the green light of that lane to make way.

# Advantages:
1. We have added a clearance mechanism for emergency vehicles.
2. The inefficient traffic light switching in quad-lane system can be rectified with our project.
3. Unnecessary man power which was a disadvantage in existing system can be eliminated with this project.
4. Traffic lights will switch based on real time Traffic density feedback.

## Author
This Supermarket Management System is created and maintained by [Narayan Pandey](https://github.com/Thelooper9818).
