function(image_convert TARGET FILE INPUT OUTPUT)

	#message(STATUS "python ${CMAKE_CURRENT_SOURCE_DIR}/imagetoimg.py -q -o ${OUTPUT} ${INPUT}")
	add_custom_command(OUTPUT ${OUTPUT} PRE_BUILD
		COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/imagetoimg.py -q -o "${OUTPUT}" "${INPUT}"
		COMMENT "Build"
		DEPENDS "${INPUT}"
		SOURCES "${INPUT}"
		COMMENT "Building ${INPUT}"
		BYPRODUCTS "${OUTPUT}"
	)

endfunction()

file(GLOB OUTPUT LIST_DIRECTORIES true *)

foreach(DIRECTORY IN ITEMS ${OUTPUT})

	if(IS_DIRECTORY ${DIRECTORY})
		#message(STATUS "==${DIRECTORY}")
		file(GLOB OUTPUT LIST_DIRECTORIES false ${DIRECTORY}/*.png)
		get_filename_component(AB ${DIRECTORY} NAME)

		foreach(FILE IN ITEMS ${OUTPUT})
			get_filename_component(AA ${FILE} NAME_WE)
			#message(STATUS "=+${FILE} -- ${AB} -- ${AA}")
			image_convert(images ${AA} ${FILE} ${AB}/${AA}.img)
		endforeach()
	endif()
endforeach()
