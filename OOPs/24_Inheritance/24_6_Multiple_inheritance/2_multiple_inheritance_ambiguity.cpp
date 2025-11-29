#include <iostream>
#include <iomanip>

class USBDevice
{
    int m_ID{};
public:
    USBDevice(int id=0) : m_ID{id}
    {
        std::cout << "USBDevice Constructor\n";
    }

    int getID() const { return m_ID; }
};

class NetworkDevice
{
    int m_ID{};
public:
    NetworkDevice(int id=0) : m_ID{id}
    {
        std::cout << "NetworkDevice Constructor\n";
    }

    int getID() const { return m_ID; }
};

class WirelessAdapter : public USBDevice, public NetworkDevice
{
public:
    WirelessAdapter(int id1=0, int id2=0) : USBDevice{id1}, NetworkDevice{id2}
    {
        std::cout << "WirelessAdapter Constructor\n";
    }
};

int main()
{
    WirelessAdapter wd{1024, 2048};
    //std::cout << "USBDevice ID : " << wd.getID() << "\n";      // error: request for member ‘getID’ is ambiguous

    std::ios oldFlags(nullptr);
    oldFlags.copyfmt(std::cout);

    std::cout << std::left;
    std::cout << std::setw(20) << "USBDevice ID" << " : " << wd.USBDevice::getID() << "\n";      
    std::cout << std::setw(20) << "NetworkDevice ID" << " : " << wd.NetworkDevice::getID() << "\n";     

    std::cout.copyfmt(oldFlags);

    return 0;
}