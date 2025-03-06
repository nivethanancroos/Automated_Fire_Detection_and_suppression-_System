# Automated_Fire_Detection_and_suppression-_System
An innovative fire detection and suppression system combining multi-sensor fusion and IoT integration for accurate detection, real-time alerts, and automated suppression. Designed for residential, commercial, and industrial safety, it offers cost-effectiveness, scalability, and energy efficiency.

## Circuit Diagram
Include an image of the circuit design here. 


![Alt text](design.png)




## Problem Statement
Current fire detection systems often struggle with false alarms, slow response times, and a lack of real-time alerts. A smarter system is needed to ensure quick detection, reduce false alarms, and provide instant notifications.


## Technologies Used
- Microcontroller: ESP32 (Dual-core processing, built-in Wi-Fi and Bluetooth).
- Sensors:
  - DHT22: Temperature and humidity.
  - MQ-2: Smoke and combustible gas detection.
  - Flame Sensor: Infrared-based flame detection.
- IoT Protocols: MQTT or Firebase for real-time communication.
- Relay Module: Controls suppression systems (e.g., water pump).
- Power Supply: 5V DC adapter with voltage regulators.



## Features
1. Multi-Sensor Fusion: Reduces false positives by cross-verifying data from multiple sensors.
2. Real-Time Monitoring: Sends notifications to mobile apps or dashboards.
3. Automated Suppression: Activates sprinklers or pumps immediately upon fire detection.
4. Customizable Thresholds: Tailored for residential, commercial, or industrial environments.
5. Energy-Efficient Design: Low-power standby mode.
6. Error Handling: Alerts users if sensors malfunction or disconnect.



## Installation and Setup
1. Hardware Setup:
   - Connect components as per the circuit diagram.
   - Ensure power supply is stable.
2. Software Setup:
   - Install the Arduino IDE.
   - Add ESP32 libraries and configure the code.
   - Upload the code to ESP32.
3. IoT Configuration:
   - Set up an MQTT broker or Firebase for notifications.
   - Configure Wi-Fi credentials in the code.
4. Testing:
   - Simulate fire conditions to verify system responses.






PROBLEM AND SOLUTION

	Problem

1.	Fire Hazards in Residential, Commercial, and Industrial Spaces
o	Fires can occur due to electrical faults, gas leaks, human negligence, or external factors like natural disasters.
o	Early detection is critical to prevent fires from spreading and causing extensive damage to life and property.
2.	Limitations of Existing Systems
o	Smoke Alarms: Traditional smoke detectors often trigger false alarms (e.g., due to cooking smoke or dust).
o	Heat Detectors: These respond slowly, as they rely on a significant temperature rise, which may not occur in the initial stages of a fire.
o	Standalone Suppression Systems: Sprinkler systems activate based on temperature thresholds but cannot provide alerts to users in real-time.
3.	Lack of Real-Time Alerts
o	Many fire detection systems lack IoT integration, meaning users are unaware of a fire unless they are physically present.
o	Remote alerts or monitoring could save valuable time in notifying authorities or taking action.
4.	False Positives and Inaccuracies
o	A single sensor (e.g., only smoke or temperature) might misinterpret environmental changes as fire hazards, leading to unnecessary disruptions.
5.	Slow Response Times
o	Delays in detecting and suppressing fires result in rapid fire spread, increasing risks to life and property.

	Solution

1.	Multi-Sensor Fire Detection
o	The system combines inputs from temperature (DHT22), smoke (MQ-2), and flame sensors to accurately detect fire hazards.
o	Sensor fusion improves reliability and reduces false alarms by cross-verifying data from multiple 
sources:
	For instance, smoke alone might not indicate a fire, but smoke combined with high temperature and visible flames would.




2.	Automated Fire Suppression
o	A relay-controlled water pump or sprinkler system automatically activates upon detecting a fire.
o	This reduces dependency on manual intervention, allowing immediate action to suppress fires and limit damage.
3.	Real-Time Notifications with IoT
o	The system uses the ESP32's built-in Wi-Fi to send alerts via MQTT or Firebase to a mobile app or dashboard.
o	Notifications inform users about the fire, enabling remote monitoring and quick decision-making (e.g., alerting emergency services).
4.	Local Alarm for On-Site Alerts
o	An audible buzzer alerts anyone nearby, ensuring immediate attention to the fire hazard.
5.	Customizable Detection Thresholds
o	Thresholds for temperature, smoke concentration, and flame intensity can be customized based on the environment (e.g., industrial spaces may require stricter thresholds).
6.	Energy-Efficient Design
o	The system remains in low-power standby mode, continuously monitoring the environment but consuming minimal energy.
o	When sensors detect abnormal conditions, the system activates, saving energy while ensuring reliability.
7.	Low-Cost and Scalable Solution
o	By leveraging affordable components (e.g., ESP32, DHT22, MQ-2), the design is cost-effective and can be deployed in homes, offices, and factories.
o	The modular design allows for easy expansion (e.g., adding more sensors or actuators).
8.	Enhanced Safety Features
o	Built-in error handling:
	Alerts users if a sensor malfunctions or disconnects.
	Allows manual override via the IoT app, giving users control over suppression systems remotely.
Novelty if you have any

	Multi-Sensor Fusion

•	Uses multiple sensors (DHT22, MQ-2, Flame Sensor) to enhance fire detection reliability by reducing false positives. For example:
o	Smoke without a temperature rise or flame may indicate cooking rather than a fire.
o	Flame detection alone may miss smoldering fires.

	Real-Time Monitoring with IoT

•	Leverages ESP32's Wi-Fi capabilities for cloud connectivity. Sends notifications (via MQTT or Firebase) to mobile devices or dashboards.
	Automated Suppression System

•	Integrates a relay-controlled water pump or sprinkler to quickly suppress the fire once detected, reducing reliance on manual intervention.
	Cost-Effectiveness

•	Provides a low-cost, scalable solution using widely available components, making it suitable for residential, commercial, and industrial applications.
	Energy Efficiency

•	The system remains in low-power standby mode until fire-related activity is detected, optimizing energy consumption.

High-level architecture: mention why you choose a specific technology.

Technology Choices

1.	Microcontroller: ESP32
o	Chosen for its:
	Dual-core processing power to handle sensor data and IoT tasks simultaneously.
	Built-in Wi-Fi and Bluetooth for seamless IoT integration.
	Low cost and wide availability.
2.	Sensors
o	DHT22: For precise temperature and humidity measurements.
o	MQ-2: Detects smoke or combustible gases like LPG and propane.
o	Flame Sensor: Infrared-based detection of flames within a specific wavelength range.
o	These sensors ensure a robust detection system that reduces false alarms.
3.	Relay Module
o	Allows the ESP32 to control high-power devices like water pumps or sprinklers for fire suppression.
4.	IoT Protocols
o	MQTT (Message Queuing Telemetry Transport): Lightweight protocol for real-time notifications to a mobile app or dashboard.
o	Alternatively, Firebase can be used for database storage and push notifications.

5.	Power Supply
o	A 5V DC adapter powers the sensors, ESP32, and relay. Voltage regulators ensure stable operation.



System Workflow

1.	Sensors continuously monitor temperature, smoke, and flame.
2.	If fire conditions are detected (based on thresholds):
o	The relay activates the water pump/sprinkler.
o	The buzzer sounds a local alarm.
o	An IoT message is sent to notify the user via mobile or dashboard.
3.	User can remotely monitor and deactivate the system if needed.

Circuit Design and Protocols

Protocols
1.	Relay Control Protocol
o	Relay operates in a low-level trigger mode, activated by pulling the control pin LOW. This ensures compatibility with ESP32's GPIO logic levels.
2.	Data Processing Algorithm
o	Sensor thresholds:
	Temperature > 60°C
	Smoke (MQ-2 output) above 400 ppm
	Flame detection signal > 50% intensity
o	Combine readings from all sensors to determine fire conditions.
3.	Error Handling
o	Implement fallback mechanisms:
	Alert user if a sensor fails or disconnects.
	Provide manual override options via a mobile app.










