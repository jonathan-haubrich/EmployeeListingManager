# CMakeList.txt : CMake project for EmployeeListingManager, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (EmployeeListingManager
	"EmployeeListingManager.c" "EmployeeListingManager.h"
	"EmployeeListing.c" "EmployeeListing.h"
	"EmployeeListingFormatters.c" "EmployeeListingFormatters.h"
 "main.c" "EmployeeListingCollection.c" "EmployeeListingCollection.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET EmployeeListingManager PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
