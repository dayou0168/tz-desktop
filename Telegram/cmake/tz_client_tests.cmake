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

add_executable(mtproto_dc_options_contract_tests EXCLUDE_FROM_ALL)
init_target(mtproto_dc_options_contract_tests "(tests)")

target_precompile_headers(mtproto_dc_options_contract_tests
PRIVATE
	${src_loc}/mtproto/mtproto_pch.h
)

nice_target_sources(mtproto_dc_options_contract_tests ${src_loc}
PRIVATE
	tests/mtproto_dc_options_contract_tests.cpp
	mtproto/details/mtproto_dump_to_text.cpp
	mtproto/details/mtproto_dump_to_text.h
	mtproto/details/mtproto_rsa_public_key.cpp
	mtproto/details/mtproto_rsa_public_key.h
	mtproto/mtproto_dc_options.cpp
	mtproto/mtproto_dc_options.h
)

target_link_libraries(mtproto_dc_options_contract_tests
PRIVATE
	tdesktop::td_scheme
	desktop-app::external_zlib
	desktop-app::external_openssl
	desktop-app::external_qt
	desktop-app::lib_base
	desktop-app::lib_tl
)

set_target_properties(mtproto_dc_options_contract_tests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(tz_contract_tests)
add_dependencies(tz_contract_tests
	tz_client_contract_tests
	group_creation_contract_tests
	mtproto_dc_options_contract_tests
	send_as_policy_contract_tests
	tz_internal_url_contract_tests
)
