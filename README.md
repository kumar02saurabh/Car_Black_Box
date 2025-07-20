Car Black Box System
A Car Black Box system, inspired by aviation black boxes, designed to capture and log critical vehicle parameters in real-time, especially moments leading up to a collision. This project enhances vehicle safety and supports post-accident analysis through reliable, non-volatile data retention.

🔧 Tech Stack
Microcontroller: PIC18F4580

IDE: MPLAB IDE

Language: Embedded C

Communication Protocols: I2C, GPIO,UART

Storage: Onboard EEPROM

📌 Key Features
📊 Event Logging:
Logs vital data such as speed, gear status, and time into EEPROM at regular intervals or during key events.

💾 EEPROM Integration:
Leveraged onboard EEPROM for persistent storage to ensure data is preserved even after power loss.

⚙️ Real-Time Data Capture:
Continuously monitors and stores speed, gear, and timestamp data with high reliability — no event loss.

🔌 Peripheral Interface:
Efficient interfacing of buttons (user inputs), EEPROM, and other peripherals using I2C and GPIO lines.
