###############################################################
function(
            ## Function name 
            add_test_suite 
            ## Argument list
            name           # Test name
            source_list    # List of sources
        )

    add_executable ( ${name} ${source_list} )

    set_target_properties ( ${name} PROPERTIES COMPILE_FLAGS "-std=c++0x -Wall -Werror -g" )

    target_link_libraries ( ${name} ${PROJECT_NAME}lib boost_unit_test_framework )

    add_test ( ${name} ${name} )

endfunction(add_test_suite)
###############################################################



