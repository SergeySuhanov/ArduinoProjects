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
double angle = 0;
double radian = 0;
int smallDot = 3;
int mediumDot = 5;
int bigDot = 8;

class MyTriangle {
  public:
    double x0;
    double y0;
    double x1;
    double y1;
    double x2;
    double y2;
    double rightAngleRadian = 1.5707963;

    MyTriangle(double originX, double originY) {
      x0 = originX;
      y0 = originY;
      x1 = x0 + 8 * sin(0);
      y1 = y0 + 8 * cos(0);
      x2 = x0 + 6 * sin(rightAngleRadian);
      y2 = y0 + 6 * cos(rightAngleRadian);
    }

    void rotate(double angle) {
      x1 = x0 + 24 * sin(angle);
      y1 = y0 + 24 * cos(angle);
      double newRadian = angle + rightAngleRadian;
      x2 = x0 + 18 * sin(newRadian);
      y2 = y0 + 18 * cos(newRadian);
    }

    void moveOriginPoint(double speed, double angle) {
      double moveX = speed * cos(angle);
      double moveY = speed * sin(angle);

      x0 += moveX;
      y0 += moveY;

      x1 += moveX;
      y1 += moveY;

      x2 += moveX;
      y2 += moveY;
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
  myTimer = millis();
}


void loop() {
  if (millis() >= (myTimer + frameDuration)) {
    display.clearDisplay();
    myTimer += frameDuration;
    if (angle >=360) angle == 0;
    radian = angle * 3,14 / 180;
    shape.rotate(radian);
    //shape.moveOriginPoint(1, radian);
    angle += 0.03;

    display.drawTriangle(shape.x0, shape.y0, shape.x1, shape.y1, shape.x2, shape.y2, SSD1306_WHITE);
    display.drawCircle(shape.x0, shape.y0, smallDot, SSD1306_WHITE);
    display.drawCircle(shape.x1, shape.y1, mediumDot, SSD1306_WHITE);
    display.drawCircle(shape.x2, shape.y2, bigDot, SSD1306_WHITE);
    display.display();
  }
}