# CMakeList.txt : CMake project for EmployeeListingManager, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (EmployeeListingManager
	"main.c"
	"EmployeeListingManager.c" "EmployeeListingManager.h"
	"EmployeeListing.c" "EmployeeListing.h"
	"EmployeeListingFormatters.c" "EmployeeListingFormatters.h"
	"EmployeeListingCollection.c" "EmployeeListingCollection.h"
	"Protocol/Requests.c" "Protocol/Requests.h"
	"Protocol/Responses.c" "Protocol/Responses.h"
	"Network/Receive.h" "Network/Receive.c"
	"Network/Send.h" "Network/Send.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EmployeeListingManager PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
