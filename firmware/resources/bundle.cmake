function(image_convert NAME)
	set(options)
	set(oneValueArgs)
	set(multiValueArgs SOURCES)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	add_custom_target(image_convert_${NAME}_all COMMENT "Build all images for ${NAME}")
	foreach(SOURCE IN ITEMS ${ARG_SOURCES})
		get_filename_component(SOURCE_NAME "${SOURCE}" NAME_WE)
		add_custom_target(image_convert_${NAME}_${SOURCE_NAME}
			COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/imagetoimg.py -q -o "${NAME}/${SOURCE_NAME}.img" "${SOURCE}"

			DEPENDS "${SOURCE}"
			SOURCES "${SOURCE}"
			COMMENT "Building ${SOURCE}"
			BYPRODUCTS "${NAME}//${SOURCE_NAME}.img"
		)

		add_dependencies(image_convert_${NAME}_all image_convert_${NAME}_${SOURCE_NAME})
	endforeach()

endfunction()

function(firmware_bundle NAME)
	set(options)
	set(oneValueArgs STRING_FILE OUTPUT_NAME)
	set(multiValueArgs SOURCES)
	cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

	add_custom_target(firmware_bundle_${NAME} COMMENT "Create ${NAME} bundle"
		COMMAND python2 ./bundle.py -s "${ARG_STRING_FILE}" -o ${ARG_OUTPUT_NAME} ${ARG_SOURCES}
		SOURCES ${ARG_STRING_FILE}
		WORKING_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}"
		DEPENDS ${ARG_STRING_FILE}
		BYPRODUCTS ${ARG_OUTPUT_NAME}
	)
endfunction()
