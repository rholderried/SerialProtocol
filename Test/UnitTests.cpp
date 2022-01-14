#include <iostream>
#include <cstdint>
#include <cstdbool>
#include "SerialProtocol.h"
#include "Helpers.h"

using namespace std;

//float testVar = 2.32;

void testFtoA (void)
{
    uint8_t retVal[40] = {0};
    uint8_t size = 0;
    float testVal = 5.131666;

    for (int i = 0; i < 20; i++ )
    {
        size = ftoa(retVal, testVal - static_cast<float>(i)/2, 5, true);

        retVal[size] = '\0';

        cout << retVal << endl;
    }

    testVal = 5;

    for (int i = 0; i < 10; i++ )
    {
        size = ftoa(retVal, testVal - static_cast<float>(i), 0, true);

        retVal[size] = '\0';

        cout << retVal << endl;
    }

}

// void testProtocol (void)
// {

// }

bool testTxCb(uint8_t* p_buf, uint8_t size)
{
    p_buf[size] = '\0';
    cout << p_buf << endl;

    return true;
}

int main (void)
{
    float testvar = -0.567;
    SerialProtocol prot = SerialProtocol();
    prot.setupCallbacks(testTxCb, nullptr, nullptr);

    prot.receive(2);
    prot.receive('7');
    prot.receive('?');
    prot.receive(3);

    prot.statemachine();
    prot.statemachine();
    prot.statemachine();

    prot.receive(2);
    prot.receive('7');
    prot.receive('!');
    prot.receive('1');
    prot.receive('.');
    prot.receive('5');
    prot.receive(3);


    prot.statemachine();
    prot.statemachine();
    prot.statemachine();

    prot.receive(2);
    prot.receive('7');
    prot.receive('?');
    prot.receive(3);

    prot.statemachine();
    prot.statemachine();
    prot.statemachine();


    //testFtoA();



    return 0;
}