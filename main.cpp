#include <iostream>
#include <cstdlib> 

#include "server.h"
#include "debug.h"

using std::cout;
using std::endl;

using std::uint16_t;
using std::atoi;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "usage: " << argv[0] << " <port>" << endl;

        return 1;
    }
    
    cout << endl;
    cout << " ***             ******          *** ***           ***   " << endl;
    cout << "  *               *    *          ** **           *   *  " << endl;
    cout << "  *               *    *          ** **          *     * " << endl;
    cout << "  *               *    *          ** **          *     * " << endl;
    cout << "  *               *    *          ** **          *     * " << endl;
    cout << "  *               *****           ** **          *     * " << endl;
    cout << "  *               *               * * *          *     * " << endl;
    cout << "  *               *               * * *          *     * " << endl;
    cout << "  *               *               * * *          * **  * " << endl;
    cout << "  *    *          *               * * *           *  **  " << endl;
    cout << " *******         ***             ** * **           ***   " << endl;
    cout << "                                                      ** " << endl;
    cout << endl;
                                                         
    uint16_t port = atoi(argv[1]);
    if(port < 1024)
    {
        LOG_ERR("Ports 0~1023 are system ports,please choice another ports.");
        
        return 1;
    }

    LOG_INFO("Starting lpmqd with port " << port);

    Server lpmqd(port);

    if(!lpmqd.exec())
    {
        LOG_INFO("lpmqd exit!");
    }

    return 0;
}