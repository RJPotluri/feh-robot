#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>


class Adjust {
    public:
        Adjust(float a = 0, int b = 0);

        void rForward(float distance, float Speed);
        void resetPID();
        float RPID(float currenttime, float expectedvelocity, int i);

        float oldTime[2], oldError[2], errorSum[2], oldMotorPower[2];
        int oldCounts[2];
};

//Front bump switch
DigitalInputPin frontBump(FEHIO::P3_0);
//Back bump switch
//DigitalInputPin backBump(FEHIO::P4_0);

// cds cell
AnalogInputPin cdsCell(FEHIO::P3_7);

//Declare Left Motor
FEHMotor left_drive(FEHMotor::Motor0,7.2);
//Declare Right Motor
FEHMotor right_drive(FEHMotor::Motor1,7.2);
//Declare Servo Motor
FEHServo crankServo(FEHServo::Servo7);

// declare encoders
DigitalEncoder left_encoder(FEHIO::P1_0);
DigitalEncoder right_encoder(FEHIO::P2_0);



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
void calibrateServo(int turn);

#define northH 90.0
#define eastH 360.0
#define southH 270.0
#define westH 180.0

#define PI 3.14159

#define Pconst 0.75
#define Iconst 0.1
#define Dconst 0.25

int main(void)
{


    // START ON LIGHT
//    float touch_x,touch_y;

//    RPS.InitializeTouchMenu();

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    LCD.SetBackgroundColor(BLACK);
//    bool wait = true;

//     while (wait){

//        LCD.WriteLine(cdsCell.Value());

//        if ((cdsCell.Value() < .70)){
//            wait = false;
//        }

//        Sleep(.25);
//        LCD.Clear();
//    }

//     Sleep(2.0);


     // RETURN COORDS
/*     while(!LCD.Touch(&touch_x,&touch_y)){
         LCD.Clear();
         LCD.WriteLine("X");
         LCD.WriteLine(RPS.X());
         LCD.WriteLine("Y");
         LCD.WriteLine(RPS.Y());
         LCD.WriteLine("Heading");
         LCD.WriteLine(RPS.Heading());
         LCD.WriteLine("Press Screen To Start");
         Sleep(10);
     }*/

     Sleep(1.0);

     //calibrateServo(turn);

     // initialize instance of Adjust methods as RPID
     Adjust RPID;


     LCD.WriteLine("RPID 20 inches at 10 in/s");
     RPID.rForward(20, 10);

     Sleep(10.0);

     LCD.WriteLine("RPID 10 inches at 10 in/s");
     RPID.rForward(10, 10);

     Sleep(10.0);

     LCD.WriteLine("RPID 5 inches at 10 in/s");
     RPID.rForward(5, 10);

     Sleep(100.0);

     int turn = RPS.FuelType();

     LCD.WriteLine("Move Forward");
     driveForward(15,110);
     Sleep(.2);

     LCD.WriteLine("Checking Y");
     check_y_minus(18.0);
     Sleep(.2);


     LCD.WriteLine("Turn Left");
     turnLeft(25,85);
     Sleep(.2);


     LCD.WriteLine("Check Heading");
     check_heading(30.0);
     Sleep(.2);

     LCD.WriteLine("Move Forward");
     driveForward(15,85);
     Sleep(.2);


     LCD.WriteLine("Turn Right");
     turnRight(20,70);
     Sleep(.2);


     LCD.WriteLine("Check Heading");
     check_heading(270);
     Sleep(.2);

     bool flag = false;

     int killSwitch = 0;

     while((RPS.IsDeadzoneActive() == 0)&&(killSwitch < 15)){

         LCD.WriteLine("Button: UNPRESSED");

         driveForward(20,10);

         Sleep(.5);
         killSwitch++;
         check_heading(270);

     }

     LCD.WriteLine("Button: TOUCHED");

     right_drive.SetPercent(22);
     left_drive.SetPercent(20);

     Sleep(1.5);

     stopAll();

     while (!flag){

         Sleep(.6);

         if (RPS.IsDeadzoneActive() == 2){
             flag = true;
         }
     }
     LCD.WriteLine("Button: PRESSED");

     stopAll();
     Sleep(.2);


     driveForward(-20,20);


     Sleep(.2);

     LCD.WriteLine("Turn Left");
     turnLeft(25,120);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(52);
     Sleep(.2);

     LCD.WriteLine("Move Forward");
     driveForward(15,65);
     Sleep(.2);

     LCD.WriteLine("Turn Left");
     turnLeft(25,10);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(90);
     Sleep(.2);

     LCD.WriteLine("Move Forward");
     driveForward(30,200);
     Sleep(.2);

     /*
      *
      *
      * UPPPPPPPEEEEERRRRRRRRRRRRRRRRRRR LEVEEEEELLLLLLLLLLLLLLLLLLLLLL
      *
      *
      * */

     LCD.WriteLine("UPPPPPPPEEEEERRRRRRRRRRRRRRRRRRR LEVEEEEELLLLLLLLLLLLLLLLLLLLLL");

     LCD.WriteLine("Check Heading");
     check_heading(90);
     Sleep(.2);

     LCD.WriteLine("Turn Left");
     turnLeft(25,35);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(134);
     Sleep(.2);

     LCD.WriteLine("Move Forward");
     driveForward(15,144);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(134);
     Sleep(.2);

     LCD.WriteLine("Turn Left");
     turnLeft(25,80);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(225);
     Sleep(.2);


     for (int i = 0; i < 25; i++){
         driveForward(-15,5);
         Sleep(.2);
         LCD.WriteLine("Check Heading");
         check_heading(225);
     }

     LCD.WriteLine("Check Heading");
     check_heading(225);

     LCD.WriteLine("Check Y");
     check_y_minus(62.5);
     Sleep(.2);

     LCD.WriteLine("Check X");
     check_x_minus(24.5);
     Sleep(.2);


     LCD.WriteLine("TURN CRANK");

     if(turn == 1){
         crankServo.SetDegree(180);
         LCD.WriteLine("Turn: 1");
     }else{
         crankServo.SetDegree(0);
         LCD.WriteLine("Turn: 2");
     }

     LCD.WriteLine("CRANK TURNED");

     Sleep(.2);

     LCD.WriteLine("Drive Forwards");
     driveForward(15,15);
     Sleep(.2);


     crankServo.SetMin(917);
     crankServo.SetMax(2500);

     turn = RPS.FuelType();

     Sleep(.2);

     if (turn == 1){
         crankServo.SetDegree(0);
         LCD.WriteLine("Turn: 1");

     }else{
         crankServo.SetDegree(180);
         LCD.WriteLine("Turn: 2");
     }

     for (int i = 0; i < 4; i++){
         driveForward(-15,5);
         Sleep(.5);
         LCD.WriteLine("Check Heading");
         check_heading(225);
     }

     LCD.WriteLine("Check Heading");
     check_heading(225);

     LCD.WriteLine("Check Y");
     check_y_minus(62.5);
     Sleep(.2);

     LCD.WriteLine("Check X");
     check_x_plus(24.5);
     Sleep(.2);

     LCD.WriteLine("TURN CRANK");

     if(turn == 1){
         crankServo.SetDegree(160);
         LCD.WriteLine("Turn: 1");
     }else{
         crankServo.SetDegree(20);
         LCD.WriteLine("Turn: 2");
     }

     LCD.WriteLine("CRANK TURNED AGAIN");

     Sleep(.2);

     LCD.WriteLine("Move Forward");
     driveForward(15,150);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(225);

     LCD.WriteLine("Turn Left");
     turnLeft(25,80);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(314);

     LCD.WriteLine("Move Forward");
     driveForward(15,150);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(314);

     LCD.WriteLine("Turn Right");
     turnRight(25,40);
     Sleep(.2);

     LCD.WriteLine("Check Heading");
     check_heading(270);

     LCD.WriteLine("Move Forward");
     driveForward(15,300);
     Sleep(.2);

    return 0;
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
        if(RPS.X() > x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-12);
            left_drive.SetPercent(-12);
            Sleep(100);
            stopAll();
        }
        else if(RPS.X() < x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(12);
            left_drive.SetPercent(12);
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
        if(RPS.X() > x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(12);
            left_drive.SetPercent(12);
            Sleep(100);
            stopAll();
        }
        else if(RPS.X() < x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-12);
            left_drive.SetPercent(-12);
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
        if(RPS.Y() > y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-12);
            left_drive.SetPercent(-12);
            Sleep(300);
            stopAll();
        }
        else if(RPS.Y() < y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(12);
            left_drive.SetPercent(12);
            Sleep(300);
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
        if(RPS.Y() > y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(12);
            left_drive.SetPercent(12);
            Sleep(100);
            stopAll();
        }
        else if(RPS.Y() < y_coordinate)
        {

            //pulse the motors for a short duration in the correct direction
            right_drive.SetPercent(-12);
            left_drive.SetPercent(-12);
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
                    right_drive.SetPercent(-12);
                    left_drive.SetPercent(12);
                    Sleep(200);
                    stopAll();
                }
                else if(newHeading < heading)
                {

                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(12);
                    left_drive.SetPercent(-12);
                    Sleep(200);
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
                    right_drive.SetPercent(-12);
                    left_drive.SetPercent(12);
                    Sleep(200);
                    stopAll();
                }
                else if(RPS.Heading() < heading)
                {
                    //pulse the motors for a short duration in the correct direction
                    right_drive.SetPercent(12);
                    left_drive.SetPercent(-12);
                    Sleep(200);
                    stopAll();
                }
            }
        }
    }else{
        LCD.WriteLine("In Dead Zone!");
    }


}


void calibrateServo(int turn){

    crankServo.SetMin(917);
    crankServo.SetMax(2500);

    if (turn == 1){
        crankServo.SetDegree(0);
        LCD.WriteLine("Turn: 1");

    }else{
        crankServo.SetDegree(180);
        LCD.WriteLine("Turn: 2");
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
}


void Adjust::rForward(float distance, float Speed)
{
    float currentTime, dummy;
     bool flag;

    oldTime[0] = TimeNow();
    oldTime[1] = TimeNow();

    resetPID();

    flag = true;

    while (flag){

        Sleep(.1);
        dummy = (((right_encoder.Counts() + left_encoder.Counts()) / 2.0 ) * (((3.0*PI)/90.0)));

        LCD.WriteLine(dummy);
        LCD.WriteLine(((right_encoder.Counts() + left_encoder.Counts())/2));

        flag = (dummy < distance);

        currentTime = TimeNow();
        //go forward
        right_drive.SetPercent(RPID(currentTime,Speed, 0));
        left_drive.SetPercent(RPID(currentTime,Speed, 1));
        }
    stopAll();
}

void Adjust::resetPID()
{
    //reset variables
    //record time
    //reset encoders
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();
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
    }else{
        dcounts=left_encoder.Counts() - oldCounts[i];
    }


    //Calculate velocity using time and encoder counts
    velocity= (((3*PI)/90))*(dcounts/dtime);
    error = expectedVelocity - velocity;
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

    LCD.WriteLine(i);
    LCD.WriteLine(oldMotorPower[i] + PTerm);

    oldMotorPower[i] += PTerm;

    return(oldMotorPower[i]);//+ITerm+DTerm)
}
