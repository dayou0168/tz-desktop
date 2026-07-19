add_executable(tz_client_contract_tests EXCLUDE_FROM_ALL)
init_target(tz_client_contract_tests "(tests)")

target_include_directories(tz_client_contract_tests PRIVATE ${src_loc})

nice_target_sources(tz_client_contract_tests ${src_loc}
PRIVATE
	tests/tz_client_contract_tests.cpp
	tz/tz_client_contract.h
)

target_link_libraries(tz_client_contract_tests
PRIVATE
	desktop-app::external_qt
)

set_target_properties(
	tz_client_contract_tests
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
