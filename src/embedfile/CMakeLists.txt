add_executable(embedfile embedfile.cpp)

# you can embed executables like that:
#
#add_custom_command(
#    OUTPUT ${MAINFOLDER}/build/generated_src/foo.cpp
#    COMMAND ${CMAKE_COMMAND} -E make_directory ${MAINFOLDER}/build/generated_src
#    COMMAND embedfile foo ${MAINFOLDER}/CMakeLists.txt ${MAINFOLDER}/build/generated_src/foo.cpp
#    DEPENDS ${MAINFOLDER}/CMakeLists.txt
#    COMMENT "Create embedded resource file."
#    VERBATIM
#    )
# OUTPUT -- generated file
# second line creates the directory, third line uses embedfile to generate C++ source
# DEPENDS specifies used file
#
# To use generated file, just add source to sources list and then add dependency:
#add_dependencies(target ${MAINFOLDER}/build/generated_src/foo.cpp)
