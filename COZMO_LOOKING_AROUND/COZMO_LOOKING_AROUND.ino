#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int eyeX1 = 26, eyeX2 = 70, eyeY = 20;  // Position of both eyes
int eyeSize = 36;  // Size of eyes
int sizeVariation = 4;  // Reduced size variation for subtle effect
int positionVariation = 6; // Movement offset for more realism

void drawEyees(int height) {
    display.clearDisplay();

    // Draw left eye
    display.fillRoundRect(eyeX1, eyeY + (eyeSize - height) / 2, eyeSize, height, 4, SSD1306_WHITE);
    // Draw right eye
    display.fillRoundRect(eyeX2, eyeY + (eyeSize - height) / 2, eyeSize, height, 4, SSD1306_WHITE);

    display.display();
}

void drawEyes(int leftSize, int rightSize, int xOffset) {
    display.clearDisplay();

    // Draw left eye
    display.fillRoundRect(eyeX1 - xOffset, eyeY + (eyeSize - leftSize) / 2, leftSize, leftSize, 4, SSD1306_WHITE);
    // Draw right eye
    display.fillRoundRect(eyeX2 - xOffset, eyeY + (eyeSize - rightSize) / 2, rightSize, rightSize, 4, SSD1306_WHITE);

    display.display();
}
void blinkAnimation() {
    // Slowly close the eyes
    for (int h = eyeSize; h >= 1; h -= 7) {
        drawEyees(h);
        
    }
    delay(100);  // Eyes closed briefly

    // Slowly open the eyes
    for (int h = 1; h <= eyeSize; h += 7) {
        drawEyees(h);
        
    }
}

void lookAroundAnimation() {
    // Look left
    for (int i = 0; i <= 10; i++) {
        int leftEyeSize = eyeSize + (i * sizeVariation) / 10;
        int rightEyeSize = eyeSize - (i * sizeVariation) / 10;
        int offset = (i * positionVariation) / 10; // Adjust offset for consistent gap and position shift
        drawEyes(leftEyeSize, rightEyeSize, offset);
        delay(1);
    }
    delay(2000);
    
    // Look right
    for (int i = 10; i >= -10; i--) {
        int leftEyeSize = eyeSize + (i * sizeVariation) / 10;
        int rightEyeSize = eyeSize - (i * sizeVariation) / 10;
        int offset = (i * positionVariation) / 10; // Adjust offset for consistent gap and position shift
        drawEyes(leftEyeSize, rightEyeSize, offset);
        delay(1);
    }
    delay(1000);
    
    // Return to center
    for (int i = -10; i <= 0; i++) {
        int leftEyeSize = eyeSize + (i * sizeVariation) / 10;
        int rightEyeSize = eyeSize - (i * sizeVariation) / 10;
        int offset = (i * positionVariation) / 10; // Adjust offset for consistent gap and position shift
        drawEyes(leftEyeSize, rightEyeSize, offset);
        delay(1);
    }
    delay(500);
}

void setup() {
    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    display.clearDisplay();
    drawEyes(eyeSize, eyeSize, 0);  // Show normal eyes
}

void loop() {
    blinkAnimation();
    delay(1500); 
    blinkAnimation();
    delay(100); 
    lookAroundAnimation();
    delay(800);
    // Wait 2-5 seconds before blinking
  
}

