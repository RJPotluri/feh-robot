#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHSD.h>
//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_1);
DigitalEncoder left_encoder(FEHIO::P0_0);
FEHMotor right_motor(FEHMotor::Motor1, 9.0);
FEHMotor left_motor(FEHMotor::Motor0, 9.0);
void StopAll() //stops motors {

    right_drive.SetPercent(0);

    left_drive.SetPercent(0);
}

void move_forward(int percent, int counts) //using encoders {
//Reset encoder counts
right_encoder.ResetCounts();
left_encoder.ResetCounts();
   //Set both motors to desired percent
right_motor.SetPercent(percent);
left_motor.SetPercent(percent);
   //While the average of the left and right encoder are less than counts,
//keep running motors
while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
   //Turn off motors
right_motor.Stop();
left_motor.Stop();
}
void turn_right(int percent, int counts) //using encoders {
//Reset encoder counts
right_encoder.ResetCounts();
left_encoder.ResetCounts();
   //Set both motors to desired percent
right_motor.SetPercent(-percent);
left_motor.SetPercent(percent);
   //While the average of the left and right encoder are less than counts,    //keep running motors
while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
   //Turn off motors
right_motor.Stop();
left_motor.Stop(); }
void turn_left(int percent, int counts) //using encoders {
//Reset encoder counts
right_encoder.ResetCounts();
left_encoder.ResetCounts();
   //Set both motors to desired percent    right_motor.SetPercent(percent);
left_motor.SetPercent(-percent);
   //While the average of the left and right encoder are less than counts,    //keep running motors
while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);
   //Turn off motors
right_motor.Stop();
left_motor.Stop();

}


void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction

{

    //check whether the robot is within an acceptable range

    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)

    {

        if(RPS.X() > x_coordinate)

        {
        //pulse the motors for a short duration in the correct direction


        right_motor.SetPercent(-10);

        left_motor.SetPercent(-10);

        Sleep(10);

        StopAll();

    }


        else if(RPS.X() < x_coordinate)
        {

            //pulse the motors for a short duration in the correct direction


            right_motor.SetPercent(10);

            left_motor.SetPercent(10);

            Sleep(10);

            StopAll();

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

            right_motor.SetPercent(10);

            left_motor.SetPercent(10);

            Sleep(10);

            StopAll();


        }


        else if(RPS.Y() < y_coordinate)

        {

            //pulse the motors for a short duration in the correct direction


            right_motor.SetPercent(-10);

            left_motor.SetPercent(-10);

            Sleep(10);

            StopAll();


        }


    }

}
void check_y_plus(float y_coordinate)
//using RPS while robot is in the +y direction {
//check whether the robot is within an acceptable range
while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
{
if(RPS.Y() > y_coordinate)
{            //pulse the motors for a short duration in the correct direction

right_motor.SetPercent(-10);
left_motor.SetPercent(-10);
Sleep(10);
StopAll();
}
else if(RPS.Y() < y_coordinate)        {
//pulse the motors for a short duration in the correct direction

right_motor.SetPercent(10);
left_motor.SetPercent(10);
Sleep(10);
StopAll();
}
} }
void check_heading(float heading) //using RPS
{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)

    float newHeading = 0;

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
                right_motor.SetPercent(-10);
                left_motor.SetPercent(10);
                Sleep(10);
                StopAll();
            }
            else if(newHeading < heading)
            {

                //pulse the motors for a short duration in the correct direction
                right_motor.SetPercent(10);
                left_motor.SetPercent(-10);
                Sleep(10);
                StopAll();
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
                right_motor.SetPercent(-10);
                left_motor.SetPercent(10);
                Sleep(10);
                StopAll();
            }
            else if(RPS.Heading() < heading)
            {
                //pulse the motors for a short duration in the correct direction
                right_motor.SetPercent(10);
                left_motor.SetPercent(-10);
                Sleep(10);
                StopAll();
            }
        }
    }
}

void writeToSD() //using RPS while robot is in the +y direction {
SD.Printf("X:%f  Y:%f  Heading:%f\n", RPS.X(), RPS.Y(),
RPS.Heading()); }

int main(void) {
float touch_x,touch_y;

RPS.InitializeTouchMenu();

SD.OpenLog();

LCD.WriteLine("RPS & Data Logging Test");
LCD.WriteLine("Y");
LCD.WriteLine(RPS.Y());
LCD.WriteLine("X");
LCD.WriteLine(RPS.X());
LCD.WriteLine("Heading");
LCD.WriteLine(RPS.Heading());
LCD.WriteLine("Press Screen To Start");
while(!LCD.Touch(&touch_x,&touch_y));
//Wait for touchscreen press

LCD.Clear();

writeToSD();

LCD.WriteLine("Move Forward");
move_forward(15,400);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Checking Y");
check_y_minus(19.699);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Turn Left");
turn_left(25,220);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Check Heading");
check_heading(86.99);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Move Forward");
move_forward(15,480);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Check X");
check_x_plus(29.5);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Turn Left");
turn_left(25,220);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Check Heading");
check_heading(176);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Move Forward");
move_forward(15,120);
Sleep(2.0);
writeToSD();

LCD.WriteLine("Check Y");
check_y_minus(22.8);
writeToSD();

SD.CloseLog();

return 0; }

