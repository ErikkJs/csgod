#include <iostream>

void testSwitch(volatile int value)
{
    volatile int result = 0;
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
    default:
        result = 0;
        break;
    }
    std::cout << "Switch Result: " << result << std::endl;
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
