#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

AnalogInputPin leftSensor(FEHIO::P1_2);
AnalogInputPin middleSensor(FEHIO::P1_1);
AnalogInputPin rightSensor(FEHIO::P1_0);



FEHMotor leftMotor(FEHMotor::Motor0, 9.0);
FEHMotor rightMotor(FEHMotor::Motor1, 9.0);

void hardRight();
void hardLeft();
void slightRight();
void slightLeft();
void straight();
void stopAll();


bool lineTest(float value, float low, float high);

#define leftHigh 2.4
#define leftLow 2.2
#define middleHigh 2.6
#define middleLow 2.2
#define rightHigh 2.8
#define rightLow 2.5

//Straight Line
//#define leftHigh 1.25
//#define leftLow .7
//#define middleHigh 1.6
//#define middleLow 1.1
//#define rightHigh 1.8
//#define rightLow 1.5


int main(void)
{


    Sleep (3.0);

    //bool leftStatus = false, middleStatus = false, rightStatus = false;

//#define int leftSensor = 0;
//#define int middleSensor = 1;
//#define int rightSensor = 2;

    while(true)
    {

        bool lStatus = lineTest(leftSensor.Value(), leftLow, leftHigh);
        bool mStatus = lineTest(middleSensor.Value(), middleLow, middleHigh);
        bool rStatus = lineTest(rightSensor.Value(), rightLow, rightHigh);

        LCD.WriteAt("Left: ",10,40);
        LCD.WriteAt(leftSensor.Value(),100,40);
        LCD.WriteAt(lStatus,10,60);

        LCD.WriteAt("Middle: ",10,80);
        LCD.WriteAt(middleSensor.Value(),100,80);
        LCD.WriteAt(mStatus,10,100);

        LCD.WriteAt("Right: ",10,120);
        LCD.WriteAt(rightSensor.Value(),100,120);
        LCD.WriteAt(rStatus,10,140);

        /*if (!lStatus)
        {
            if (mStatus)
            {

                if (rStatus)
                {
                    slightRight();
                }
                else
                {
                    straight();
                }

            }else{
                hardLeft();
            }

        }else {
            if (mStatus){
                slightLeft();
            }
            else{
                hardRight();
            }



        }*/

        if ((!lStatus) && (mStatus) && (rStatus))
        {
            slightRight();

        }
        else if ((!lStatus)&&(!mStatus)&&(rStatus))
        {
            hardRight();

        }else if(((!lStatus)&&(mStatus)&&(!rStatus)) || ((lStatus)&&(!mStatus)&&(rStatus)))
        {
            straight();
        }
        else if((lStatus)&&(!mStatus)&&(!rStatus))
        {
            hardLeft();
        }
        else if((lStatus) && (mStatus)&& (!rStatus))
        {
        slightLeft();
        }
        else if((!lStatus)&&(!mStatus)&&(!rStatus))
        {
            straight()                             ;
        }
    else
    {
        leftMotor.Stop();
        rightMotor.Stop();
    }

        LCD.Clear();
    }
    return 0;
}


bool lineTest(float value, float low, float high)
{
    if ((value < high) && (value > low))
    {
        return true;
    }
    else{
        return false;
    }
}

void hardLeft()
{
    leftMotor.Stop();
    Sleep(.5);
    leftMotor.SetPercent(15);

}

void hardRight()
{
    rightMotor.Stop();
    Sleep(.5);
    rightMotor.SetPercent(15);

}

void slightLeft()
{
    leftMotor.Stop();
    Sleep(.25);
    leftMotor.SetPercent(15);

}

void slightRight()
{
    rightMotor.Stop();
    Sleep(.25);
    rightMotor.SetPercent(15);

}

void straight()
{
    rightMotor.SetPercent(15);
    leftMotor.SetPercent(15);
    Sleep(.25);
}

void stopAll()
{
    // stop both motors for both wheels
    leftMotor.Stop();
    rightMotor.Stop();
}


