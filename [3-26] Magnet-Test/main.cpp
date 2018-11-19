#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>


//Front bump switch
DigitalOutputPin magnet(FEHIO::P0_0);
//Back bump switch
DigitalInputPin backBump(FEHIO::P3_0);

DigitalInputPin frontBump(FEHIO::P3_2);

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


    Sleep(3.0);

    LCD.Clear();
    LCD.SetBackgroundColor(BLACK);

    LCD.WriteLine("Mask OFF");

    driveStraight(20,5);

    magnet.Write(true);

    LCD.WriteLine("Mask ON (8)");ZAS

    Sleep(8.0);

    driveStraight(-30,5);

    Sleep(10.0);

    magnet.Write(false);

    LCD.WriteLine("Mask OFF");

    Sleep(10.0);
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
