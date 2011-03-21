set ( MAJOR 0 )
set ( MINOR 1 )

execute_process( COMMAND git rev-list --all
                 COMMAND wc -l
                 OUTPUT_VARIABLE REVISION
                 RESULT_VARIABLE RETURN_CODE
                 WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
 
 if( ${RETURN_CODE} )
   message( FATAL_ERROR "Failed to get revision number" )
 endif()                
 
 #remove \n from the end of REVISION
 string( REPLACE "\n" "" REVISION ${REVISION}  )
 
 message( ------------------------ )               
 message( "Version: " ${MAJOR} "." ${MINOR} "." ${REVISION} )
 message( ------------------------ )
 
 set( FILE_CONTENT
      "#if !defined( a2cdfde0_fcd8_4c96_8479_c8de6fa0fdc0) \n" 
      "# define a2cdfde0_fcd8_4c96_8479_c8de6fa0fdc0\n"
      "#include <string>\n"
      "struct version\n"
      "{\n"
      "  static std::string to_str()\n"
      "  {\n"
      "    return \"${MAJOR}.${MINOR}.${REVISION}\"\; \n"
      "  }\n\n"
      "  static unsigned get()\n"
      "  {\n"
      "    return (unsigned)${REVISION} + ((unsigned char)${MINOR} << 16) + ((unsigned char)${MAJOR} << 24)\;\n"
      "  }\n"
      "}\; //struct version\n"
      "#endif //# define a2cdfde0_fcd8_4c96_8479_c8de6fa0fdc0\n" )
      
 file(WRITE ${PROJECT_SOURCE_DIR}/version/version.hpp ${FILE_CONTENT} )
 
 math( EXPR VERSION_NUMBER "${REVISION} + (${MINOR} << 16) + (${MAJOR} << 24)" )
 file(WRITE ${PROJECT_SOURCE_DIR}/version/version ${VERSION_NUMBER} )  