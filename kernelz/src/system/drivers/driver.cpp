#include <system/drivers/driver.h>
#include <system/log.h>

using namespace HeisenOs;
using namespace HeisenOs::system;
using namespace HeisenOs::system::drivers;

Driver::Driver(char* name, char* description)
{
    this->Name = name;
    this->Description = description;
}
                
char* Driver::GetDriverName() {
    return this->Name; 
}
char* Driver::GetDriverDescription() {
    return this->Description;
}

bool Driver::Initialize()
{
    Log(Error, "Virtual function called directly %s:%d", __FILE__, __LINE__);
    return false;
}