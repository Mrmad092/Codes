// =======@Madtektonik=======//

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BTN_LEFT 13
#define BTN_RIGHT 12
#define BTN_FIRE 14
#define BUZZER_PIN 4

const unsigned char spaceship_bmp[] PROGMEM = {
  0b00011000,
  0b00111100,
  0b01111110,
  0b11111111,
  0b01111110,
  0b00100100,
  0b01000010
};


struct Bullet {
  int x, y;
  bool active;
};

struct Enemy {
  int x, y;
  int size;
  bool alive;
};

const int maxBullets = 5;
Bullet bullets[maxBullets];
Enemy enemies[5];

int playerX = SCREEN_WIDTH / 2;
int playerY = SCREEN_HEIGHT - 16;
int score = 0;
int health = 10;
bool gameOver = false;
bool gameStarted = false;
unsigned long lastFireTime = 0;
int enemySpeed = 1;

void setup() {
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_FIRE, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  showStartScreen();
}

void resetGame() {
  gameOver = false;
  gameStarted = true;
  score = 0;
  health = 10;
  enemySpeed = 1;
  playerX = SCREEN_WIDTH / 2;
  for (int i = 0; i < maxBullets; i++) bullets[i].active = false;
  for (int i = 0; i < 5; i++) {
    enemies[i] = {random(0, SCREEN_WIDTH - 10), random(-60, -10), random(6, 12), true};
  }
}

void showStartScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.print("SPACE SHOOTER");
  display.setCursor(5, 35);
  display.print("Press any button");
  display.setCursor(30, 45);
  display.print("to start");
  display.display();

  while (digitalRead(BTN_LEFT) == HIGH && digitalRead(BTN_RIGHT) == HIGH && digitalRead(BTN_FIRE) == HIGH) {
    delay(10);
  }
  resetGame();
}

void drawPlayer() {
  display.drawBitmap(playerX - 4, playerY, spaceship_bmp, 8, 7, SSD1306_WHITE); 
  
}


void drawBullets() {
  for (int i = 0; i < maxBullets; i++) {
    if (bullets[i].active) {
      display.drawFastVLine(bullets[i].x, bullets[i].y, 4, SSD1306_WHITE);
    }
  }
}

void drawEnemies() {
  for (int i = 0; i < 5; i++) {
    if (enemies[i].alive) {
      int x = enemies[i].x;
      int y = enemies[i].y;
      int s = enemies[i].size;
      display.drawCircle(x + s / 2, y + s / 2, s / 2, SSD1306_WHITE);
      display.drawLine(x, y + s / 2, x + s, y + s / 2, SSD1306_WHITE);
      display.drawLine(x + s / 2, y, x + s / 2, y + s, SSD1306_WHITE);
    }
  }
}

void updateBullets() {
  for (int i = 0; i < maxBullets; i++) {
    if (bullets[i].active) {
      bullets[i].y -= 5;
      if (bullets[i].y < 0) bullets[i].active = false;
    }
  }
}

void updateEnemies() {
  for (int i = 0; i < 5; i++) {
    if (enemies[i].alive) {
      enemies[i].y += enemySpeed;
      if (enemies[i].y > SCREEN_HEIGHT) {
        health--;
        enemies[i].x = random(0, SCREEN_WIDTH - 10);
        enemies[i].y = random(-60, -10);
        enemies[i].size = random(6, 12);
      }
      // Direct hit detection
      if (enemies[i].y + enemies[i].size >= playerY &&
          enemies[i].x + enemies[i].size / 2 >= playerX - 4 &&
          enemies[i].x + enemies[i].size / 2 <= playerX + 4) {
        gameOver = true;
      }
    }
  }
}

void fireBullet() {
  if (millis() - lastFireTime < 200) return; // Prevent spam
  for (int i = 0; i < maxBullets; i++) {
    if (!bullets[i].active) {
      bullets[i].x = playerX;
      bullets[i].y = playerY;
      bullets[i].active = true;
      tone(BUZZER_PIN, 1200, 30);
      lastFireTime = millis();
      break;
    }
  }
}

void checkCollisions() {
  for (int i = 0; i < maxBullets; i++) {
    if (bullets[i].active) {
      for (int j = 0; j < 5; j++) {
        int s = enemies[j].size;
        if (enemies[j].alive && bullets[i].x > enemies[j].x && bullets[i].x < enemies[j].x + s && bullets[i].y < enemies[j].y + s) {
          bullets[i].active = false;
          score += 5;
          enemies[j].x = random(0, SCREEN_WIDTH - 10);
          enemies[j].y = random(-60, -10);
          enemies[j].size = random(6, 12);
          tone(BUZZER_PIN, 800, 20);
          if (score % 25 == 0 && enemySpeed < 5) enemySpeed++; // Increase difficulty
        }
      }
    }
  }
}

void loop() {
  if (!gameStarted) {
    showStartScreen();
    return;
  }

  if (gameOver || health <= 0) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(30, 20);
    display.print("GAME OVER");
    display.setCursor(20, 35);
    display.print("Score: ");
    display.print(score);
    display.setCursor(10, 50);
    display.print("Press any button");
    display.display();
    tone(BUZZER_PIN, 200, 300);
    delay(500);
    while (digitalRead(BTN_LEFT) == HIGH && digitalRead(BTN_RIGHT) == HIGH && digitalRead(BTN_FIRE) == HIGH) {
      delay(10);
    }
    showStartScreen();
    return;
  }

  if (digitalRead(BTN_LEFT) == LOW && playerX > 5) playerX -= 2;
  if (digitalRead(BTN_RIGHT) == LOW && playerX < SCREEN_WIDTH - 5) playerX += 2;
  if (digitalRead(BTN_FIRE) == LOW) fireBullet();

  updateBullets();
  updateEnemies();
  checkCollisions();

  display.clearDisplay();
  drawPlayer();
  drawBullets();
  drawEnemies();

  display.setCursor(0, 0);
  display.print("Score:"); display.print(score);
  display.setCursor(80, 0);
  display.print("HP:"); display.print(health);

  display.display();
  delay(30);
}
