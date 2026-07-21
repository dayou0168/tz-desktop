function(add_tz_contract_test target source header)
	add_executable(${target} EXCLUDE_FROM_ALL)
	init_target(${target} "(tests)")

	target_include_directories(${target} PRIVATE ${src_loc})

	nice_target_sources(${target} ${src_loc}
	PRIVATE
		${source}
		${header}
	)

	target_link_libraries(${target}
	PRIVATE
		desktop-app::external_qt
	)

	set_target_properties(${target} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
	)
endfunction()

add_tz_contract_test(
	tz_client_contract_tests
	tests/tz_client_contract_tests.cpp
	tz/tz_client_contract.h)
add_tz_contract_test(
	group_creation_contract_tests
	tests/group_creation_contract_tests.cpp
	boxes/group_creation_contract.h)
add_tz_contract_test(
	send_as_policy_contract_tests
	tests/send_as_policy_contract_tests.cpp
	main/session/send_as_policy.h)
add_tz_contract_test(
	tz_internal_url_contract_tests
	tests/tz_internal_url_contract_tests.cpp
	tz/tz_internal_url.h)

add_custom_target(tz_contract_tests)
add_dependencies(tz_contract_tests
	tz_client_contract_tests
	group_creation_contract_tests
	send_as_policy_contract_tests
	tz_internal_url_contract_tests
)
