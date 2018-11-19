#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>


//Front bump switch
DigitalInputPin frontBump(FEHIO::P0_0);
//Back bump switch
DigitalInputPin backBump(FEHIO::P3_0);

// cds cell
AnalogInputPin cdsCell(FEHIO::P0_7);

//Declare Left Motor
FEHMotor left_drive(FEHMotor::Motor0,7.2);
//Declare Right Motor
FEHMotor right_drive(FEHMotor::Motor1,7.2);


bool bumperTest();
void turnOne();
void turnTwo();
void leftNinety();
void rightNinety();
void leftRadius();
void rightRadius();
void bumpStraight(int x);
void driveStraight(int motorPercent, float duration);
void stopAll();


int main(void)
{


    LCD.SetBackgroundColor(BLACK);
    bool wait = true;

     while (wait){

        LCD.WriteLine(cdsCell.Value());

        if ((cdsCell.Value() < .70) && (cdsCell.Value() > .05)){
            wait = false;
        }

        Sleep(.25);
        LCD.Clear();
    }

    /*while (!LCD.Touch()){
        LCD.WriteLine("Waiting");
        Sleep(.1);
        LCD.Clear();
    }*/


    // drive out of start zone
    driveStraight(35, 1.75);
    Sleep(.5);

    // turn left towards ramp
    leftNinety();
    Sleep(.5);

    // drive to button area
    driveStraight(35, 1.5);
    Sleep(.5);
    // BUMP STRAIGHT

    stopAll();


    wait = true;

    int route = 0;
    // RED IS 1
    // BLUE IS 2

     while (wait){


        Sleep(.25);
        LCD.Clear();

        LCD.WriteLine(cdsCell.Value());

        // TEST FOR RED
        if ((cdsCell.Value() < 0.650) && (cdsCell.Value() > .05)){
            route = 1;
            wait = false;
            LCD.WriteLine("RED Route");
            bumpStraight(30);
            Sleep(.5);

            LCD.WriteLine("RIGHT RED");
            left_drive.SetPercent(0);
            right_drive.SetPercent(-30);
            Sleep(3.5);

            driveStraight(20, 1.0);

            left_drive.SetPercent(0);
            right_drive.SetPercent(-20);
            Sleep(1.3);

            driveStraight(30, 1.7);

            Sleep (1.0);

            LCD.WriteLine("RIGHT RED");
            left_drive.SetPercent(15);
            right_drive.SetPercent(-30);
            Sleep(1.05);
            stopAll();

            Sleep (1.0);




        }
        else if ((cdsCell.Value() < .900) && (cdsCell.Value() > .660)) {
            route = 2;
            wait = false;
            LCD.WriteLine("BLUE Route");

            bumpStraight(30);
            Sleep(.5);

            LCD.WriteLine("RIGHT BLUE");
            left_drive.SetPercent(0);
            right_drive.SetPercent(-30);
            Sleep(1.8);
            stopAll();

            Sleep(1.0);


            driveStraight(30, 1.5);

            Sleep (1.0);

            driveStraight(-30, 1.0);

            Sleep(.5);

            LCD.WriteLine("RIGHT BLUE");
            left_drive.SetPercent(20);
            right_drive.SetPercent(-30);
            Sleep(1.9);
            stopAll();

        }
    }

     Sleep (3.0);

     //LCD.WriteLine("RIGHT RED");
     //left_drive.SetPercent(20);
     //right_drive.SetPercent(-30);
     //Sleep(1.5);

     bumpStraight(35);
     Sleep(.5);

     driveStraight(-35,1.0);

     Sleep(.5);

     rightRadius();

     Sleep(.5);

     left_drive.SetPercent(20);
     right_drive.SetPercent(-20);
     Sleep(.2);

     Sleep(.5);

     driveStraight(40,5.0);

     Sleep(15.0);



    // turn right to face buttons
    rightRadius();
    Sleep(5.0);

    // drive into buttons
    driveStraight(30, 1.5);
    Sleep(.5);

    // back up
    driveStraight(-25, .5);
    Sleep(.5);

    // turn left to face ramp wall
    LCD.WriteLine("LEFT TURN");
    left_drive.SetPercent(30);
    right_drive.SetPercent(10);
    Sleep(2.0);
    stopAll();

    Sleep(.5);

    driveStraight(-10,2);
    Sleep(.5);

    driveStraight(20,3);
    Sleep(.5);

   // drive backwards towards wrench
   driveStraight(-45,3);
   Sleep(.5);


   // pivot backwards from left wheel
   //LCD.Clear();
   LCD.WriteLine("RIGHT TURN");
   left_drive.SetPercent(0);
   right_drive.SetPercent(-30);
   Sleep(1.5);
   stopAll();

   driveStraight(20,3);
   Sleep(.5);

   // pivot backwards from left wheel
   //LCD.Clear();
   LCD.WriteLine("RIGHT TURN");
   left_drive.SetPercent(0);
   right_drive.SetPercent(-30);
   Sleep(2.5);
   stopAll();

   driveStraight(20,5);
   Sleep(.5);

   LCD.WriteLine("RIGHT TURN");
   left_drive.SetPercent(20);
   right_drive.SetPercent(15);
   Sleep(2.5);
   stopAll();

   driveStraight(15,3);
   Sleep(.5);


   driveStraight(-15,1);
   Sleep(.5);

   // REVERSE FROM LEVER

   LCD.WriteLine("RIGHT BACKWARDS");
   left_drive.SetPercent(-20);
   right_drive.SetPercent(-40);
   Sleep(2.0);
   stopAll();
   Sleep(.5);

   driveStraight(-20,2.5);
   Sleep(.5);


   LCD.WriteLine("LEFT BACKWARDS");
   left_drive.SetPercent(40);
   right_drive.SetPercent(10);
   Sleep(2.5);
   stopAll();
   Sleep(.5);

   driveStraight(-20,2.0);
   Sleep(.5);

   LCD.WriteLine("LEFT BACKWARDS");
   left_drive.SetPercent(-10);
   right_drive.SetPercent(-30);
   Sleep(2.0);
   stopAll();
   Sleep(.5);

   driveStraight(15,1.0);
   Sleep(3.0);

   driveStraight(-75,7.0);
   Sleep(.5);

   Sleep(10.0);
   left_drive.SetPercent(-10);
   right_drive.SetPercent(30);
   Sleep(2.0);
   stopAll();

   driveStraight(20,1.0);
   Sleep(.5);

   Sleep(3.0);

   driveStraight(-75,7.0);
   Sleep(.5);

   Sleep(10.0);
   left_drive.SetPercent(-10);
   right_drive.SetPercent(30);
   Sleep(2.0);
   stopAll();

   driveStraight(20,1.0);
   Sleep(.5);

   Sleep(3.0);

   driveStraight(-75,7.0);
   Sleep(.5);



   /*


    // drive into ramp
    driveStraight(-75,7.0);
    Sleep(.5);

    // drive into button panel
    driveStraight(30,4.0);
    Sleep(.5);

    // drive slightly forwards, away from button
    driveStraight(-25, .5);
    Sleep(.5);

    // turn right to face wrench
    rightNinety();
    Sleep(.5);

    // reverse into ramp panel
    bumpStraight(-15);
    Sleep(.5);

    // drive forward towards wrench
    driveStraight(25, 6.0);
    Sleep(.5);

    // turn left to face lever wall
    leftNinety();
    Sleep(.5);

    // drive into lever wall
    bumpStraight(35);
    Sleep(.5);
    // BUMP STRAIGHT

    // drive slightly backwards, away from wall
    driveStraight(-15, 2.0);
    Sleep(.5);

    // turn left to face lever
    leftNinety();
    Sleep(.5);

    // reverse into wrench wall
    bumpStraight(-15);
    Sleep(.5);

    // drive forward into lever
    driveStraight(25, 4.0);
    Sleep(.5);*/

    return 0;
}

bool bumperTest()
{
    // returns TRUE if all switches are NOT being pressed
    // or FALSE if one or more switches is pressed
    //return ((switch_FR)&&(switch_FL)&&(switch_BR)&&(switch_BL));

    if ((frontBump.Value() == 1)&&(backBump.Value() == 1)){
        return true;
    }
    else {
        return false;
    }

}

void turnOne ()
{
    // drive backwards from the wall
    left_drive.SetPercent(-5);
    right_drive.SetPercent(-5);
    Sleep(1.0);

    // for 10 seconds, pivot backwards from the B_L wheel
    left_drive.SetPercent(0);
    right_drive.SetPercent(-10);
    Sleep(10.25);
    stopAll();

}

void turnTwo ()
{
    // drive backwards from the wall
    left_drive.SetPercent(-12);
    right_drive.SetPercent(-12);
    Sleep(1.0);

    // for 10 seconds, pivot backwards from the B_L wheel
        left_drive.SetPercent(-20);
        right_drive.SetPercent(0);
        Sleep(2.5);
        stopAll();

}

void bumpStraight(int motor_percent)
{

    LCD.WriteLine("BUMP STRAIGHT");

    while (bumperTest()){
        // while not touching any bumpers
        // set both motors to the input motor percent
        left_drive.SetPercent(motor_percent);
        right_drive.SetPercent(motor_percent);
    }
    // once a bump switch is toggled, stop
    stopAll();
}

void driveStraight(int motor_percent, float time)
{

    //LCD.Clear();
    LCD.WriteLine("DRIVE STRAIGHT");

    left_drive.SetPercent(motor_percent);
    right_drive.SetPercent(motor_percent);

    Sleep(time);

    stopAll();
}




void leftRadius ()
{

    // pivot backwards from right wheel
    //LCD.Clear();
    LCD.WriteLine("LEFT TURN");
    left_drive.SetPercent(-30);
    right_drive.SetPercent(0);
    Sleep(2.7);
    stopAll();

}

void rightRadius ()
{

    // pivot backwards from left wheel
    //LCD.Clear();
    LCD.WriteLine("RIGHT TURN");
    left_drive.SetPercent(0);
    right_drive.SetPercent(-30);
    Sleep(2.7);
    stopAll();

}


void leftNinety ()
{

    // 0 radius turn to the left
    LCD.WriteLine("LEFT NINETY");
    left_drive.SetPercent(-30);
    right_drive.SetPercent(30);
    Sleep(1.40);
    stopAll();

}

void rightNinety ()
{

    // 0 radius turn to the right
    LCD.WriteLine("RIGHT NINETY");
    left_drive.SetPercent(30);
    right_drive.SetPercent(-30);
    Sleep(1.40);
    stopAll();

}




void stopAll()
{
    // stop both motors for both wheels
    left_drive.Stop();
    right_drive.Stop();
}
