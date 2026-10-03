add_executable(DeferApp Main.cpp)
target_link_libraries(DeferApp PRIVATE sb::logger)

if(CMAKE_OBJCOPY)
    add_custom_command(
        TARGET DeferApp POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --remove-section=.sb_defmt $<TARGET_FILE:DeferApp> $<TARGET_FILE_DIR:DeferApp>/DeferApp_stripped
        COMMENT "Generating stripped binary (DeferApp_stripped) without .sb_defmt section"
    )
endif()
