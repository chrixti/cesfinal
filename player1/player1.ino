#include <TFT_eSPI.h>
#include <esp_now.h>
#include <WiFi.h>

// Include bitmap header files
#include "rock_bitmap.h"
#include "paper_bitmap.h"
#include "scissors_bitmap.h"

// TFT Display
TFT_eSPI tft = TFT_eSPI();
#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF
#define TFT_RED 0xF800

// Buttons
#define LEFT_BUTTON 0   // GPIO 0 for Left Button
#define RIGHT_BUTTON 35 // GPIO 35 for Right Button

#define SPEAKER_PIN 27  // GPIO pin connected to the speaker



// Player data
int choiceIndex = 0; // 0 = Rock, 1 = Paper, 2 = Scissors
bool moveConfirmed = false;

// ESP-NOW communication
uint8_t peerAddress[] = {0xA0, 0xDD, 0x6C, 0x70, 0x84, 0x28}; // MAC address of Device B
bool peerAddressSet = true;

struct Data {
    int choice;
    bool ready;
} data, opponentData;

bool bothReady = false;

// Array of embedded images and labels
static const unsigned char *images[] = {rockBitmap, paperBitmap, scissorsBitmap};
static const char *labels[] = {"Rock", "Paper", "Scissors"};

// Function to play a tone
void playTone(int frequency, int duration) {
    ledcSetup(0, frequency, 8);  // Channel 0, frequency in Hz, 8-bit resolution
    ledcAttachPin(SPEAKER_PIN, 0);
    ledcWriteTone(0, frequency);
    delay(duration);
    ledcWriteTone(0, 0);  // Stop the tone
}

// Function to display the bitmap and label on the screen
void displaySelection(int index, bool confirmed) {
    tft.fillScreen(TFT_BLACK);

    // Display the bitmap on the left
    tft.drawBitmap(10, (tft.height() - 140) / 2, images[index], 104, 140, TFT_WHITE);

    // Display the label closer to the bitmap
    tft.setCursor(130, tft.height() / 2 - 20);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println(labels[index]);

    // Display "Confirmed" below the label
    if (confirmed) {
        tft.setCursor(130, tft.height() / 2 + 20);
        tft.println("Confirmed");
    }
}

// Display result screen
void displayResult(int myChoice, int opponentChoice) {
    tft.fillScreen(TFT_BLACK);

    // Player's move on the left
    tft.setCursor(10, 20);
    tft.setTextSize(2);
    tft.setTextColor(TFT_WHITE);
    tft.println("You:");
    tft.drawBitmap(10, (tft.height() - 140) / 2, images[myChoice], 104, 140, TFT_WHITE);

    // Opponent's move on the right
    tft.setCursor(130, 20);
    tft.println("Opponent:");
    tft.drawBitmap(130, (tft.height() - 140) / 2, images[opponentChoice], 104, 140, TFT_RED);

    delay(1000);

    // Determine winner and play corresponding sound
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 100);
    if (myChoice == opponentChoice) {
        tft.println("It's a tie!");
        playTone(800, 500);  // Tie sound
    } else if ((myChoice == 0 && opponentChoice == 2) ||
               (myChoice == 1 && opponentChoice == 0) ||
               (myChoice == 2 && opponentChoice == 1)) {
        tft.println("You Win!");
        playTone(1200, 500);  // Win sound
    } else {
        tft.println("You Lose!");
        playTone(400, 500);  // Loss sound
    }
    delay(3000);
}

// Button handling
void handleButtons() {
    if (digitalRead(LEFT_BUTTON) == LOW) {
        choiceIndex = (choiceIndex + 1) % 3;
        Serial.printf("Updated choice: %s\n", labels[choiceIndex]);
        playTone(1000, 200);  // Play tone for selection
        displaySelection(choiceIndex, false); // Update selection without confirmation
        delay(300);
    }

    if (digitalRead(RIGHT_BUTTON) == LOW) {
        moveConfirmed = true;
        data.choice = choiceIndex;
        data.ready = true;

        playTone(1500, 300);  // Play tone for move confirmation

        esp_err_t result = esp_now_send(peerAddress, (uint8_t *)&data, sizeof(data));
        if (result == ESP_OK) {
            Serial.println("Data sent successfully");
        } else {
            Serial.printf("Data send failed with error code: %d\n", result);
        }

        displaySelection(choiceIndex, true);
        delay(3000);
    }
}

// ESP-NOW callbacks
void onReceive(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
    Serial.println("Data received!");
    memcpy(&opponentData, incomingData, sizeof(opponentData));

    // Print received data
    Serial.printf("Opponent choice: %d, Ready: %s\n", opponentData.choice, opponentData.ready ? "Yes" : "No");

    if (data.ready && opponentData.ready) {
        bothReady = true;
        Serial.println("Both players are ready!");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LEFT_BUTTON, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON, INPUT_PULLUP);

    // Initialize TFT Display
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);

    // Display initial move
    displaySelection(choiceIndex, false);

    // ESP-NOW setup
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed");
        return;
    }

    // Print this device's MAC address
    Serial.print("This device's MAC address: ");
    Serial.println(WiFi.macAddress());

    // Register peer
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, peerAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) == ESP_OK) {
        Serial.println("Peer added successfully");
    } else {
        Serial.println("Failed to add peer");
    }

    esp_now_register_recv_cb(onReceive);
}

void loop() {
    handleButtons();

    // Debugging: Check if bothReady is false repeatedly
    if (!bothReady) {
        Serial.println("Waiting for both players to be ready...");
    }

    if (bothReady) {
        Serial.println("Displaying result...");
        displayResult(data.choice, opponentData.choice);
        bothReady = false;
        moveConfirmed = false;
        opponentData.ready = false;
        data.ready = false;
        displaySelection(choiceIndex, false);
    }
}