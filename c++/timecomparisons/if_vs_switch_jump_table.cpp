#include <iostream>
int testSwitch(int value)
{
    int result = 0;
    switch (value)
    {
    case 1:
        result = 10;
        break;
    case 2:
        result = 20;
        break;
    case 3:
        result = 30;
        break;
    case 4:
        result = 40;
        break;

    case 6:
        result = 60;
        break;
    case 7:
        result = 70;
        break;
    case 8:
        result = 80;
        break;
    case 9:
        result = 90;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

void testIf(volatile int value)
{
    volatile int result = 0;
    if (value == 1)
    {
        result = 10;
    }
    else if (value == 2)
    {
        result = 20;
    }
    else if (value == 3)
    {
        result = 30;
    }
    else
    {
        result = 0;
    }
    std::cout << "If Result: " << result << std::endl;
}

int main()
{
    volatile int value = 2;
    testSwitch(value);
    testIf(value);
    return 0;
}
