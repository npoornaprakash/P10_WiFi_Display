/*
 * ============================================================================
 * SCRATCH-FREE SCROLLING LED MESSAGE DISPLAY - ESP32 CODE
 * ============================================================================
 * 
 * Project: Scrolling LED Message Display with P10 Panel
 * Author: N Poorna Prakash
 * Department: CSE (IoT & Automation)
 * 
 * Description:
 *   This code runs on ESP32 and creates a WiFi Access Point with a web server.
 *   Users can connect to the WiFi and send messages through a web interface.
 *   Messages are sent to Arduino UNO via Serial communication.
 * 
 * WiFi Credentials:
 *   SSID: SISTK_CSE_DISPLAY
 *   Password: 1001 000 000 101 01 10
 *   Web Access: http://192.168.4.1 or http://csedept.local
 * 
 * Hardware Connections:
 *   ESP32 -> Arduino UNO:
 *   - GPIO17 (TX2) -> Arduino Pin 10
 *   - GND          -> Arduino GND
 * 
 * ============================================================================
 */

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <EEPROM.h>

// ============================================================================
// CONFIGURATION
// ============================================================================

// WiFi Access Point Credentials
const char* ap_ssid = "SISTK_CSE_DISPLAY";
const char* ap_password = "1001 000 000 101 01 10";

// mDNS hostname (access via http://csedept.local)
const char* mdns_hostname = "csedept";

// Static IP for Access Point
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// Serial communication pins
#define SERIAL2_RX 16  // GPIO16 (not used, but defined)
#define SERIAL2_TX 17  // GPIO17 - Connect to Arduino Pin 10

// Web server port
WebServer server(80);

// Default message
const char* DEFAULT_MESSAGE = "Welcome to SIST CSE Department";

// ============================================================================
// HTML WEB INTERFACE
// ============================================================================

/**
 * Generate the HTML page for web interface
 * @param currentMessage - Currently displayed message
 * @return String - Complete HTML page
 */
String getHTMLPage(String currentMessage) {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=yes">
    <title>SIST CSE - LED Display Control</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 20px;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
            max-width: 500px;
            width: 100%;
            padding: 30px;
            animation: fadeIn 0.5s ease;
        }
        
        @keyframes fadeIn {
            from {
                opacity: 0;
                transform: translateY(-20px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
        
        h1 {
            color: #333;
            text-align: center;
            margin-bottom: 5px;
            font-size: 24px;
        }
        
        .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 25px;
            font-size: 14px;
            border-bottom: 1px solid #eee;
            padding-bottom: 10px;
        }
        
        .current-message {
            background: linear-gradient(135deg, #667eea15 0%, #764ba215 100%);
            padding: 15px;
            border-radius: 12px;
            margin-bottom: 20px;
            border: 1px solid #667eea30;
        }
        
        .current-message label {
            font-weight: bold;
            color: #667eea;
            display: block;
            margin-bottom: 8px;
            font-size: 14px;
        }
        
        .current-message div {
            color: #333;
            font-size: 16px;
            word-break: break-word;
            font-family: monospace;
        }
        
        .message-input {
            margin-bottom: 20px;
        }
        
        .message-input label {
            display: block;
            margin-bottom: 8px;
            font-weight: bold;
            color: #555;
        }
        
        textarea {
            width: 100%;
            padding: 12px;
            border: 2px solid #e0e0e0;
            border-radius: 12px;
            font-size: 14px;
            font-family: monospace;
            resize: vertical;
            transition: all 0.3s;
        }
        
        textarea:focus {
            outline: none;
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }
        
        button {
            width: 100%;
            padding: 14px;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            border-radius: 12px;
            font-size: 16px;
            font-weight: bold;
            cursor: pointer;
            transition: all 0.3s;
        }
        
        button:hover:not(:disabled) {
            transform: translateY(-2px);
            box-shadow: 0 5px 20px rgba(102, 126, 234, 0.4);
        }
        
        button:active:not(:disabled) {
            transform: translateY(0);
        }
        
        button:disabled {
            opacity: 0.6;
            cursor: not-allowed;
        }
        
        .status {
            margin-top: 20px;
            padding: 12px;
            border-radius: 12px;
            text-align: center;
            display: none;
            animation: slideIn 0.3s ease;
        }
        
        @keyframes slideIn {
            from {
                opacity: 0;
                transform: translateY(-10px);
            }
            to {
                opacity: 1;
                transform: translateY(0);
            }
        }
        
        .status.success {
            background: #d4edda;
            color: #155724;
            border: 1px solid #c3e6cb;
            display: block;
        }
        
        .status.error {
            background: #f8d7da;
            color: #721c24;
            border: 1px solid #f5c6cb;
            display: block;
        }
        
        .status.info {
            background: #d1ecf1;
            color: #0c5460;
            border: 1px solid #bee5eb;
            display: block;
        }
        
        .info-panel {
            margin-top: 20px;
            padding: 15px;
            background: #f8f9fa;
            border-radius: 12px;
            font-size: 12px;
        }
        
        .info-item {
            margin: 8px 0;
            display: flex;
            justify-content: space-between;
        }
        
        .info-label {
            font-weight: bold;
            color: #555;
        }
        
        .info-value {
            color: #667eea;
            font-family: monospace;
        }
        
        .footer {
            text-align: center;
            margin-top: 20px;
            padding-top: 15px;
            border-top: 1px solid #eee;
            font-size: 11px;
            color: #999;
        }
        
        @media (max-width: 480px) {
            .container {
                padding: 20px;
            }
            h1 {
                font-size: 20px;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>📺 SIST CSE DISPLAY</h1>
        <div class="subtitle">Digital Notice Board Controller</div>
        
        <div class="current-message">
            <label>📝 CURRENT MESSAGE</label>
            <div id="currentMsg">)rawliteral" + currentMessage + R"rawliteral(</div>
        </div>
        
        <div class="message-input">
            <label>✏️ ENTER NEW MESSAGE</label>
            <textarea id="message" rows="4" placeholder="Type your message here...&#10;&#10;Examples:&#10;• Exam schedule released&#10;• Workshop tomorrow at 10 AM&#10;• Holiday on Friday"></textarea>
        </div>
        
        <button id="updateBtn" onclick="updateMessage()">🚀 UPDATE DISPLAY</button>
        
        <div id="status" class="status"></div>
        
        <div class="info-panel">
            <div class="info-item">
                <span class="info-label">📡 WiFi SSID:</span>
                <span class="info-value">SISTK_CSE_DISPLAY</span>
            </div>
            <div class="info-item">
                <span class="info-label">🔗 Web Access:</span>
                <span class="info-value">http://csedept.local</span>
            </div>
            <div class="info-item">
                <span class="info-label">🌐 IP Address:</span>
                <span class="info-value">192.168.4.1</span>
            </div>
            <div class="info-item">
                <span class="info-label">👤 Developer:</span>
                <span class="info-value">N Poorna Prakash</span>
            </div>
        </div>
        
        <div class="footer">
            Department of Computer Science and Engineering<br>
            (with Specialization in IoT & Automation)
        </div>
    </div>
    
    <script>
        const updateBtn = document.getElementById('updateBtn');
        const messageInput = document.getElementById('message');
        const statusDiv = document.getElementById('status');
        const currentMsgDiv = document.getElementById('currentMsg');
        
        function showStatus(message, type) {
            statusDiv.textContent = message;
            statusDiv.className = `status ${type}`;
            setTimeout(() => {
                statusDiv.className = 'status';
            }, 3000);
        }
        
        async function updateMessage() {
            const message = messageInput.value.trim();
            
            if (!message) {
                showStatus('⚠️ Please enter a message!', 'error');
                messageInput.focus();
                return;
            }
            
            if (message.length > 250) {
                showStatus('⚠️ Message too long! Maximum 250 characters.', 'error');
                return;
            }
            
            // Disable button and show loading
            const originalText = updateBtn.textContent;
            updateBtn.textContent = '⏳ UPDATING...';
            updateBtn.disabled = true;
            
            try {
                const response = await fetch('/update', {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/x-www-form-urlencoded',
                    },
                    body: 'message=' + encodeURIComponent(message)
                });
                
                const result = await response.text();
                
                if (result === 'OK') {
                    showStatus('✅ Message updated successfully!', 'success');
                    currentMsgDiv.textContent = message;
                    messageInput.value = '';
                    
                    // Clear the textarea
                    messageInput.value = '';
                    
                    // Optional: Add a success animation
                    currentMsgDiv.style.animation = 'none';
                    setTimeout(() => {
                        currentMsgDiv.style.animation = 'fadeIn 0.5s ease';
                    }, 10);
                    
                } else {
                    showStatus('❌ Update failed. Please try again.', 'error');
                }
            } catch (error) {
                console.error('Error:', error);
                showStatus('❌ Connection error! Make sure you are connected to the display WiFi.', 'error');
            } finally {
                updateBtn.textContent = originalText;
                updateBtn.disabled = false;
            }
        }
        
        // Allow Enter key to submit (Ctrl+Enter or Cmd+Enter)
        messageInput.addEventListener('keydown', (e) => {
            if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
                e.preventDefault();
                updateMessage();
            }
        });
        
        // Auto focus on textarea
        messageInput.focus();
    </script>
</body>
</html>
)rawliteral";
  
  return html;
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
  EEPROM.begin(512);
  delay(10);
  
  int length = EEPROM.read(0);
  
  if (length > 0 && length <= 255) {
    for (int i = 0; i < length; i++) {
      char c = EEPROM.read(i + 1);
      if (c >= 32 && c <= 126) {
        message += c;
      }
    }
  }
  
  EEPROM.end();
  
  Serial.print("[EEPROM] Read message: ");
  Serial.println(message);
  
  return message;
}

/**
 * Save message to EEPROM
 * @param message - The message to save
 */
void saveMessageToEEPROM(String message) {
  EEPROM.begin(512);
  delay(10);
  
  int length = min(message.length(), 255);
  
  EEPROM.write(0, length);
  for (int i = 0; i < length; i++) {
    EEPROM.write(i + 1, message[i]);
  }
  
  EEPROM.commit();
  EEPROM.end();
  
  Serial.print("[EEPROM] Saved message: ");
  Serial.println(message);
}

// ============================================================================
// WEB SERVER HANDLERS
// ============================================================================

/**
 * Handle root web page request
 */
void handleRoot() {
  Serial.println("[Web] Root page requested");
  String currentMessage = readMessageFromEEPROM();
  if (currentMessage.length() == 0) {
    currentMessage = DEFAULT_MESSAGE;
  }
  server.send(200, "text/html", getHTMLPage(currentMessage));
}

/**
 * Handle message update request
 */
void handleUpdate() {
  Serial.println("[Web] Update request received");
  
  if (server.hasArg("message")) {
    String newMessage = server.arg("message");
    newMessage.trim();
    
    if (newMessage.length() > 0) {
      // Save to EEPROM
      saveMessageToEEPROM(newMessage);
      
      // Send to Arduino via Serial2
      Serial2.println(newMessage);
      
      Serial.print("[Serial] Sent to Arduino: ");
      Serial.println(newMessage);
      
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Empty message");
    }
  } else {
    server.send(400, "text/plain", "No message parameter");
  }
}

/**
 * Handle 404 not found
 */
void handleNotFound() {
  Serial.println("[Web] 404: " + server.uri());
  server.send(404, "text/plain", "404: Page Not Found\n\nAccess: http://192.168.4.1");
}

// ============================================================================
// SETUP FUNCTION
// ============================================================================

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n==========================================");
  Serial.println("P10 LED Display Controller - ESP32");
  Serial.println("Department of CSE (IoT & Automation)");
  Serial.println("==========================================\n");
  
  // Initialize Serial2 for Arduino communication
  Serial2.begin(9600, SERIAL_8N1, SERIAL2_RX, SERIAL2_TX);
  Serial.println("[Serial] Arduino communication initialized on GPIO17(TX)");
  
  // Initialize EEPROM
  EEPROM.begin(512);
  
  // Check and set default message if needed
  String savedMessage = readMessageFromEEPROM();
  if (savedMessage.length() == 0) {
    saveMessageToEEPROM(DEFAULT_MESSAGE);
    Serial.println("[EEPROM] Default message saved");
  }
  
  // Configure and start WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  WiFi.softAP(ap_ssid, ap_password);
  
  Serial.println("\n[WiFi] Access Point Started");
  Serial.print("  SSID: ");
  Serial.println(ap_ssid);
  Serial.print("  Password: ");
  Serial.println(ap_password);
  Serial.print("  IP Address: ");
  Serial.println(WiFi.softAPIP());
  
  // Setup mDNS
  if (MDNS.begin(mdns_hostname)) {
    Serial.print("[mDNS] Responder started: http://");
    Serial.print(mdns_hostname);
    Serial.println(".local/");
  } else {
    Serial.println("[mDNS] Error starting responder!");
  }
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/update", HTTP_POST, handleUpdate);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("[Web] Server started on port 80");
  
  // Send initial message to Arduino
  delay(2000);
  Serial2.println(savedMessage);
  Serial.print("[Serial] Initial message sent: ");
  Serial.println(savedMessage);
  
  Serial.println("\n==========================================");
  Serial.println("ESP32 Ready! Connect to WiFi and visit:");
  Serial.println("  http://192.168.4.1");
  Serial.println("  http://csedept.local");
  Serial.println("==========================================\n");
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
  // Handle web client requests
  server.handleClient();
  
  // Optional: Print connected clients count every 30 seconds
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 30000) {
    int clients = WiFi.softAPgetStationNum();
    if (clients > 0) {
      Serial.printf("[WiFi] Connected clients: %d\n", clients);
    }
    lastDebug = millis();
  }
}

// ============================================================================
// END OF CODE
// ============================================================================