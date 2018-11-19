#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

AnalogInputPin leftSensor(FEHIO::P1_2);
AnalogInputPin middleSensor(FEHIO::P1_1);
AnalogInputPin rightSensor(FEHIO::P1_0);



FEHMotor leftMotor(FEHMotor::Motor1, 9.0);
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);

void stopAll();
void writeScreen(bool left, bool midle, bool right);

bool lineTest(float value, float low, float high);
int returnState (bool left, bool middle, bool right);

#define leftHigh 1.25
#define leftLow .7
#define middleHigh 1.6
#define middleLow 1.1
#define rightHigh 1.8
#define rightLow 1.5



int main(void)
{


    Sleep (3.0);

    while(true)
    {
        Sleep(.25);

        bool lStatus = lineTest(leftSensor.Value(), leftLow, leftHigh);
        bool mStatus = lineTest(middleSensor.Value(), middleLow, middleHigh);
        bool rStatus = lineTest(rightSensor.Value(), rightLow, rightHigh);

        LCD.Clear();
        LCD.WriteAt("Left: ",10,40);
        LCD.WriteAt(leftSensor.Value(),100,40);
        LCD.WriteAt(lStatus,10,60);

        LCD.WriteAt("Middle: ",10,80);
        LCD.WriteAt(middleSensor.Value(),100,80);
        LCD.WriteAt(mStatus,10,100);

        LCD.WriteAt("Right: ",10,120);
        LCD.WriteAt(rightSensor.Value(),100,120);
        LCD.WriteAt(rStatus,10,140);


        //writeScreen(lStatus, mStatus, rStatus);

        int state = returnState(lStatus, mStatus, rStatus);

        switch(state)
        {
            case 0: // ALL FALSE, then stop
                stopAll();
                break;
            case 1: // FTF
                rightMotor.SetPercent(15);
                leftMotor.SetPercent(15);
                break;
            case 2: // TTF
            case 3: // TFF
                leftMotor.SetPercent(0);
                rightMotor.SetPercent(15);
                while (lStatus)
                {
                    bool lStatus = lineTest(leftSensor.Value(), leftLow, leftHigh);
                    bool mStatus = lineTest(middleSensor.Value(), middleLow, middleHigh);
                    bool rStatus = lineTest(rightSensor.Value(), rightLow, rightHigh);
                    writeScreen(lStatus, mStatus, rStatus);
                }
                stopAll();
                Sleep(.25);
                break;
            case 4: // FTT
            case 5: // FFT
                leftMotor.SetPercent(15);
                rightMotor.SetPercent(0);
                while (rStatus)
                {
                    bool lStatus = lineTest(leftSensor.Value(), leftLow, leftHigh);
                    bool mStatus = lineTest(middleSensor.Value(), middleLow, middleHigh);
                    bool rStatus = lineTest(rightSensor.Value(), rightLow, rightHigh);
                    writeScreen(lStatus, mStatus, rStatus);
                }
                stopAll();
                Sleep(.25);
                break;
        }
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


int returnState(bool left, bool middle, bool right)
{
    if ((!left) && (!middle) && (!right))
        // (Left)      (Middle)      (Right)
        //  FALSE     FALSE         FALSE
    {
        return 1; // stop state
    }

    if ((!left) && (middle) && (!right))
        // (Left)      (Middle)      (Right)
        //  FALSE        TRUE         FALSE
    {
        return 1; // drive state
    }

    if ((left) && (middle) && (!right))
        // (Left)      (Middle)      (Right)
        //  TRUE        TRUE         FALSE
    {
        return 2; // left turn
    }

    if ((left) && (!middle) && (!right))
        // (Left)      (Middle)      (Right)
        //  TRUE        FALSE         FALSE
    {
        return 3; // left turn
    }

    if ((!left) && (middle) && (right))
        // (Left)      (Middle)     (Right)
        //  FALSE        TRUE        TRUE
    {
        return 4; // right turn
    }

    if ((!left) && (!middle) && (right))
        // (Left)      (Middle)     (Right)
        //  FALSE        FALSE        TRUE
    {
        return 5; // right turn
    }

    // NOT USED
    if ((left) && (!middle) && (right))
        // (Left)      (Middle)     (Right)
        //  TRUE        FALSE        TRUE
    {
        return 1; // rotate until middle is found
    }

}

void stopAll()
{
    // stop both wheels
    leftMotor.Stop();
    rightMotor.Stop();
}

void writeScreen(bool left, bool middle, bool right)
{
    LCD.Clear();
    LCD.WriteAt("Left: ",10,40);
    LCD.WriteAt(leftSensor.Value(),100,40);
    LCD.WriteAt(left,10,60);

    LCD.WriteAt("Middle: ",10,80);
    LCD.WriteAt(middleSensor.Value(),100,80);
    LCD.WriteAt(middle,10,100);

    LCD.WriteAt("Right: ",10,120);
    LCD.WriteAt(rightSensor.Value(),100,120);
    LCD.WriteAt(right,10,140);
}
