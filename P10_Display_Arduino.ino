/*
 * ============================================================================
 * SCRATCH-FREE SCROLLING LED MESSAGE DISPLAY - ARDUINO UNO CODE
 * ============================================================================
 * 
 * Project: Scrolling LED Message Display with P10 Panel
 * Author: N Poorna Prakash
 * Department: CSE (IoT & Automation)
 * 
 * Description: 
 *   This code runs on Arduino UNO and controls the P10 LED display panel.
 *   It receives messages from ESP32 via SoftwareSerial and displays them
 *   as smoothly scrolling text.
 * 
 * Hardware Connections:
 *   P10 Panel -> Arduino UNO:
 *   - CLK (S)  -> Pin 13
 *   - R (Data) -> Pin 11  
 *   - OE       -> Pin 9
 *   - L (Latch)-> Pin 8
 *   - B        -> Pin 7
 *   - A        -> Pin 6
 *   - GND      -> GND
 *   
 *   ESP32 -> Arduino UNO:
 *   - GPIO17 (TX) -> Pin 10 (SoftwareSerial RX)
 *   - GND         -> GND
 * 
 * Libraries Required:
 *   - DMD by Freetronics
 *   - TimerOne by Paul Stoffregen
 *   - SoftwareSerial (built-in)
 *   - EEPROM (built-in)
 * 
 * ============================================================================
 */

#include <SPI.h>
#include <DMD.h>
#include <TimerOne.h>
#include "Arial_Black_16.h"
#include "SystemFont5x7.h"
#include <SoftwareSerial.h>
#include <EEPROM.h>

// ============================================================================
// CONFIGURATION
// ============================================================================

// DMD Panel Configuration
#define DISPLAYS_ACROSS 1     // Number of panels horizontally
#define DISPLAYS_DOWN 1       // Number of panels vertically

// Scroll Speed (milliseconds per frame, lower = faster)
#define SCROLL_DELAY_MS 50

// Software Serial Pins (for ESP32 communication)
#define ESP_RX_PIN 10         // Connect to ESP32 TX (GPIO17)
#define ESP_TX_PIN 11         // Not used, but required for SoftwareSerial

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

// Initialize DMD display
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// Initialize Software Serial for ESP32 communication
SoftwareSerial espSerial(ESP_RX_PIN, ESP_TX_PIN);

// Message variables
String currentMessage = "";
String newMessage = "";
unsigned long lastScrollTime = 0;
int scrollPosition = 32;
bool messageUpdated = false;
bool booting = true;

// ============================================================================
// INTERRUPT SERVICE ROUTINE
// ============================================================================

/**
 * Timer interrupt handler for DMD refresh scanning
 * This must be called regularly to refresh the display
 */
void ScanDMD() {
  dmd.scanDisplayBySPI();
}

// ============================================================================
// EEPROM FUNCTIONS
// ============================================================================

/**
 * Read saved message from EEPROM
 * @return String - The stored message
 */
String readMessageFromEEPROM() {
  String message = "";
  int length = EEPROM.read(0);
  
  // Validate length (max 255 chars)
  if (length > 0 && length <= 255) {
    for (int i = 0; i < length; i++) {
      char c = EEPROM.read(i + 1);
      if (c >= 32 && c <= 126) {  // Only printable ASCII
        message += c;
      }
    }
  }
  
  Serial.print("[EEPROM] Read message: ");
  Serial.println(message);
  return message;
}

/**
 * Save message to EEPROM
 * @param message - The message to save
 */
void saveMessageToEEPROM(String message) {
  int length = min(message.length(), 255);
  
  EEPROM.write(0, length);
  for (int i = 0; i < length; i++) {
    EEPROM.write(i + 1, message[i]);
  }
  
  Serial.print("[EEPROM] Saved message: ");
  Serial.println(message);
}

// ============================================================================
// DISPLAY FUNCTIONS
// ============================================================================

/**
 * Display a temporary message on screen
 * @param text - The message to display
 * @param duration - How long to show (milliseconds)
 */
void showTempMessage(String text, int duration) {
  dmd.clearScreen(true);
  dmd.drawString(0, 0, text.c_str(), text.length(), GRAPHICS_NORMAL);
  delay(duration);
  dmd.clearScreen(true);
}

/**
 * Show boot animation sequence
 */
void showBootAnimation() {
  dmd.clearScreen(true);
  
  // Show "SIST" animation
  dmd.drawString(8, 0, "SIST", 4, GRAPHICS_NORMAL);
  delay(500);
  
  dmd.clearScreen(true);
  dmd.drawString(4, 0, "CSE", 3, GRAPHICS_NORMAL);
  delay(500);
  
  dmd.clearScreen(true);
  dmd.drawString(0, 0, "IoT", 3, GRAPHICS_NORMAL);
  delay(500);
  
  dmd.clearScreen(true);
  
  // Show boot message
  dmd.drawString(0, 0, "BOOTING", 7, GRAPHICS_NORMAL);
  delay(1000);
  dmd.clearScreen(true);
}

/**
 * Update the scrolling display
 */
void updateScrollingDisplay() {
  if (currentMessage.length() == 0) {
    currentMessage = "System Ready!";
  }
  
  dmd.clearScreen(true);
  
  // Convert String to char array for DMD library
  char displayText[currentMessage.length() + 1];
  currentMessage.toCharArray(displayText, sizeof(displayText));
  
  // Draw scrolling text
  dmd.drawMarquee(displayText, currentMessage.length(), scrollPosition, 0);
  dmd.stepMarquee(-1, 0);
  
  // Update scroll position
  scrollPosition--;
  
  // Reset position when text has completely scrolled
  if (scrollPosition < -6 * (int)currentMessage.length()) {
    scrollPosition = 32;
    if (messageUpdated) {
      messageUpdated = false;
      Serial.println("[Display] Scroll reset, new message active");
    }
  }
}

// ============================================================================
// SERIAL COMMUNICATION FUNCTIONS
// ============================================================================

/**
 * Check for incoming messages from ESP32
 */
void checkForNewMessages() {
  if (espSerial.available()) {
    String incoming = espSerial.readStringUntil('\n');
    incoming.trim();
    
    if (incoming.length() > 0 && incoming != currentMessage) {
      Serial.println("[ESP32] New message received:");
      Serial.println(incoming);
      
      newMessage = incoming;
      saveMessageToEEPROM(newMessage);
      
      // Show confirmation on display
      showTempMessage("NEW", 300);
      
      // Update current message and reset scroll
      currentMessage = newMessage;
      scrollPosition = 32;
      messageUpdated = true;
      
      Serial.println("[Display] Message updated successfully");
    }
  }
}

// ============================================================================
// SETUP FUNCTION
// ============================================================================

void setup() {
  // Initialize serial for debugging
  Serial.begin(9600);
  Serial.println("\n==========================================");
  Serial.println("P10 LED Display Controller - Arduino UNO");
  Serial.println("Department of CSE (IoT & Automation)");
  Serial.println("==========================================\n");
  
  // Initialize Software Serial for ESP32
  espSerial.begin(9600);
  Serial.println("[Serial] ESP32 communication initialized on pin 10");
  
  // Initialize Timer for DMD refresh (1000 microseconds)
  Timer1.initialize(1000);
  Timer1.attachInterrupt(ScanDMD);
  Serial.println("[Timer] DMD refresh timer initialized");
  
  // Initialize DMD
  dmd.clearScreen(true);
  dmd.selectFont(Arial_Black_16);
  Serial.println("[DMD] Display initialized");
  
  // Show boot animation
  showBootAnimation();
  
  // Load saved message from EEPROM
  currentMessage = readMessageFromEEPROM();
  if (currentMessage.length() == 0) {
    currentMessage = "Welcome to SIST CSE Department";
    saveMessageToEEPROM(currentMessage);
    Serial.println("[EEPROM] Default message saved");
  }
  
  // Send ready signal to ESP32
  espSerial.println("ARDUINO_READY");
  delay(500);
  
  Serial.println("\n[System] Arduino UNO Ready!");
  Serial.print("[Display] Current message: ");
  Serial.println(currentMessage);
  Serial.println("\n==========================================\n");
  
  booting = false;
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Check for new messages from ESP32
  checkForNewMessages();
  
  // Update scrolling display at configured interval
  if (millis() - lastScrollTime > SCROLL_DELAY_MS) {
    updateScrollingDisplay();
    lastScrollTime = millis();
  }
}

// ============================================================================
// END OF CODE
// ============================================================================