#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <FEHSD.h>


class Adjust {
    public:
        Adjust(float a = 0, int b = 0);

        void rForward(float distance, float Speed);
        void rReverse(float distance, float Speed);
        void turnRIGHT(float degrees, float Speed);
        void turnLEFT(float degrees, float Speed);
        void resetPID();
        float RPID(float currenttime, float expectedvelocity, int i);

        float oldTime[2], oldError[2], errorSum[2], oldMotorPower[2];
        int oldCounts[2];
        float startTime;
};

// Front bump switch
DigitalInputPin frontBump(FEHIO::P1_7);

// Declare CDS Cell
// Declare Magnet
AnalogInputPin cdsCell(FEHIO::P3_7);
DigitalOutputPin magnet(FEHIO::P3_0);

// Declare Left Motor
// Declare Right Motor
// Declare Crank Servo
// Declare Wrench Servo

FEHMotor left_drive(FEHMotor::Motor0,7.2);
FEHMotor right_drive(FEHMotor::Motor1,7.2);
FEHServo crankServo(FEHServo::Servo7);
FEHServo wrenchServo(FEHServo::Servo0);

// declare encoders
DigitalEncoder left_encoder(FEHIO::P2_0);
DigitalEncoder right_encoder(FEHIO::P1_0);



void stopAll();
void resetCounts();
void driveForward(int percent, int counts);
void turnRight(int percent, int counts);
void turnLeft(int percent, int counts);
void check_x_plus(float x_coordinate);
void check_x_minus(float x_coordinate);
void check_y_plus(float y_coordinate);
void check_y_minus(float y_coordinate);
void check_heading(float heading);
void check_heading360();
void calibrateServo(int turn);
void returnCoords();
bool bumpTest();

#define northH 90.0
#define eastH 360.0
#define southH 270.0
#define westH 180.0

#define PI 3.14159

#define Pconst 0.5
#define Iconst 0.04
#define Dconst 0.25

int main(void)
{


    float touch_x,touch_y;

    RPS.InitializeTouchMenu();

    float startTime = TimeNow();

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );
    LCD.SetBackgroundColor(BLACK);

    SD.OpenLog();

    SD.Printf("START LOG\n");


    // WAIT FOR START LIGHT
    bool wait = false;
    float lightThreshold = .70;
     while (wait){

        LCD.WriteLine(cdsCell.Value());
        if ((cdsCell.Value() < lightThreshold)){
            wait = false;
        }
        Sleep(.25);
        LCD.Clear();
    }

     Adjust PID;

     // Turn Magnet ON
     LCD.WriteLine("Magnet ON");
     magnet.Write(true);

     float startX = RPS.X();
     float startY = RPS.Y();
     float vel = 20.0;

     // DRIVE OUT OF START ZONE
     PID.rForward(6.0, vel);

     //check_y_minus(startY - 6.1);

     wrenchServo.SetMin(500);
     wrenchServo.SetMax(2300);
     LCD.WriteLine("Servo Calibrated");

     wrenchServo.SetDegree(0);
     LCD.WriteLine("Servo at 0 degrees");

    for (int i = 0; i < 90; i++){
        LCD.Clear();
        wrenchServo.SetDegree(2 * i);
        LCD.WriteLine("Servo at: ");
        LCD.WriteLine(2*i);
        LCD.WriteLine("degrees");
        Sleep(1.0);
    }

     Sleep(5.0);
     LCD.Clear();

     SD.CloseLog();

     Sleep(100.0);

    return 0;
}

bool bumpTest(){

    if (frontBump.Value() == 1){
        return true;
    }else{
        return false;
    }

}

void stopAll()
{
    // stop both motors for both wheels
    left_drive.Stop();
    right_drive.Stop();
     Sleep(.2);
}

void resetCounts()
{
    // reset encoder counts for both motors
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
}

void driveForward(int percent, int counts)
{

    //Reset encoder counts
    resetCounts();

    int constant = 0;

    if (percent < 0){
        constant = -4;
    }else{
        constant = 4;
    }

    //Set both motors to desired percent
    right_drive.SetPercent(2*percent + constant);
    left_drive.SetPercent(2*percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);


        //Turn off motors
        stopAll();

}


void turnRight(int percent, int counts)
{
    //Reset encoder counts
    resetCounts();

    //Set both motors to desired percent
    right_drive.SetPercent(-1 * percent - 2);
    left_drive.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts)
    {
    }

    //Turn off motors
    stopAll();
}

void turnLeft(int percent, int counts)
{
    //Reset encoder counts
    resetCounts();

    //Set both motors to desired percent
    right_drive.SetPercent(percent + 2);
    left_drive.SetPercent(-1 * percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts)
    {
    }

    //Turn off motors
    stopAll();
}

void check_x_plus(float x_coordinate)
//using RPS while robot is in the +x direction

{

    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        LCD.Clear();
        LCD.WriteLine("CHECK X PLUS");
        LCD.WriteLine("Current X: ");
        LCD.WriteLine(RPS.X());
        LCD.WriteLine("Correcting to: ");
        LCD.WriteLine(x_coordinate);

        if(RPS.X() > x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-15);
            left_drive.SetPercent(-15);
            Sleep(100);
            stopAll();
        }
        else if(RPS.X() < x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(15);
            left_drive.SetPercent(15);
            Sleep(100);
            stopAll();
        }
    }
}

void check_x_minus(float x_coordinate)
//using RPS while robot is in the -x direction

{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        LCD.Clear();
        LCD.WriteLine("CHECK X MINUS");
        LCD.WriteLine("Current X: ");
        LCD.WriteLine(RPS.X());
        LCD.WriteLine("Correcting to: ");
        LCD.WriteLine(x_coordinate);

        if(RPS.X() > x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(20);
            left_drive.SetPercent(20);
            Sleep(100);
            stopAll();
        }
        else if(RPS.X() < x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-20);
            left_drive.SetPercent(-20);
            Sleep(100);
            stopAll();
        }
    }
}

void check_y_plus(float y_coordinate)
//using RPS while robot is in the +y direction

    {

//check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {

        LCD.Clear();
        LCD.WriteLine("CHECK Y PLUS");
        LCD.WriteLine("Current Y: ");
        LCD.WriteLine(RPS.Y());
        LCD.WriteLine("Correcting to: ");
        LCD.WriteLine(y_coordinate);

        if(RPS.Y() > y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-20);
            left_drive.SetPercent(-20);
            Sleep(100);
            stopAll();
        }
        else if(RPS.Y() < y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(20);
            left_drive.SetPercent(20);
            Sleep(100);
            stopAll();
        }
    }
    }

void check_y_minus(float y_coordinate)
//using RPS while robot is in the -y direction

{

//check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {

        LCD.Clear();
        LCD.WriteLine("CHECK Y MINUS");
        LCD.WriteLine("Current Y: ");
        LCD.WriteLine(RPS.Y());
        LCD.WriteLine("Correcting to: ");
        LCD.WriteLine(y_coordinate);

        if(RPS.Y() > y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(20);
            left_drive.SetPercent(20);
            Sleep(100);
            stopAll();
        }
        else if(RPS.Y() < y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-20);
            left_drive.SetPercent(-20);
            Sleep(100);
            stopAll();
        }
    }
}

void check_heading(float heading) //using RPS

{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)

    float newHeading = 0;

    if (heading != -1){
        if((heading < 2 && heading > 0) || (heading < 360 && heading > 358))
        {
            heading = 5;
            if(RPS.Heading() <= 2 && RPS.Heading() >= 0){
                newHeading = 5 + RPS.Heading();        }
            else
            {
                newHeading = 5 - (360-RPS.Heading());
            }
            while(newHeading < heading - 2 || newHeading > heading + 2)
            {
                if(newHeading > heading)
                {

                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(-15);
                    left_drive.SetPercent(15);
                    Sleep(100);
                    stopAll();
                }
                else if(newHeading < heading)
                {

                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(15);
                    left_drive.SetPercent(-15);
                    Sleep(100);
                    stopAll();
                }
                if(RPS.Heading() <= 2 && RPS.Heading() >= 0)
                {
                    newHeading = 5 + RPS.Heading();
                }
                else
                {
                    newHeading = 5 - (360-RPS.Heading());
                }
            }
        }
        else
        {
            while(RPS.Heading() < heading - 2 || RPS.Heading() > heading + 2)
            {
                if(RPS.Heading() > heading)
                {
                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(-15);
                    left_drive.SetPercent(15);
                    Sleep(100);
                    stopAll();
                }
                else if(RPS.Heading() < heading)
                {
                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(15);
                    left_drive.SetPercent(-15);
                    Sleep(100);
                    stopAll();
                }
            }
        }
    }else{
        LCD.WriteLine("In Dead Zone!");
    }


    Sleep(.1);
}

void check_heading360() //using RPS

{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)

    float newHeading = 0;
    LCD.WriteLine("CHECKING 360");

    float heading = RPS.Heading();

    if (heading != -1){
        if((heading < 90) &&( heading > 0))
        {
//            heading = 5;
//            if(RPS.Heading() <= 2 && RPS.Heading() >= 0){
//                newHeading = 5 + RPS.Heading();        }
//            else
//            {
//                newHeading = 5 - (360-RPS.Heading());
//            }

            newHeading = RPS.Heading();

            while((newHeading < 90)&&(newHeading > 0))
            {
                // needs to turn RIGHT
                newHeading = RPS.Heading();
                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(-20);
                    left_drive.SetPercent(20);
                    Sleep(200);
                    stopAll();
            }
        }else{
            {
    //            heading = 5;
    //            if(RPS.Heading() <= 2 && RPS.Heading() >= 0){
    //                newHeading = 5 + RPS.Heading();        }
    //            else
    //            {
    //                newHeading = 5 - (360-RPS.Heading());
    //            }

                newHeading = RPS.Heading();

                while((newHeading > 270)&&(newHeading < 359))
                {
                    // needs to turn LEFT
                        //pulse the motors for a short duration in the correct direction
                        right_drive.SetPercent(20);
                        left_drive.SetPercent(-20);
                        Sleep(200);
                        newHeading = RPS.Heading();
                        stopAll();
                }
        }
    }
    }else{
        LCD.WriteLine("In Dead Zone!");
    }

    Sleep(.1);

}


void calibrateServo(int turn){

    crankServo.SetMin(917);
    crankServo.SetMax(2500);

    if (turn == 1){
        crankServo.SetDegree(0);
        LCD.WriteLine("CALIBRATED: Turn 1");

    }else{
        crankServo.SetDegree(180);
        LCD.WriteLine("CALIBRATED: Turn 2");
    }
}


void returnCoords()
{

    float touch_x,touch_y;

     while(!LCD.Touch(&touch_x,&touch_y))
     {
        LCD.Clear();
        LCD.WriteLine("X:");
        LCD.WriteLine(RPS.X());
        LCD.WriteLine("Y:");
        LCD.WriteLine(RPS.Y());
        LCD.WriteLine("Heading:");
        LCD.WriteLine(RPS.Heading());
        LCD.WriteLine("PRESS TO CONTINUE");
        Sleep(.5);
    }
}


// CLASS CONSTRUCTOR
// and CLASS FUNCTIONS

Adjust::Adjust(float a, int b)
{
    // right motor
    oldError[0] = a;
    oldMotorPower[0] = a;
    oldTime[0] = a;
    errorSum[0] = a;
    oldCounts[0] = b;

    // left motor
    oldError[1] = a;
    oldMotorPower[1] = a;
    oldTime[1] = a;
    errorSum[1] = a;
    oldCounts[1] = b;

    startTime = TimeNow();
}


void Adjust::rForward(float distance, float Speed)
{
    float currentTime, dummy;
    bool flag;
    LCD.WriteLine("rFORWARD");
    oldMotorPower[0] = 0;
    oldMotorPower[1] = 0;
    oldTime[0] = TimeNow();
    oldTime[1] = TimeNow();
    Speed = Speed * 2;
    resetPID();
    flag = true;
    while (flag){
        oldTime[0] = TimeNow();
        oldTime[1] = TimeNow();
        Sleep(.1);
        dummy = (((right_encoder.Counts() + left_encoder.Counts()) / 2.0 ) * (((3.0*PI)/180.0)));
        flag = (dummy < distance);
        currentTime = TimeNow();
        //go forward
        right_drive.SetPercent(RPID(currentTime,Speed, 0));
        left_drive.SetPercent(RPID(currentTime,Speed, 1));
        }
    stopAll();
    Sleep(.1);
}

void Adjust::rReverse(float distance, float Speed)
{
    float currentTime, dummy;
    bool flag;
    LCD.WriteLine("rREVERSE");

    oldMotorPower[0] = 0;
    oldMotorPower[1] = 0;
    oldTime[0] = TimeNow();
    oldTime[1] = TimeNow();

    resetPID();
    flag = true;

    while (flag){
        oldTime[0] = TimeNow();
        oldTime[1] = TimeNow();

        Sleep(.1);
        dummy = (((right_encoder.Counts() + left_encoder.Counts()) / 2.0 ) * (((3.0*PI)/180.0)));

        //LCD.WriteLine(dummy);
        //LCD.WriteLine(((right_encoder.Counts() + left_encoder.Counts())/2));

        flag = (dummy < distance);
        currentTime = TimeNow();

        //go forward
        right_drive.SetPercent(-1 * RPID(currentTime,Speed, 0));
        left_drive.SetPercent(-1 * RPID(currentTime,Speed, 1));
        }
    stopAll();
    Sleep(.1);
}

void Adjust::turnRIGHT(float degrees, float Speed){

    float currentTime, dummy;
    bool flag;
    LCD.WriteLine("turnRIGHT");

    float distance = degrees / 15;

    oldMotorPower[0] = 0;
    oldMotorPower[1] = 0;
    oldTime[0] = TimeNow();
    oldTime[1] = TimeNow();

    resetPID();
    flag = true;

    while (flag){
        oldTime[0] = TimeNow();
        oldTime[1] = TimeNow();

        Sleep(.1);
        dummy = (((right_encoder.Counts() + left_encoder.Counts()) / 2.0 ) * (((3.0*PI)/180.0)));

        //LCD.WriteLine(dummy);
        //LCD.WriteLine(((right_encoder.Counts() + left_encoder.Counts())/2));

        flag = (dummy < distance);
        currentTime = TimeNow();

        //go forward
        right_drive.SetPercent(-1 * RPID(currentTime,Speed, 0));
        left_drive.SetPercent(RPID(currentTime,Speed, 1));
        }
    stopAll();
    Sleep(.1);

}

void Adjust::turnLEFT(float degrees, float Speed){

    float currentTime, dummy;
    bool flag;
    LCD.WriteLine("turnLEFT");

    float distance = degrees / 15;

    oldMotorPower[0] = 0;
    oldMotorPower[1] = 0;
    oldTime[0] = TimeNow();
    oldTime[1] = TimeNow();

    resetPID();
    flag = true;

    while (flag){
        oldTime[0] = TimeNow();
        oldTime[1] = TimeNow();

        Sleep(.1);
        dummy = (((right_encoder.Counts() + left_encoder.Counts()) / 2.0 ) * (((3.0*PI)/180.0)));

        //LCD.WriteLine(dummy);
        //LCD.WriteLine(((right_encoder.Counts() + left_encoder.Counts())/2));

        flag = (dummy < distance);
        currentTime = TimeNow();

        //go forward
        right_drive.SetPercent(RPID(currentTime,Speed, 0));
        left_drive.SetPercent(-1 *RPID(currentTime,Speed, 1));
        }
    stopAll();
    Sleep(.1);

}


void Adjust::resetPID()
{
    //reset variables
    //record time
    //reset encoders
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    float a = 0;
    int b = 0;

    oldError[0] = a;
    oldMotorPower[0] = a;
    oldTime[0] = a;
    oldCounts[0] = b;

    // left motor
    oldError[1] = a;
    oldMotorPower[1] = a;
    oldTime[1] = a;
    oldCounts[1] = b;

    startTime = TimeNow();


    //wait
    Sleep(.1);
}

float Adjust::RPID(float currentTime, float expectedVelocity, int i)
{
    int dcounts;
    float dtime, velocity, PTerm, ITerm, DTerm, error;
    //dtime is delta time
    dtime=currentTime - oldTime[i];

    if (i == 0){
        dcounts=right_encoder.Counts() - oldCounts[i];
        SD.Printf("||||| RIGHT WHEEL |||||\n");
    }else{
        dcounts=left_encoder.Counts() - oldCounts[i];
        SD.Printf("||||| LEFT WHEEL  |||||\n");
    }


    //Calculate velocity using time and encoder counts
    SD.Printf("Dcounts: %d\n",dcounts);
    SD.Printf("Dtime: %f\n",dtime);

    velocity= (((3*PI)/90))*(dcounts/dtime);
    error = expectedVelocity - velocity;

    SD.Printf("Velocity: %f\n",velocity);
    SD.Printf("Error: %f\n",error);

    errorSum[i] += error;
    //Prop term
    PTerm = error * Pconst;
    //Integral term
    ITerm = errorSum[i] * Iconst;
    //Derivative term *don't use until other two are working
    DTerm = (error - oldError[i]) * Dconst;
    //Replace variables

    if (i == 0){
    oldCounts[i] = right_encoder.Counts();
    }else{
    oldCounts[i] = left_encoder.Counts();
    }

    // ***** oldTime[i] = currentTime;
    oldError[i] = error;

    //return adjusted power, I and D terms commented out

    //LCD.WriteLine(i);
    //LCD.WriteLine(oldMotorPower[i] + PTerm);

    oldMotorPower[i] += PTerm ; //+ ITerm;

    SD.Printf("Set Motor Percent To: %f\n",oldMotorPower[i]);
    if (i == 0){
        SD.Printf("||||| RIGHT WHEEL  |||||\n");
    }else{
        SD.Printf("||||| LEFT WHEEL   |||||\n\n\\nn");
    }

    return(oldMotorPower[i]);//+ITerm+DTerm)
}
