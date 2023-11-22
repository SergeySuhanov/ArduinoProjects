#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long myTimer;
byte frameDuration = 20;
int frameTime = 0;
double angle = 0;
int smallDot = 3;
int mediumDot = 5;
int bigDot = 8;

class MyTriangle {
  public:
    double x0;  // centre origin point
    double y0;
    double x1;  // vertices
    double y1;
    double x2;
    double y2;
    double x3;
    double y3;
    double radianOf120 = 120 * 3.14 / 180;  // vertices spread

    MyTriangle(double originX, double originY) {
      x0 = originX;
      y0 = originY;
      x1 = x0 + 20 * sin(0);
      y1 = y0 + 20 * cos(0);
      x2 = x0 + 20 * sin(radianOf120);
      y2 = y0 + 20 * cos(radianOf120);
      x3 = x0 + 20 * sin(-radianOf120);
      y3 = y0 + 20 * cos(-radianOf120);
    }

    void rotate(double angle) {
      x1 = x0 + 20 * sin(0 + angle);
      y1 = y0 + 20 * cos(0 + angle);
      x2 = x0 + 20 * sin(radianOf120 + angle);
      y2 = y0 + 20 * cos(radianOf120 + angle);
      x3 = x0 + 20 * sin(-radianOf120 + angle);
      y3 = y0 + 20 * cos(-radianOf120 + angle);
    }

    void moveOriginPoint(double speed, double angle) {
      double moveX = speed * cos(angle);
      double moveY = speed * sin(angle);

      x0 += moveX;
      y0 += moveY;
    }
};

MyTriangle shape(64, 32);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // the library initializes this with an Adafruit splash screen.
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  myTimer = millis();
}


void loop() {
  if (millis() >= (myTimer + frameDuration)) {
    display.clearDisplay();
    
    if (angle >=2) angle == 0;
    shape.rotate(angle);
    //shape.moveOriginPoint(1, radian);
    angle += 0.03;

    display.drawTriangle(shape.x1, shape.y1, shape.x2, shape.y2, shape.x3, shape.y3, SSD1306_WHITE);
    display.drawCircle(shape.x1, shape.y1, smallDot, SSD1306_WHITE);
    display.drawCircle(shape.x2, shape.y2, mediumDot, SSD1306_WHITE);
    display.drawCircle(shape.x3, shape.y3, bigDot, SSD1306_WHITE);

    frameTime = millis() - myTimer;
    display.setCursor(0, 0);
    display.println(frameTime);
    myTimer += frameDuration;

    display.display();
  }
}