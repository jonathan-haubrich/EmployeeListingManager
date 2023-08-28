
file(GLOB
	EmployeeListingSources
	"../EmployeeListingManager/Employee*.c"
	"../EmployeeListingManager/Network/*.c"
	"../EmployeeListingManager/Protocol/*.c"
)

add_executable(EmployeeListingTests
	"EmployeeListingTests.c"
	${EmployeeListingSources}
)
add_test(EmployeeListingTests EmployeeListingTests)

add_executable(EmployeeListingCollectionTests
	"EmployeeListingCollectionTests.c"
	${EmployeeListingSources}
)
add_test(EmployeeListingCollectionTests EmployeeListingCollectionTests)
