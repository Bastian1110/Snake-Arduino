#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define BUTTONONE D3 //Up
#define BUTTONTWO D7 //Left
#define BUTTONTRE D5 //Right
#define BUTTONFOR D6 //Down

int up, down, left, right;
int headX = 64; int headY = 32;
int lastX, lastY;
int foodX, foodY; 
bool fUp = false;
bool fDown = false;
bool fLeft = false;
bool fRight = false;

bool eaten = true;
bool dead = false;


int points  = -1;
int bufX[128];
int bufY[128];

bool game = true;
bool deadDisplay = false;


bool checkIntersect(int x, int y){
    for(int i = 1; i < points; i++){
        if(bufX[i] == x && bufY[i] == y){
            return true;
        }
    }
    return false;
}

bool checkWall(int x, int y){
    if((x < 0 || x > 124) || (y < 0 || y > 61)){
        return true;
    }
    else{
        return false;
    }
}

void setup(){
    Serial.begin(115200);

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("Screen allocation failed"));
        for(;;); 
    }

    pinMode(BUTTONONE,INPUT_PULLUP);
    pinMode(BUTTONTWO,INPUT_PULLUP);  //Initialize Buttons
    pinMode(BUTTONTRE,INPUT_PULLUP);
    pinMode(BUTTONFOR,INPUT_PULLUP);

    // Show initial display buffer contents on the screen 
    display.display();
    delay(2000); 

    display.clearDisplay();

    display.setCursor(4,4);
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
}

void loop(){
    if(game){
        bool flag = false;
        up = digitalRead(BUTTONONE);
        down = digitalRead(BUTTONFOR);
        left = digitalRead(BUTTONTWO);
        right = digitalRead(BUTTONTRE);

        if(up == LOW){
            fUp = true;
            fDown = false;
            fLeft = false;
            fRight = false;
        }
        if(down == LOW){
            fUp = false;
            fDown = true;
            fLeft = false;
            fRight = false;
        }
        if(left == LOW){
            fUp = false;
            fDown = false;
            fLeft = true;
            fRight = false;
        }
        if(right == LOW){
            fUp = false;
            fDown = false;
            fLeft = false;
            fRight = true;
        }
        if(fUp){
            headY-=4;
            flag = true;
        }
        if(fDown){
            headY+=4;
            flag = true;
        }
        if(fLeft){
            headX-=4;
            flag = true;
        }
        if(fRight){
            headX+=4;
            flag = true;
        }
        int distanceX = headX - foodX;
        int distanceY = headY - foodY;
        if((distanceX<=4 && distanceX >= -4)&&(distanceY<=4 && distanceY >= -4)){
            eaten = true;
        }
        if(flag){
            display.clearDisplay();
            if(eaten){
                bool notIn = true;
                while(notIn == true){
                    foodX = random(3,124);
                    foodY = random(3,60);
                    notIn = checkIntersect(foodX,foodY);
                }
                points++;
                eaten = false;
            }
            display.setCursor(4,4);
            for(int i = 0; i < points; i++){
                display.fillRect(bufX[i],bufY[i],4,4,WHITE);
            }
            int tempX, tempY;
            for(int i = 0; i < points; i++){
                bufX[points-1-i] = bufX[points-2-i];
                bufY[points-1-i] = bufY[points-2-i];
            }
            bufX[0] = headX;
            bufY[0] = headY;
            bool killedBySnake = checkIntersect(headX,headY);
            bool killedByWall  = checkWall(headX, headY);
            if(killedBySnake || killedByWall){
                game = false;
                deadDisplay = true;
            } 
            display.println(points);
            display.fillCircle(foodX, foodY, 2, WHITE);
            display.fillRect(headX, headY, 4, 4, WHITE);
            display.display();
            delay(100);
        }
    }
    else{
        if(deadDisplay){
            display.setCursor(25,25);
            display.setTextColor(SSD1306_WHITE);
            display.setTextSize(2);
            display.println("YOU LOST");
            display.display();
        }
        up = digitalRead(BUTTONONE);
        down = digitalRead(BUTTONFOR);
        if(up == LOW && down == LOW){
            game = true;
            deadDisplay = false;
            points = 0;
            display.clearDisplay();
            display.setCursor(4,4);
            display.setTextColor(SSD1306_WHITE);
            display.setTextSize(1);
        }

    }
}