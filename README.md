
**1. User connects to display's WiFi → 2. Opens web browser → 3. Types message → 4. Clicks Update → 5. Message scrolls on LED panel**

---

## ✨ Features

### Core Features
- 🖥️ **High-Brightness P10 LED Panel** - Visible even in daylight
- 📱 **WiFi Control** - No internet required, creates its own network
- 🌐 **Mobile-Responsive Web Interface** - Works on any device
- 💾 **Persistent Storage** - Messages saved even after power loss
- 🔄 **Smooth Scrolling** - Professional-looking text animation
- ⚡ **Real-Time Updates** - Messages appear instantly
- 🔌 **Plug & Play** - Simple 7-wire connection
- 📡 **Up to 10m Range** - Control from across the room

### Technical Features
- 🎨 **Multiple Font Support** - Arial Black, System Fonts
- ⚙️ **Adjustable Scroll Speed** - Customize animation speed
- 🔧 **Expandable Design** - Support for multiple panels
- 💪 **Robust Error Handling** - Auto-recovery on errors
- 📊 **Client Counter** - Shows connected devices
- 🔐 **Secure Access** - Password-protected WiFi

---

## 🎯 Applications & Use Cases

### Educational Institutions
- 🏫 **College Notice Boards** - Display exam schedules, holidays, events
- 📚 **Department Updates** - Share project deadlines, seminar info
- 🎓 **Exam Hall Instructions** - Show real-time exam guidelines

### Corporate & Business
- 🏢 **Company Announcements** - Internal communications
- 📊 **KPI Displays** - Show metrics, targets, achievements
- 🎉 **Event Information** - Conference schedules, meeting rooms

### Public Spaces
- 🏥 **Hospitals** - Patient information, department directions
- 🏪 **Retail Stores** - Promotions, offers, announcements
- 🏛️ **Government Offices** - Public notices, citizen information

### Events & Venues
- 🎪 **Conferences** - Session schedules, speaker info
- 🎵 **Concerts** - Set times, announcements
- ⚽ **Sports Venues** - Scores, updates, sponsors

---

## 🖥️ Hardware Requirements

### Essential Components

| Component | Quantity | Specifications |
|-----------|----------|----------------
| **Arduino UNO** | 1 | R3, ATmega328P 
| **ESP32 Dev Board** | 1 | ESP32-WROOM-32, 38-pin 
| **P10 LED Panel** | 1+ | 32x16 or 32x64 dot matrix 
| **5V Power Supply** | 1 | 2A minimum (3A recommended) 
| **Jumper Wires** | 10+ | Female-to-female, Male-to-female 
| **USB Cables** | 2 | A to B (Arduino), Micro USB (ESP32) 

### Optional Components

| Component | Purpose |
|-----------|---------|
| **Logic Level Converter** | For 5V/3.3V isolation (optional, direct works) |
| **Enclosure/Case** | Professional installation |
| **External Antenna** | Extended WiFi range (if ESP32 has IPEX connector) |
| **Heat Sink** | For ESP32 if in hot environment |
| **Power Distribution Board** | Clean power distribution |

### Total Estimated Cost
- **Basic Setup (1 panel):** $60-80
- **Professional Setup (2-3 panels):** $100-150

---

## 📦 Software Requirements

### Development Environments
| Software | Version | Purpose |
|----------|---------|---------|
| **Arduino IDE** | 1.8.19+ | Code upload to Arduino & ESP32 |
| **ESP32 Board Package** | 2.0.14+ | ESP32 support in Arduino IDE |
| **Git** | Latest | Version control (optional) |

### Required Libraries

#### For Arduino UNO
| Library | Installation Method | Version |
|---------|-------------------|---------|
| **DMD** | Library Manager → "DMD by Freetronics" | 1.0+ |
| **TimerOne** | Library Manager → "TimerOne by Paul Stoffregen" | 1.1+ |
| **SPI** | Built-in | - |
| **SoftwareSerial** | Built-in | - |
| **EEPROM** | Built-in | - |

#### For ESP32
| Library | Installation Method | Version |
|---------|-------------------|---------|
| **WiFi** | Built-in | - |
| **WebServer** | Built-in | - |
| **ESPmDNS** | Built-in | - |
| **EEPROM** | Built-in | - |

### Font Files (for DMD library)
- `Arial_Black_16.h` - Large bold font
- `Arial_Black_16_ISO_8859_1.h` - International characters
- `SystemFont5x7.h` - Small 5x7 pixel font

---

## 🔌 Wiring Instructions

### ⚠️ IMPORTANT SAFETY NOTES
- **Always disconnect power** before making connections
- **Double-check connections** before powering on
- **Use appropriate wire gauge** for power lines
- **Common ground** is essential for all components
- **Power P10 panel separately** for best brightness

### Connection 1: P10 Panel → Arduino UNO

| P10 Panel Pin | Label | Arduino UNO Pin | Wire Color | Function |
|---------------|-------|----------------|------------|----------|
| **Pin 1** | S / CLK | **Pin 13** | Yellow | SPI Clock |
| **Pin 2** | R (Data) | **Pin 11** | Green | Data Out (MOSI) |
| **Pin 3** | nOE / OE | **Pin 9** | Blue | Output Enable |
| **Pin 4** | L / SCLK | **Pin 8** | Orange | Latch Clock |
| **Pin 5** | B | **Pin 7** | Brown | Row Select B |
| **Pin 6** | A | **Pin 6** | Red | Row Select A |
| **Pin 7** | GND | **GND** | Black | Ground |

**Visual Connection Diagram:**

P10 PANEL ARDUINO UNO
┌──────────────┐ ┌──────────────┐
│ 1 S/CLK (Y)──┼──────────────────►│ Pin 13 │
│ 2 R (G)──┼──────────────────►│ Pin 11 │
│ 3 nOE (B)──┼──────────────────►│ Pin 9 │
│ 4 L (O)──┼──────────────────►│ Pin 8 │
│ 5 B (BR)─┼──────────────────►│ Pin 7 │
│ 6 A (R)──┼──────────────────►│ Pin 6 │
│ 7 GND (BK)─┼──────────────────►│ GND │
└──────────────┘ └──────────────┘

text

### Connection 2: ESP32 → Arduino UNO

| ESP32 Pin | Pin Label | Arduino UNO Pin | Wire Color | Function |
|-----------|-----------|-----------------|------------|----------|
| **GPIO17** | TX2 | **Pin 10** | Purple | Data from ESP32 to Arduino |
| **GND** | GND | **GND** | Black | Common Ground |

**ESP32 Pinout Reference:**
ESP32 DEV BOARD (Top View)
┌─────────────────────────────────────┐
│ 3V3 EN VP VN D34 D35 D32 │
│ D23 D22 TX0 RX0 D21 D19 D18 D5 │
│ D17 D16 D4 D0 D2 D15 D8 D7 │
│ D6 D9 D10 D11 D12 D13 D14 D15 │
│ GND D25 D26 D27 D14 D12 D13 GND │
│ VIN D33 D25 D26 D27 D14 D12 GND │
│ ↑ │
│ GPIO17 (TX2) here │
└─────────────────────────────────────┘

text

### Connection 3: Power Distribution
EXTERNAL 5V POWER SUPPLY (2-3A)
┌─────────────────────────────┐
│ │
│ +5V ───┬────┬────┬────┐ │
│ │ │ │ │ │
│ GND ───┼────┼────┼────┼─ │
│ │ │ │ │ │
└─────────┼────┼────┼────┼────┘
│ │ │ │
▼ ▼ ▼ ▼
┌─────┐┌─────┐┌─────┐
│P10 ││P10 ││P10 │
│Panel││Panel││Panel│
│1 ││2 ││3 │
└─────┘└─────┘└─────┘

ARDUINO UNO: Powered via USB
ESP32: Powered via USB

text

### Complete System Diagram
EXTERNAL 5V POWER
│
┌─────────────┼─────────────┐
│ │ │
▼ ▼ ▼
┌─────────┐ ┌─────────┐ ┌─────────┐
│ P10 │ │ P10 │ │ P10 │
│ Panel 1 │ │ Panel 2 │ │ Panel 3 │
└────┬────┘ └────┬────┘ └────┬────┘
│ │ │
└─────────────┼─────────────┘
│ (SPI)
▼
┌─────────────┐
│ Arduino │
│ UNO │
│ │
│ Pin 10 ◄────┼───┐
│ GND ◄───────┼───┼─┐
└─────────────┘ │ │
│ │
(Serial)
│ │
▼ ▼
┌─────────────┐
│ ESP32 │
│ GPIO17 TX │
│ GND │
└─────────────┘
│
(WiFi AP)
│
▼
┌─────────────┐
│ Phone/ │
│ Laptop │
└─────────────┘

text

---

## 🚀 Installation Guide

### Step 1: Install Arduino IDE

**Windows:**
```bash
1. Download from https://www.arduino.cc/en/software
2. Run installer (arduino-ide.exe)
3. Follow installation wizard
4. Allow driver installation
macOS:

bash
1. Download .dmg file
2. Drag Arduino to Applications folder
3. Open from Applications
Linux (Ubuntu/Debian):

bash
sudo apt update
sudo apt install arduino
Step 2: Install ESP32 Board Support
bash
1. Open Arduino IDE
2. Go to File → Preferences
3. Add to "Additional Boards Manager URLs":
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
4. Click OK
5. Tools → Board → Boards Manager
6. Search "ESP32"
7. Install "ESP32 by Espressif Systems" (version 2.0.14+)
8. Wait for installation (5-10 minutes)
Step 3: Install Required Libraries
Method 1: Through Library Manager (Recommended)

bash
1. Sketch → Include Library → Manage Libraries
2. Search and install:
   - "DMD by Freetronics"
   - "TimerOne by Paul Stoffregen"
3. Close Library Manager
Method 2: Manual Installation

bash
# Create libraries folder if not exists
mkdir -p ~/Arduino/libraries

# Clone DMD library
cd ~/Arduino/libraries
git clone https://github.com/freetronics/DMD.git

# Clone TimerOne library
git clone https://github.com/PaulStoffregen/TimerOne.git
Step 4: Add Font Files
bash
# Fonts are included in DMD library
# Verify they exist:
ls ~/Arduino/libraries/DMD/fonts/

# Should show:
# Arial_Black_16.h
# Arial_Black_16_ISO_8859_1.h
# SystemFont5x7.h
Step 5: Download Project Files
bash
# Clone repository
git clone https://github.com/YOUR_USERNAME/P10-WiFi-Message-Display.git
cd P10-WiFi-Message-Display

# Or download ZIP from GitHub
Step 6: Make Hardware Connections
bash
1. Power off all components
2. Connect P10 panel to Arduino UNO (7 wires)
3. Connect ESP32 to Arduino UNO (2 wires)
4. Connect power supply to P10 panel
5. Connect Arduino via USB
6. Connect ESP32 via USB
7. Power on system
Step 7: Configure WiFi Credentials (Optional)
If you want to change default WiFi:

Edit ESP32 code (P10_Display_ESP32.ino):

cpp
// Lines to modify:
const char* ap_ssid = "YOUR_WIFI_NAME";
const char* ap_password = "YOUR_PASSWORD";
📤 Uploading Code
Upload to Arduino UNO
bash
1. Open Arduino IDE
2. File → Open → Select "P10_Display_Arduino.ino"
3. Tools → Board → Arduino Uno
4. Tools → Port → Select COM port (Windows) or /dev/ttyUSBx (Linux)
5. Verify code compiles (Checkmark icon)
6. Click Upload (→) button
7. Wait for "Done uploading" message
Expected output:

text
Compiling sketch...
Uploading...
Done uploading.
Upload to ESP32
bash
1. Open Arduino IDE (new window)
2. File → Open → Select "P10_Display_ESP32.ino"
3. Tools → Board → ESP32 Arduino → ESP32 Dev Module
4. Tools → Port → Select COM port (different from Arduino)
5. Select: Tools → Partition Scheme → "Huge App (3MB No OTA)"
6. Verify code compiles
7. Click Upload (→) button
8. If upload fails, press and hold BOOT button on ESP32 while uploading
Troubleshooting ESP32 Upload:

text
If you see "Connecting....____...." error:
1. Press and hold BOOT button
2. Click Upload
3. When "Connecting..." appears, release BOOT button
4. Upload should proceed
Verify Communication
After both uploads complete:

bash
1. Open Serial Monitor for Arduino (Tools → Serial Monitor)
2. Set baud rate to 9600
3. You should see: "P10 LED Display Controller - Arduino UNO"
4. Open Serial Monitor for ESP32 (different window)
5. Set baud rate to 115200
6. You should see: "P10 LED Display Controller - ESP32"
📱 Usage Guide
Step 1: Power On System
Ensure all connections are secure

Apply power to P10 panel

Connect Arduino USB

Connect ESP32 USB

Watch display for "BOOTING..." message

Wait for ESP32 to start (blue LED blinking)

Step 2: Connect to WiFi
On your phone/laptop:

text
Settings → WiFi → Available Networks
Select: SISTK_CSE_DISPLAY
Password: 1001 000 000 101 01 10
Wait for "Connected" status
Step 3: Open Web Interface
Choose one of these methods:

Method 1: Using IP Address

text
Open browser → Type: http://192.168.4.1
Press Enter
Method 2: Using mDNS (if supported)

text
Open browser → Type: http://csedept.local
Press Enter
Method 3: Scan QR Code (if displayed)

text
Use phone camera to scan QR code (if implemented)
Step 4: Send Your First Message
Web interface loads showing current message

Type your message in textarea (max 250 characters)

Click "UPDATE DISPLAY" button

Watch the message scroll on P10 panel!

Example messages to try:

text
Welcome to SIST CSE Department
Exam Schedule: December 15-20, 2024
Happy Holidays!
Workshop on IoT Tomorrow at 10 AM
Project Submission Deadline: Friday
Step 5: Quick Tips
Ctrl+Enter (or Cmd+Enter on Mac) - Submit message

Message appears instantly - No delay

Power loss - Last message is saved

Disconnect WiFi - Display continues scrolling last message

🌐 Web Interface Guide
Interface Layout
text
┌─────────────────────────────────────────────┐
│         📺 SIST CSE DISPLAY                 │
│         Digital Notice Board Controller      │
├─────────────────────────────────────────────┤
│  📝 CURRENT MESSAGE                          │
│  ┌─────────────────────────────────────┐    │
│  │ Welcome to SIST CSE Department       │    │
│  └─────────────────────────────────────┘    │
├─────────────────────────────────────────────┤
│  ✏️ ENTER NEW MESSAGE                        │
│  ┌─────────────────────────────────────┐    │
│  │                                     │    │
│  │ Type your message here...           │    │
│  │                                     │    │
│  └─────────────────────────────────────┘    │
│                                               │
│         [🚀 UPDATE DISPLAY]                  │
├─────────────────────────────────────────────┤
│  📡 WiFi SSID: SISTK_CSE_DISPLAY             │
│  🔗 Web Access: http://csedept.local         │
│  🌐 IP Address: 192.168.4.1                 │
│  👤 Developer: N Poorna Prakash              │
├─────────────────────────────────────────────┤
│  Dept. of Computer Science and Engineering   │
│  (with Specialization in IoT & Automation)   │
└─────────────────────────────────────────────┘

📊 Technical Specifications
Hardware Specifications
Component	Specification
Microcontroller (Arduino)	ATmega328P, 16MHz, 2KB RAM, 32KB Flash
Microcontroller (ESP32)	Xtensa LX6, 240MHz, 520KB RAM, 4MB Flash
Display Resolution	32 x 16 pixels per panel (expandable)
Display Technology	SMD 2121 RGB LED
Refresh Rate	60Hz (smooth scrolling)
Brightness	2000+ nits (readable in sunlight)
Viewing Angle	160° horizontal, 120° vertical
Operating Voltage	5V DC
Current Draw	2A (1 panel), 3A (2 panels), 5A (4 panels)


*Communication Specifications*
Interface	Speed	Purpose
SPI (Arduino to P10)	4MHz	Display data transfer
Serial (ESP32 to Arduino)	9600 baud	Message transmission
WiFi (Phone to ESP32)	2.4GHz, 802.11 b/g/n	Web interface access
Range Specifications
Condition	Range (WiFi)	Range (Serial)
Line of sight	Up to 100m	Up to 10m
Through walls	20-30m	Up to 5m
With external antenna	Up to 200m	-

*Power Specifications*
Component	Voltage	Current (Idle)	Current (Active)
Arduino UNO	5V	50mA	200mA
ESP32	5V	100mA	250mA
P10 Panel (1 panel)	5V	500mA	2000mA
P10 Panel (2 panels)	5V	1000mA	3000mA
Memory Usage
Component	Program Memory	Dynamic Memory
Arduino UNO	28KB (87%)	1.2KB (58%)
ESP32	850KB (21%)	180KB (35%)
