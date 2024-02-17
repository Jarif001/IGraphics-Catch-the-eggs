# include "iGraphics.h"
#include <windows.h>
#include "mmsystem.h"
#include<stdlib.h>
#include <string.h>

//Screen width and height
int sWidth = 1000, sHeight = 700;

//menu
//Menu or Play state
int scene = -1;//menu=-1,exit=0,start=1,resume=2,highscore=3,Ins-4,TimeOver-5
//Music state
bool menuMusic = true;
bool gamePlayMusic = true;
bool timeOverMusic = true;

//point
struct Point{
    int x;
    int y;
}menuButtonPos[3];

//menuButton BMP names
char menuButtonPic[3][50] = {"img\\exitButton.bmp", "img\\startButton.bmp", "img\\resumeButton.bmp"};
//Chicken BMP names
char chickenPic[2][50] = {"img\\chickenSitting.bmp", "img\\chickenLayingEgg.bmp"};

//Chicken number
int numChick = 1;
//chicken position
int chickenposX[2];
int chickenposY = sHeight - 195;
int layPosX[2];
//Basket Thing
int basketPos = (sWidth/2)-70;
int dxBasket = 100;

//laidThings manager
//Laid things
char picLaidThings[4][50] = {"img\\egg.bmp", "img\\blueEgg.bmp", "img\\poop.bmp", "img\\goldenEgg.bmp"};

//Laying manager
int nrmlEggPos[2] = {sHeight - 200,sHeight - 200};
int blueEggPos[2] = {sHeight - 200,sHeight - 200};
int poopPos[2] = {sHeight - 200,sHeight - 200};
int goldenEggPos[2] = {sHeight - 200,sHeight - 200};

int lay[2];
int stop[2] = {0,0};
int anim[2] = {0,0};
int jump = 40;

//Pause state
int pause = 1;
int startOverText = 0;

//Time state
int timeOver = 0;

void laidThingsOne(){
    if(scene == 1 && !pause && !timeOver){
        if(!stop[0]){
            anim[0] = 1;
            int ranNum = rand() % 8;
            if(ranNum < 3){
                lay[0] = 0;//normal egg
            }
            else if(ranNum < 5){
                lay[0] = 1;//blue egg
            }
            else if(ranNum < 7){
                lay[0] = 2;//poop
            }
            else{
                lay[0] = 3;//golden egg
            }
            int posX = (rand()%1000) + 5;
            if(posX < 900)
                chickenposX[0] = posX;
            else{
                chickenposX[0] = 905;
            }
            layPosX[0] = chickenposX[0];
        }
        stop[0] = 1;
    }
}

void laidThingsTwo(){
    if(scene == 1 && !pause && !timeOver){
        if(!stop[1]){
            anim[1] = 1;
            int ranNum = rand() % 8;
            if(ranNum < 3){
                lay[1] = 0;//normal egg
            }
            else if(ranNum < 5){
                lay[1] = 1;//blue egg
            }
            else if(ranNum < 7){
                lay[1] = 2;//poop
            }
            else{
                lay[1] = 3;//golden egg
            }
            int posX = (rand()%1000) + 5;
            if(posX < 900)
                chickenposX[1] = posX;
            else{
                chickenposX[1] = 905;
            }
            layPosX[1] = chickenposX[1];
        }
        stop[1] = 1;
    }
}

//Time function
int sec = 0;
int mins = 0;
int timeOnCounter = 0;
char strTime[20];
int totalSecs = 0;
int selectTime = 0;
int duration;

//Rain time
int rainTimeOn = 0;
int rainTime = 0;

void timeFunction(){
    if(scene == 1 && timeOnCounter == 1 && !pause && !timeOver){
        sec++;
        if(sec > 59){
            mins++;
            sec = 0;
        }
        totalSecs++;
        if(rainTimeOn){
            rainTime++;
        }
    }
    sprintf(strTime, "%02d : %02d",mins,sec);
}

//gravity
int dy = 5;
int movePos[2] = {sHeight - 200,sHeight - 200};

void gravity(int n){
        if(lay[n] == 0)
            nrmlEggPos[n] = movePos[n];
        if(lay[n] == 1)
            blueEggPos[n] = movePos[n];
        if(lay[n] == 2)
            poopPos[n] = movePos[n];
        if(lay[n] == 3)
            goldenEggPos[n] = movePos[n];

        movePos[n] -= dy + (rand()%(n+1))*2;

        if(totalSecs > 95){
            dy = 25;
            jump = 200;
        }
        else if(totalSecs > 75){
            dy = 20;
            jump = 150;
        }
        else if(totalSecs > 50){
            dy = 15;
            jump = 100;
        }
        else if(totalSecs > 25){
            dy = 10;
            jump = 80;
        }
}

void gravityOne(){
    if(scene == 1 && !pause && !timeOver){
        gravity(0);
    }
}

void gravityTwo(){
    if(scene == 1 && !pause && !timeOver){
        if(numChick == 2)
            gravity(1);
    }
}

//Score calculation
int score = 0;
char scoreTxt[20];

//Music TimeOver
int once[2] = {1,1};
void timeMusic(){
    if(scene == 1 && totalSecs > duration - 8 && once[0]){
        gamePlayMusic = false;
        PlaySound(0,0,0);
        PlaySound("music\\deadTime.wav", NULL, SND_ASYNC);
        once[0] = 0;
    }
    if(timeOver && once[1]){
        PlaySound(0,0,0);
        PlaySound("music\\ohNo.wav", NULL, SND_ASYNC);
        once[1] = 0;
    }
}

int played = 0;
int showNotPlayed = 0;

//HighScore calculation
//60Secs
int highScore60[10];
char *names60[10];
//90Secs
int highScore90[10];
char *names90[10];
//120Secs
int highScore120[10];
char *names120[10];
//HighScore files
FILE *fp;
FILE *fp2;
FILE *fp3;
//HighScore menu 60,90,120 control
int show60 = 0;
int show90 = 0;
int show120 = 0;
char showboard[10][30];

//Store the name
char playerName[20];
int indexPlayer = 0;
int inputName = 1;

void sortScore(int *highscores, char *names[], int n){
    for(int i = 0; i < n-1; i++){
        for(int j = i+1; j < n; j++){
            if(highscores[i] < highscores[j]){
                int tempScore = highscores[i];
                char *tempName;
                tempName = (char*)malloc(sizeof(char)*(strlen(names[i])+1));
                strcpy(tempName, names[i]);
                highscores[i] = highscores[j];
                names[i] = (char*)malloc(sizeof(char)*((strlen(names[j]))+1));
                strcpy(names[i], names[j]);
                highscores[j] = tempScore;
                names[j] = (char*)malloc(sizeof(char)*((strlen(tempName))+1));
                strcpy(names[j], tempName);
            }
        }
    }
}

void nameProcessing(char *p){
    while(*p){
        if(*p == ' '){
            *p = '\0';
            break;
        }
        p++;
    }
}

//Instruction menu
char *insStr[7] = {"Catch the eggs, not the poops",
    "Move the basket by arrow keys, a-d key,or the mouse",
    "Another chicken will appear in the half-time",
    "*Adds-On* Rain- Golden Egg-Poop Rain(Randomly every appx 35 secs and lasts for 10 secs)",
    "50 points for no poop at the end of the game",
    "Score as much as you can within the duration",
    "               GOOD LUCK!"
};

//Bonuses
int noPoop = 1;
int noPoopText = 1;
int rain;//0 for poop, 1 for goldenEgg

int randX;
int rainFall = 1;
void rainBonus(){
    if(rainTimeOn && scene == 1 && !pause && !timeOver){
        int randomNumber = rand();
        if(rainFall){
            int randRain = randomNumber%3;
            if(randRain < 2){
                //Poop rain
                rain = 0;
            }
            else{
                //goldenEgg rain
                rain = 1;
            }
        }
        if(rainFall){
            randX = (randomNumber%900)+20;
            rainFall = 0;
        }
    }
}
int raindy = 28;
int rainPos = sHeight;
void rainGravity(){
    if(scene == 1 && !pause && !timeOver)
        rainPos -= raindy;
}

/*
    function iDraw() is called again and again by the system.
*/
void iDraw()
{
    //place your drawing codes here
    iClear();
    //Menu
    if(scene == -1){
        //menu bg
        iShowBMP(0, 0, "img\\menu.bmp");

        for(int i = 0; i < 3; i++){
            iShowBMP2(menuButtonPos[i].x, menuButtonPos[i].y, menuButtonPic[i],0);
        }
        //Highscore button
        iShowBMP2((sWidth/2) + 290, 13+(sHeight/14), "img\\highScoreButton.bmp",0);
        //Instruction button
        iShowBMP2((sWidth/3) - 270, 13+(sHeight/14), "img\\instruction.bmp",0);

        //Show not played text
        if(showNotPlayed){
            iSetColor(255,0,0);
            iText(330, 320, "Not Played Yet or Finished a Game", GLUT_BITMAP_TIMES_ROMAN_24);
        }

        //Music Control
        if(menuMusic){
            iShowBMP(sWidth - (2*60), sHeight - (1.6*60), "img\\musicOnButton.bmp");
        }
        else{
            iShowBMP(sWidth - (2*60), sHeight - (1.6*60), "img\\musicOffButton.bmp");
        }
        //Welcome text
        iSetColor(rand()%256, rand()%256, rand()%256);
        iText(448, 620, "WELCOME",GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(rand()%256, rand()%256, rand()%256);
        iText(492, 590, "TO",GLUT_BITMAP_TIMES_ROMAN_24);

        //Time
        timeOnCounter = 0;
        //time select
        if(selectTime){
            iSetColor(255, 255, 255);
            iFilledRectangle(100, 275, 800, 275);
            iSetColor(0,0,0);
            iText(380, 475, "SELECT TIME DURATION", GLUT_BITMAP_TIMES_ROMAN_24);
            for(int i = 0; i < 3; i++){
                iSetColor(195, 183, 180);
                iFilledRectangle(225*(i+1), 320, 100, 30);
                iSetColor(0,0,0);
                char str[10];
                sprintf(str, "%d",60+(30*i));
                iText(250*(i+1)-(i*15), 330, str, GLUT_BITMAP_TIMES_ROMAN_24);
            }
        }
    }

    else if(scene == 0){
        exit(1);
    }

    //GamePlay

    else if(scene == 1){
        //Start the game
        iShowBMP(0,0, "img\\gamebg.bmp");
        //Music Control Button
        if(gamePlayMusic){
            iShowBMP(sWidth - (2*60), sHeight - (1.6*60), "img\\musicOnButton.bmp");
        }
        else{
            iShowBMP(sWidth - (2*60), sHeight - (1.6*60), "img\\musicOffButton.bmp");
        }
        //Pause button
        iShowBMP2(50, sHeight - (1.5*60), "img\\pauseButton.bmp", 0);

        //wire
        iSetColor(0,0,0);
        iLine(0,sHeight-190,sWidth,sHeight-190);

        //Chicken

        for(int i = 0; i < numChick; i++){
            if(lay[i] == 0 && nrmlEggPos[i] < sHeight-210)
                iShowBMP2(layPosX[i] + 37, nrmlEggPos[i], picLaidThings[0], 0xffffff);
            if(lay[i] == 1 && blueEggPos[i] < sHeight-210)
                iShowBMP2(layPosX[i] + 37, blueEggPos[i], picLaidThings[1], 0xffffff);
            if(lay[i] == 2 && poopPos[i] < sHeight-210)
                iShowBMP2(layPosX[i] + 37, poopPos[i], picLaidThings[2], 0xffffff);
            if(lay[i] == 3 && goldenEggPos[i] < sHeight-210)
                iShowBMP2(layPosX[i] + 37, goldenEggPos[i], picLaidThings[3], 0xffffff);

            if(nrmlEggPos[i]<0 || blueEggPos[i]<0||poopPos[i]<0||goldenEggPos[i]<0){
                stop[i] = 0;
                movePos[i] = sHeight - 200;
            }
            if(nrmlEggPos[i]<sHeight-195-jump || blueEggPos[i]<sHeight-195-jump||poopPos[i]<sHeight-195-jump||goldenEggPos[i]<sHeight-195-jump){
                anim[i] = 0;
            }

        //Chicken Pic
        if(stop[i]){
            if(!anim[i])
                iShowBMP2(chickenposX[i], sHeight-195, chickenPic[0], 0);
            else
                iShowBMP2(chickenposX[i], sHeight-195, chickenPic[1],0);
        }

            //Score management
            if(layPosX[i] > basketPos-40 && layPosX[i] < basketPos+140-30){
                if(nrmlEggPos[i] < 80 && nrmlEggPos[i] > 0){
                    nrmlEggPos[i] = -100;
                    score += 1;
                }
                if(blueEggPos[i] < 80 && blueEggPos[i] > 0){
                    blueEggPos[i] = -100;
                    score += 5;
                }
                if(poopPos[i] < 80 && poopPos[i] > 0){
                    poopPos[i] = -100;
                    noPoop = 0;
                    score -= 10;
                    noPoopText = 0;
                }
                if(goldenEggPos[i] < 80 && goldenEggPos[i] > 0){
                    goldenEggPos[i] = -100;
                    score += 10;
                }
            }
        }

        //Rain
        if(rainTimeOn){
            if(rain == 0){
                iShowBMP2(randX, rainPos, picLaidThings[2], 0xffffff);
            }
            if(rain == 1){
                iShowBMP2(randX, rainPos, picLaidThings[3], 0xffffff);
            }
            //Score
            if(randX > basketPos-40 && randX < basketPos+140-30){
                if(rainPos < 80 && rainPos > 0){
                    rainPos = sHeight;
                    rainFall = 1;
                    if(rain == 0)
                        score -= 10;
                    else if(rain == 1)
                        score += 10;
                }
            }
            //Falling to ground
            if(rainPos < 0){
                rainPos = sHeight;
                rainFall = 1;
            }
        }

        //basket
        iShowBMP2(basketPos,15,"img\\basket.bmp", 0xffffff);

        //Time
        timeOnCounter = 1;
        iText((sWidth/2)-20, sHeight-30, strTime, GLUT_BITMAP_TIMES_ROMAN_24);

        if(totalSecs >= duration/2){
            numChick = 2;
        }

        if(totalSecs >= 110 && totalSecs <= 116){
            rainTimeOn = 1;
        }
        else if(totalSecs >= 75 && totalSecs <= 81){
            rainTimeOn = 1;
        }
        else if(totalSecs >= 35 && totalSecs <= 41){
            rainTimeOn = 1;
        }

        if(rainTime >= 9){
            rainTime = 0;
            rainTimeOn = 0;
        }

        if(totalSecs >= duration){
            timeOver = 1;//For time over
        }

        //Score
        sprintf(scoreTxt, "Score : %d",score);
        iText((sWidth/2)-30, sHeight-50, scoreTxt, GLUT_BITMAP_TIMES_ROMAN_24);

        //After pause
        if(pause){
            for(int i = 0; i < 3; i++){
                iShowBMP2(menuButtonPos[i].x, menuButtonPos[i].y, menuButtonPic[i],0);
            }
            if(startOverText == 1){
                iShowBMP(100, 150, "img\\sureText.bmp");
            }
        }

        //After time over
        if(timeOver){

            iShowBMP(0,0,"img\\whiteBg.bmp");
            iShowBMP(0,0,"img\\timeOverPic.bmp");

            //Score Show
            if(noPoop){
                score += 50;
                noPoop = 0;
            }

            if(noPoopText){
                iSetColor(0,200,0);
                iText(685-40, sHeight - 50, "Bonus 50 for no poop!", GLUT_BITMAP_TIMES_ROMAN_24);
            }
            char finalScore[50];
            sprintf(finalScore, "Your score : %d",score);
            iSetColor(0,0,0);
            iText(685, sHeight - 100, finalScore, GLUT_BITMAP_HELVETICA_18);

            //Name store

            //Whole input box
            if(inputName){
                iSetColor(166,161,159);
                iFilledRectangle((sWidth/2)+80, (sHeight/2)+30, 320, 150);
                //Prompt for name
                iSetColor(6,126,222);
                iText((sWidth/2)+87, (sHeight/2)+30+110, "To store score enter your name", GLUT_BITMAP_TIMES_ROMAN_24);
                //Name input field
                iSetColor(255,255,255);
                iFilledRectangle((sWidth/2)+100, (sHeight/2)+30+30, 280, 50);
                //Name type
                iSetColor(0,0,0);
                iText((sWidth/2)+165, (sHeight/2)+30+50, playerName, GLUT_BITMAP_TIMES_ROMAN_24);
                //OK box
                iSetColor(166,161,159);
                iFilledRectangle((((sWidth/2)+80)+(320/2))-25, (sHeight/2)-10, 50, 30);
                iSetColor(0,0,0);
                iText((((sWidth/2)+80)+(320/2))-15, (sHeight/2)-2, "OK", GLUT_BITMAP_HELVETICA_18);

            }
                iShowBMP2((sWidth/2)+180, 204, "img\\startButton.bmp", 0);
                iShowBMP2((sWidth/2)+180, 73, "img\\exitButton.bmp", 0);
        }

    }

    else if(scene == 2){
        //Resume the game

    }
    else if(scene == 3){
        //Highscore menu
        //BG pic
        iShowBMP(0,0,"img\\highScoreBg.bmp");

        //60 secs
        if(show60){
            iSetColor(0,0,0);
            iText(385, sHeight-35, "High Scores of 60 seconds", GLUT_BITMAP_TIMES_ROMAN_24);

            //Marking
            iSetColor(0,0,0);
            iFilledRectangle(529, 90, 145,5);

            for(int i = 0; i < 10; i++){
                char temp[33];
                sprintf(temp, "%d) ",(i+1));
                strcat(temp,showboard[i]);
                iText(80,(sHeight-180)-(i*50),temp, GLUT_BITMAP_HELVETICA_18);
            }
        }
        //90 secs
        if(show90){
            iSetColor(0,0,0);
            iText(385, sHeight-35, "High Scores of 90 seconds", GLUT_BITMAP_TIMES_ROMAN_24);

            //Marking
            iSetColor(0,0,0);
            iFilledRectangle(689, 90, 145,5);

            for(int i = 0; i < 10; i++){
                char temp[33];
                sprintf(temp, "%d) ",(i+1));
                strcat(temp,showboard[i]);
                iText(80,(sHeight-180)-(i*50),temp, GLUT_BITMAP_HELVETICA_18);
            }
        }
        //120secs
        if(show120){
            iSetColor(0,0,0);
            iText(385, sHeight-35, "High Scores of 120 seconds", GLUT_BITMAP_TIMES_ROMAN_24);

            //Marking
            iSetColor(0,0,0);
            iFilledRectangle(849, 90, 145,5);

            for(int i = 0; i < 10; i++){
                char temp[33];
                sprintf(temp, "%d) ",(i+1));
                strcat(temp,showboard[i]);
                iText(80,(sHeight-180)-(i*50),temp, GLUT_BITMAP_HELVETICA_18);
            }
        }
        //Back button
        iShowBMP(50, sHeight - (1.5*60), "img\\backButton.bmp");

        //Time
        timeOnCounter = 0;
    }
    else if(scene == 4){
        //Instruction menu
        //White Bg
        iShowBMP(0,0,"img\\whitebg.bmp");
        iSetColor(0,0,0);

        for(int i = 0; i < 7; i++){
            char *tempStr;
            tempStr = (char*)malloc(sizeof(char)*((strlen(insStr[i]))+4));
            sprintf(tempStr, "%d- ",(i+1));
            strcat(tempStr, insStr[i]);
            iText(50,sHeight-200-(50*i), tempStr, GLUT_BITMAP_HELVETICA_18);
        }
        iShowBMP(50, sHeight - (1.5*60), "img\\backButton.bmp");

        //Time
        timeOnCounter = 0;
    }

}


/*
    function iMouseMove() is called when the user presses and drags the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    //place your codes here
    if(scene == 1 && !pause){
        if(mx < 1000-140 && mx > 0){
            basketPos = mx;
        }
    }
}

/*
    function iMouse() is called when the user presses/releases the mouse.
    (mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here

        //FOR MENU
        if(scene == -1){
            for(int i = 0; i < 3; i++){
                if(mx >= menuButtonPos[i].x && mx <= menuButtonPos[i].x + 140 && my >= menuButtonPos[i].y && my <= menuButtonPos[i].y + 60){
                    {
                        if(i == 2 && played){
                            menuMusic = false;
                            PlaySound(0,0,0);
                            PlaySound("music\\gamePlay.wav", NULL, SND_LOOP || SND_ASYNC);
                            scene = 1;
                        }
                        else if(i == 2 && !played){
                            showNotPlayed = 1;
                        }
                        else{
                            if(i == 1){
                                //show time select
                                selectTime = 1;
                            }
                            if(i != 2 && i!=1){
                                for(int i = 0; i < 10; i++){
                                    free(names60[i]);
                                    free(names90[i]);
                                    free(names120[i]);
                                }
                                scene = i;
                            }
                        }
                    }

                    if(scene == 1){
                        //Stopping menu sound
                        pause = 0;
                        menuMusic = false;
                        timeOverMusic = false;
                        PlaySound(0,0,0);
                        timeOver = 0;
                        //Game Play sound
                        gamePlayMusic = true;
                        if(gamePlayMusic){
                            PlaySound("music\\gamePlay.wav", NULL, SND_LOOP|SND_ASYNC);
                        }
                    }
                }
            }

            //select time
            if(selectTime){
                if(my >= 319 && my <= 350){
                    for(int i = 0; i < 3; i++){
                        if(mx >= 225*(i+1) && mx <= 225*(i+1) + 100){
                            if(i == 0)
                                duration = 60;
                            if(i == 1)
                                duration = 90;
                            if(i == 2)
                                duration = 120;
                            showNotPlayed = 0;
                            played = 1;
                            //Stopping menu sound
                            pause = 0;
                            menuMusic = false;
                            timeOverMusic = false;
                            PlaySound(0,0,0);
                            timeOver = 0;
                            //Game Play sound
                            gamePlayMusic = true;
                            if(gamePlayMusic){
                                PlaySound("music\\gamePlay.wav", NULL, SND_LOOP|SND_ASYNC);
                            selectTime = 0;
                            scene = 1;
                        }
                    }
                }
            }
        }

    }
        if(scene == -1){
            if(mx >= (sWidth/2) + 290 && mx <= (sWidth/2) + 290 + 150 && my >= 13+(sHeight/14) && my <= 13+(sHeight/14) + 60){
                scene = 3;//for high score menu
            }
            if(mx >= (sWidth/3) - 270 && mx <= (sWidth/3) - 270+140 && my >= 13+(sHeight/14) && my <= 13+(sHeight/14)+60){
                scene = 4;//Instruction menu
            }
        }

        //Back Button for High and instruction
        if(scene == 3 || scene == 4){
            if(mx >= 50 && mx <= 50+140 && my >= sHeight - (1.5*60) && my <= sHeight - (1.5*60)+60){
                show120 = 0;
                show60 = 0;
                show90 = 0;
                scene = -1;
            }
        }

        if(scene == 3){
            if(my >= 0 && my <= 84){
                if(mx >= 529 && mx <= 674){//60 secs
                    //Show 60 secs highscore
                    show60 = 1;
                    show90 = 0;
                    show120 = 0;

                    //Read file and show
                    if((fp = fopen("file\\highScore60txt.txt", "r")) == NULL){
                        printf("Error opening file");
                    }

                    int cBoard = 0;//for maintaining iText yPosition
                    while(!feof(fp)){
                        fgets(showboard[cBoard], 30, fp);
                        cBoard++;
                    }
                    fclose(fp);
        }
                if(mx >= 689 && mx <= 833){//90 secs
                    //Show 90 secs highscore
                    show60 = 0;
                    show90 = 1;
                    show120 = 0;

                    //Read file and show
                    if((fp = fopen("file\\highScore90txt.txt", "r")) == NULL){
                        printf("Error opening file");
                    }

                    int cBoard = 0;
                    while(!feof(fp)){
                        fgets(showboard[cBoard], 30, fp);
                        cBoard++;
                    }
                    fclose(fp);
                }
                if(mx >= 849 && mx <= 1000){//120 secs
                    //Show 120 secs highscore
                    show60 = 0;
                    show90 = 0;
                    show120 = 1;

                    //Read file and show
                    if((fp = fopen("file\\highScore120txt.txt", "r")) == NULL){
                        printf("Error opening file");
                    }

                    int cBoard = 0;//for maintaining iText yPosition
                    while(!feof(fp)){
                        fgets(showboard[cBoard], 30, fp);
                        cBoard++;
                    }
                    fclose(fp);
                }
            }
        }

        //music Control button
        if(mx >= sWidth - (2*60) && mx <= sWidth - (2*60) + 60 && my >= sHeight - (1.6*60) && my <= sHeight - (1.6*60) + 60){
            //Menu
            if(menuMusic && scene == -1){
                menuMusic = false;
                PlaySound(0,0,0);
            }
            else if(menuMusic == false && scene == -1){
                menuMusic = true;
                PlaySound("music\\menuSound.wav", NULL, SND_LOOP|SND_ASYNC);
            }
            //In game
            if(gamePlayMusic && scene == 1){
                gamePlayMusic = false;
                PlaySound(0,0,0);
            }
            else if(gamePlayMusic == false && scene == 1){
                gamePlayMusic = true;
                PlaySound("music\\gamePlay.wav", NULL, SND_LOOP|SND_ASYNC);
            }
        }

        //GamePlay and pause
        if(mx >= 50 && mx <= 150 && my >= 610 && my <= 663){
            if(scene == 1 && !pause)
                pause = 1;
            else if(scene == 1 && pause)
                pause = 0;
        }

        //Pause Button
        if(scene == 1 && pause &&!timeOver){
            if(mx >= 444 && mx <= 568 && my >= 256 && my <= 308){
                pause = 0;
            }
            if(mx >= 444 && mx <= 568 && my >= 157 && my <= 205){
                //Are u sure u wanna start over?
                startOverText = 1;
            }
            if(mx >= 444 && mx <= 568 && my >= 56 && my <= 105){
                gamePlayMusic = false;
                PlaySound(0,0,0);
                menuMusic = true;
                PlaySound("music\\menuSound", NULL, SND_LOOP || SND_ASYNC);
                scene = -1;
            }
            if(startOverText){
                if(mx >= 271 && mx <= 443 && my >= 278 && my <= 338){
                    //Yes button
                    //Reset all values and resume

                    //Chicken number
                    numChick = 1;
                    //chicken position
                    chickenposX[0] = rand()%800;
                    chickenposX[1] = chickenposX[0]+100;
                    chickenposY = sHeight - 195;
                    for(int i = 0; i < 2; i++)
                        layPosX[i] = chickenposX[i];
                    //Basket Thing
                    basketPos = (sWidth/2)-70;
                    dxBasket = 100;

                    for(int i = 0; i < 2; i++){
                        nrmlEggPos[i] = sHeight - 200;
                        blueEggPos[i] = sHeight - 200;
                        poopPos[i] = sHeight - 200;
                        goldenEggPos[i] = sHeight - 200;
                        movePos[i] = sHeight - 200;
                        stop[i] = 0;
                        anim[i] = 0;
                    }
                    //Bonuses
                    noPoop = 1;

                    jump = 30;

                    sec = 0;
                    mins = 0;
                    timeOnCounter = 0;
                    totalSecs = 0;

                    //gravity
                    dy = 3;

                    score = 0;

                    startOverText = 0;
                    pause = 0;
                    Sleep(100);
                }
                if(mx >= 579 && mx <= 742 && my >= 281 && my <= 335){
                    //No button
                    startOverText = 0;//make it false
                }
            }

        }

        //Time Over menu
        if(scene == 1 && timeOver && !pause){
            if(mx >= 0 && mx <= 500 && my >= 0 && my <= 700){
                //timeOver menu sound
                timeOverMusic = true;
                if(timeOverMusic)
                    PlaySound("music\\ohNo.wav", NULL, SND_ASYNC);
            }
            if(mx >= 715 && mx <= 766 && my >= 340 && my <= 370){
                //pressing OK
                if(duration == 60){

                    if(score > highScore60[9]){
                        highScore60[9] = score;
                        //String processing****
                        nameProcessing(playerName);
                        int l = strlen(playerName);
                        names60[9] = (char*)malloc(sizeof(char)*(l+1));
                        strcpy(names60[9], playerName);
                        sortScore(highScore60, names60, 10);
                        //File Re-write
                        if((fp = fopen("file\\highScore60txt.txt", "w")) == NULL){
                            printf("Error opening file");
                        }

                        for(int i = 0; i < 10; i++){
                            fprintf(fp, "%s %d\n",names60[i],highScore60[i]);
                        }

                        fclose(fp);
                    }

                }
                if(duration == 90){

                    if(score > highScore90[9]){
                        highScore90[9] = score;
                        nameProcessing(playerName);
                        int l = strlen(playerName);
                        names90[9] = (char*)malloc(sizeof(char)*(l+1));
                        strcpy(names90[9], playerName);
                        sortScore(highScore90, names90, 10);
                        //File Re-write
                        if((fp = fopen("file\\highScore90txt.txt", "w")) == NULL){
                            printf("Error opening file");
                        }

                        for(int i = 0; i < 10; i++){
                            fprintf(fp, "%s %d\n",names90[i],highScore90[i]);
                        }

                        fclose(fp);
                    }

                }
                if(duration == 120){

                    if(score > highScore120[9]){
                        highScore120[9] = score;
                        nameProcessing(playerName);
                        int l = strlen(playerName);
                        names120[9] = (char*)malloc(sizeof(char)*(l+1));
                        strcpy(names120[9], playerName);
                        sortScore(highScore120, names120, 10);
                        //File Re-write
                        if((fp = fopen("file\\highScore120txt.txt", "w")) == NULL){
                            printf("Error opening file");
                        }

                        for(int i = 0; i < 10; i++){
                            fprintf(fp, "%s %d\n",names120[i],highScore120[i]);
                        }

                        fclose(fp);
                    }

                }
                inputName = 0;
            }
            if(mx >= 688 && mx <= 813 && my >= 210 && my <= 259){//start button
                timeOverMusic = false;
                PlaySound(0,0,0);
                gamePlayMusic = true;
                if(gamePlayMusic)
                    PlaySound("music\\gamePlay.wav", NULL, SND_ASYNC || SND_LOOP);
                timeOver = 0;
                //reset everything
                //Chicken number
                    numChick = 1;
                    //chicken position
                    chickenposX[0] = rand()%800;
                    chickenposX[1] = chickenposX[0]+100;
                    chickenposY = sHeight - 195;
                    for(int i = 0; i < 2; i++)
                        layPosX[i] = chickenposX[i];
                    //Basket Thing
                    basketPos = (sWidth/2)-70;
                    dxBasket = 100;

                    for(int i = 0; i < 2; i++){
                        nrmlEggPos[i] = sHeight - 200;
                        blueEggPos[i] = sHeight - 200;
                        poopPos[i] = sHeight - 200;
                        goldenEggPos[i] = sHeight - 200;
                        movePos[i] = sHeight - 200;
                        stop[i] = 0;
                        anim[i] = 0;
                        once[i] = 1;
                    }
                    //Bonuses
                    noPoop = 1;

                    jump = 30;

                    sec = 0;
                    mins = 0;
                    timeOnCounter = 0;
                    totalSecs = 0;

                    //gravity
                    dy = 3;

                    score = 0;

                    inputName = 1;
                    startOverText = 0;
                    timeOver = 0;
                    Sleep(100);
                }
            else if(mx >= 668 && mx <= 810 && my >= 81 && my <= 128){//exit to menu
                    timeOverMusic = false;
                    PlaySound(0,0,0);
                    menuMusic = true;
                    if(menuMusic)
                        PlaySound("music\\menuSound.wav", NULL, SND_LOOP || SND_ASYNC);
                    timeOver = 0;
                    played = 0;
                    //reset everything
                //Chicken number
                    numChick = 1;
                    //chicken position
                    chickenposX[0] = rand()%800;
                    chickenposX[1] = chickenposX[0]+100;
                    chickenposY = sHeight - 195;
                    for(int i = 0; i < 2; i++)
                        layPosX[i] = chickenposX[i];
                    //Basket Thing
                    basketPos = (sWidth/2)-70;
                    dxBasket = 100;

                    for(int i = 0; i < 2; i++){
                        nrmlEggPos[i] = sHeight - 200;
                        blueEggPos[i] = sHeight - 200;
                        poopPos[i] = sHeight - 200;
                        goldenEggPos[i] = sHeight - 200;
                        movePos[i] = sHeight - 200;
                        stop[i] = 0;
                        anim[i] = 0;
                        once[i] = 1;
                    }
                    //Bonuses
                    noPoop = 1;

                    jump = 30;

                    sec = 0;
                    mins = 0;
                    timeOnCounter = 0;
                    totalSecs = 0;

                    //gravity
                    dy = 3;

                    score = 0;

                    inputName = 1;
                    startOverText = 0;
                    timeOver = 0;
                    Sleep(100);
                    scene = -1;
            }
        }
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here

    }
}

/*
    function iKeyboard() is called whenever the user hits a key in keyboard.
    key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(key == 'q')
    {
        for(int i = 0; i < 10; i++){
            free(names60[i]);
            free(names90[i]);
            free(names120[i]);
        }
        exit(0);
    }
    //place your codes for other keys here
    if(key == 's'){

    }
    if(key == 'a' && scene == 1 && !pause){
        if(basketPos > dxBasket){
            basketPos -= dxBasket;
        }
    }
    if(key == 'd' && scene == 1 && !pause){
        if(basketPos < 1000-140-dxBasket){
            basketPos += dxBasket;
        }
    }

    //Name input
    if(timeOver){
        if(key != '\b'){
            if(indexPlayer < 15){
                playerName[indexPlayer] = key;
                playerName[++indexPlayer] = '\0';
            }
        }
        else{
            if(indexPlayer > 0){
                playerName[--indexPlayer] = '\0';
            }
        }
    }
}

/*
    function iSpecialKeyboard() is called whenever user hits special keys like-
    function keys, home, end, pg up, pg down, arrows etc. you have to use
    appropriate constants to detect them. A list is:
    GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
    GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
    GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
    GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{

    if(key == GLUT_KEY_END)
    {
        for(int i = 0; i < 10; i++){
            free(names60[i]);
            free(names90[i]);
            free(names120[i]);
        }
        exit(0);
    }
    if(key == GLUT_KEY_RIGHT && scene == 1 && !pause){
        if(basketPos < 1000-140-dxBasket){
            basketPos += dxBasket;
        }
    }
    if(key == GLUT_KEY_LEFT && scene == 1 && !pause){
        if(basketPos > dxBasket){
            basketPos -= dxBasket;
        }
    }
    //place your codes for other keys here
}


int main()
{
    //60 duration
    fp = fopen("file\\highScore60txt.txt", "r");
    char temp[20];
    int k = 0;

    while(!feof(fp)){
        fscanf(fp, "%s %d",temp,&highScore60[k]);
        names60[k] = (char*)malloc(sizeof(char)*(strlen(temp)+1));
        strcpy(names60[k],temp);
        k++;
    }
    fclose(fp);

    //90 duration
    fp3 = fopen("file\\highScore90txt.txt", "r");
    k = 0;

    while(!feof(fp3)){
        fscanf(fp3, "%s %d",temp,&highScore90[k]);
        names90[k] = (char*)malloc(sizeof(char)*(strlen(temp)+1));
        strcpy(names90[k],temp);
        k++;
    }
    fclose(fp3);
    //120 duration
    fp2 = fopen("file\\highScore120txt.txt", "r");
    k = 0;

    while(!feof(fp2)){
        fscanf(fp2, "%s %d",temp,&highScore120[k]);
        names120[k] = (char*)malloc(sizeof(char)*(strlen(temp)+1));
        strcpy(names120[k],temp);
        k++;
    }
    fclose(fp2);


    //menuButton position set
    for(int i = 0; i < 3; i++){
        menuButtonPos[i].x = (sWidth/2)-65;
        menuButtonPos[i].y = (i*100)+(sHeight/14);
    }
    //Menu music
    if(menuMusic){
        PlaySound("music\\menuSound.wav", NULL, SND_LOOP|SND_ASYNC);
    }

    //First position set
    int fPosX = (rand()%1000) + 100;
    if(fPosX < 850){
        chickenposX[0] = fPosX;
    }
    else{
        chickenposX[0] = 850;
    }
    chickenposX[1] = chickenposX[0] + 95;

    iSetTimer(1000, timeFunction);

    iSetTimer(23, laidThingsOne);
    iSetTimer(45, laidThingsTwo);

    iSetTimer(20, gravityOne);
    iSetTimer(20, gravityTwo);

    iSetTimer(30, rainBonus);
    iSetTimer(20, rainGravity);

    iSetTimer(1000, timeMusic);

    iInitialize(sWidth, sHeight, "Catch the egg");

    return 0;
}
