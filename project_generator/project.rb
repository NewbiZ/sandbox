#!/usr/bin/env ruby

require 'FileUtils'
require 'optparse'
require 'pp'

options = {
  :verbose => false,
  :noop    => false,
  :project => ''
}
optparse = OptionParser.new do |opt|
  ### Parameters
  opt.program_name = "project"
  opt.version      = [ 1, 0, 0 ]
  opt.banner       = "Usage: #{opt.program_name} [options] project_name"
  opt.define_head 'Generate a CMake/C++ project tree'
  
  ### Common options
  opt.separator 'Common:'
  opt.on( '-p', '--project project_name', String, 'Project name' ) { |p| options[:project]=p }
  opt.on( '-n', '--[no-]noop', 'Don\'t create files' ) { |n| options[:noop]=n }
  
  ### Misc options
  opt.separator 'Misc:'
  opt.on( '-v', '--[no-]verbose', 'Display information about the status of the generation' ) { |v| options[:verbose]=v }
  opt.on_tail( '-h', '--help', 'Display this help screen' ) { puts opt; exit }
  opt.on_tail( '--version', 'Display the version of this script' ) { puts "This is #{opt.program_name} v#{opt.version.join('.')}"; exit }
  
  ### Parsing
  begin
    opt.parse!
  rescue OptionParser::InvalidOption => e
    puts e
    puts opt.banner
    puts 'Use --help to display list of available options'
    exit
  end
end

if options[:project].empty? then
  puts "You have to provide a project name."
  puts 'Use --help to display list of available options'
  exit
end

if File.exists? options[:project] then
  puts "Folder #{options[:project]} already exists. Overwrite? [y/N]"
  overwrite = gets.chomp == "y"
  unless overwrite
    puts 'Canceled by user.'
    exit
  end
end

def create_directory( path, noop, verbose )
  puts "Creating directory #{path}" if verbose
  FileUtils.mkdir_p path unless noop
end

def create_file( path, noop, verbose, text )
  puts "Creating file      #{path}" if verbose
  unless noop
    FileUtils.touch path
    File.open( path, 'w' ) { |f| f.write(text) }
  end
end

main_cmakelist = <<DOC
########################## Project configuration ##############################
SET( PROJ_NAME      "#{options[:project]}" )
SET( PROJ_PATH      ${CMAKE_SOURCE_DIR} )
SET( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )
SET( PROJ_DATA_PATH ${CMAKE_SOURCE_DIR}/resources )
SET( PROJ_DOC_PATH  ${PROJ_OUT_PATH}/doc )
SET( PROJ_SOURCES   "" )
SET( PROJ_HEADERS   "" )
SET( PROJ_LIBRARIES "" )
SET( PROJ_INCLUDES  "" )

####################### Minimum CMake version to run ##########################
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

############################## Project name ###################################
PROJECT( #{options[:project]} )

MESSAGE( "--------------------------------------------------" )
MESSAGE( STATUS "Configuring #{options[:project]}" )

SET( #{options[:project]}_MAJOR_VERSION 0 )
SET( #{options[:project]}_MINOR_VERSION 0 )
SET( #{options[:project]}_PATCH_LEVEL   1 )

########################## Modules configuration ##############################
OPTION( BUILD_DOC    "Build the doxygen documentation" ON )
OPTION( BUILD_TESTS  "Build the unit tests"            ON )

MESSAGE( STATUS "Building documentation:   " ${BUILD_DOC}    )
MESSAGE( STATUS "Building tests:           " ${BUILD_TESTS}  )

############################## Sources ########################################
FILE( GLOB_RECURSE PROJ_SOURCES src/*.cpp ) # Scan all source files
FILE( GLOB_RECURSE PROJ_HEADERS inc/#{options[:project]}/*.h ) # Scan all public  header files

SET( PROJ_INCLUDES ${PROJ_INCLUDES} ${PROJ_PATH}/inc )

# Group files in virtual folders under Visual Studio
SOURCE_GROUP( "Headers" FILES ${PROJ_SOURCES} )
SOURCE_GROUP( "Sources" FILES ${PROJ_HEADERS} )

############################ External libraries ###############################
# Add current source directory as module path for Find*.cmake
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJ_PATH} )

# Template for adding a library
#FIND_PACKAGE( <LibraryName> REQUIRED )
#SET( PROJ_INCLUDES  ${PROJ_INCLUDES}  ${<LibraryName>_INCLUDE_DIR} )
#SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${<LibraryName>_LIBRARIES}   )

################################ Includes #####################################
INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} ) # Include path

################################ Resources ####################################
FILE( COPY ${PROJ_DATA_PATH} DESTINATION ${PROJ_OUT_PATH} )

############################ Project generation ###############################
#ADD_LIBRARY( #{options[:project]} SHARED ${Sources} )
ADD_EXECUTABLE( #{options[:project]} ${PROJ_SOURCES} )

TARGET_LINK_LIBRARIES( #{options[:project]} ${PROJ_LIBRARIES} )

############################## Documentation ##################################
IF ( BUILD_DOC )
  FIND_PACKAGE( Doxygen )
  SET( DOXYFILE_IN  "${PROJ_PATH}/Doxyfile.in"  )
  SET( DOXYFILE_OUT "${PROJ_OUT_PATH}/Doxyfile" )
  SET_PROPERTY( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROJ_DOC_PATH} )
  ADD_CUSTOM_TARGET( doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                                 "Generating documentation..."
                                 WORKING_DIRECTORY ${PROJ_PATH} )
  CONFIGURE_FILE( ${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY )
ENDIF( BUILD_DOC )

################################## Tests ######################################
IF ( BUILD_TESTS )
  ENABLE_TESTING()
  INCLUDE( CTest )
  ADD_SUBDIRECTORY( tests )
ENDIF( BUILD_TESTS )

MESSAGE( "--------------------------------------------------" )

############################# Target Properties ###############################
SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES LINKER_LANGUAGE CXX ) # Language forced to C++

SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES DEBUG_POSTFIX   "d" ) # Debug mode: a trailing "d"
SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES RELEASE_POSTFIX ""  ) # Release mode: no postfix

SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJ_OUT_PATH} ) # Path for shared libraries
SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJ_OUT_PATH} ) # Path for module libraries
SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJ_OUT_PATH} ) # Path for import libraries

SET_TARGET_PROPERTIES( #{options[:project]} PROPERTIES OUTPUT_NAME "#{options[:project]}" ) # Output name

DOC

tests_cmakelist = <<DOC
### Test template
#ADD_EXECUTABLE( test1 test1.cpp )
#ADD_TEST( test1 test1 )
DOC

main_source = <<DOC
#include <iostream>
#include <cstdlib>

int main( int, char** )
{
  std::cout << "Hello world from #{options[:project]}!" << std::endl;
  
  return EXIT_SUCCESS;
}
DOC

doxyfile_in = <<DOC
DOXYFILE_ENCODING      = UTF-8
PROJECT_NAME           = "@PROJ_NAME@"
PROJECT_NUMBER         = "@#{options[:project]}_MAJOR_VERSION@.@#{options[:project]}_MINOR_VERSION@.@#{options[:project]}_PATCH_LEVEL@"
OUTPUT_DIRECTORY       = "@PROJ_DOC_PATH@"
CREATE_SUBDIRS         = NO
OUTPUT_LANGUAGE        = English
BRIEF_MEMBER_DESC      = YES
REPEAT_BRIEF           = YES
ALWAYS_DETAILED_SEC    = NO
INLINE_INHERITED_MEMB  = YES
FULL_PATH_NAMES        = NO
STRIP_FROM_PATH        = 
STRIP_FROM_INC_PATH    = 
SHORT_NAMES            = NO
MULTILINE_CPP_IS_BRIEF = NO
INHERIT_DOCS           = YES
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 4
ALIASES                = 
BUILTIN_STL_SUPPORT    = YES
DISTRIBUTE_GROUP_DOC   = NO
SUBGROUPING            = YES
TYPEDEF_HIDES_STRUCT   = NO

EXTRACT_ALL            = NO
EXTRACT_PRIVATE        = NO
EXTRACT_STATIC         = NO
EXTRACT_LOCAL_CLASSES  = YES
EXTRACT_LOCAL_METHODS  = NO
EXTRACT_ANON_NSPACES   = YES
HIDE_UNDOC_MEMBERS     = NO
HIDE_UNDOC_CLASSES     = NO
HIDE_FRIEND_COMPOUNDS  = NO
HIDE_IN_BODY_DOCS      = NO
INTERNAL_DOCS          = NO
CASE_SENSE_NAMES       = YES
HIDE_SCOPE_NAMES       = NO
SHOW_INCLUDE_FILES     = YES
INLINE_INFO            = YES
SORT_MEMBER_DOCS       = YES
SORT_BRIEF_DOCS        = NO
SORT_GROUP_NAMES       = NO
SORT_BY_SCOPE_NAME     = NO
GENERATE_TODOLIST      = YES
GENERATE_TESTLIST      = YES
GENERATE_BUGLIST       = YES
GENERATE_DEPRECATEDLIST= YES
ENABLED_SECTIONS       = 
MAX_INITIALIZER_LINES  = 30
SHOW_USED_FILES        = YES
SHOW_DIRECTORIES       = NO
SHOW_FILES             = YES
SHOW_NAMESPACES        = YES
FILE_VERSION_FILTER    = 

LAYOUT_FILE            = 
QUIET                  = YES
WARNINGS               = YES
WARN_IF_UNDOCUMENTED   = YES
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = NO
WARN_FORMAT            = "$file:$line: $text"
WARN_LOGFILE           = 

INPUT                  = "@PROJ_PATH@/src" "@PROJ_PATH@/inc"
INPUT_ENCODING         = UTF-8
FILE_PATTERNS          = 
RECURSIVE              = YES
EXCLUDE                = "_darcs"
EXCLUDE_SYMLINKS       = NO
EXCLUDE_PATTERNS       = "*/.*" "*/.*/*"
EXCLUDE_SYMBOLS        = 
EXAMPLE_PATH           = 
EXAMPLE_PATTERNS       = 
EXAMPLE_RECURSIVE      = NO
IMAGE_PATH             = 
INPUT_FILTER           = 
FILTER_PATTERNS        = 
FILTER_SOURCE_FILES    = NO

SOURCE_BROWSER         = NO
INLINE_SOURCES         = NO
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = NO
REFERENCES_RELATION    = NO
REFERENCES_LINK_SOURCE = YES
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES

ALPHABETICAL_INDEX     = NO
COLS_IN_ALPHA_INDEX    = 5
IGNORE_PREFIX          = 

GENERATE_HTML          = YES
HTML_OUTPUT            = "."
HTML_FILE_EXTENSION    = .html
HTML_HEADER            = 
HTML_FOOTER            = 
HTML_STYLESHEET        = 
HTML_ALIGN_MEMBERS     = YES
HTML_DYNAMIC_SECTIONS  = NO
GENERATE_DOCSET        = NO
DOCSET_FEEDNAME        = "Doxygen generated docs"
DOCSET_BUNDLE_ID       = org.doxygen.Project
GENERATE_HTMLHELP      = NO
CHM_FILE               = 
HHC_LOCATION           = 
GENERATE_CHI           = NO
CHM_INDEX_ENCODING     = 
BINARY_TOC             = NO
TOC_EXPAND             = NO
GENERATE_QHP           = NO
DISABLE_INDEX          = NO
ENUM_VALUES_PER_LINE   = 4
GENERATE_TREEVIEW      = NONE
TREEVIEW_WIDTH         = 250
FORMULA_FONTSIZE       = 10

ENABLE_PREPROCESSING   = YES
MACRO_EXPANSION        = NO
EXPAND_ONLY_PREDEF     = NO
SEARCH_INCLUDES        = YES
INCLUDE_PATH           = 
INCLUDE_FILE_PATTERNS  = 
PREDEFINED             = 
EXPAND_AS_DEFINED      = 
SKIP_FUNCTION_MACROS   = YES

TAGFILES               = 
GENERATE_TAGFILE       = 
ALLEXTERNALS           = NO
EXTERNAL_GROUPS        = YES
PERL_PATH              = /usr/bin/perl

CLASS_DIAGRAMS         = YES
MSCGEN_PATH            = 
HIDE_UNDOC_RELATIONS   = YES
HAVE_DOT               = YES
DOT_FONTNAME           = FreeSans
DOT_FONTSIZE           = 10
DOT_FONTPATH           = 
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = YES
GROUP_GRAPHS           = YES
UML_LOOK               = NO
TEMPLATE_RELATIONS     = NO
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES
CALL_GRAPH             = NO
CALLER_GRAPH           = NO
GRAPHICAL_HIERARCHY    = YES
DIRECTORY_GRAPH        = YES
DOT_IMAGE_FORMAT       = png
DOT_PATH               = "@DOXYGEN_DOT_PATH@"
DOTFILE_DIRS           = 
DOT_GRAPH_MAX_NODES    = 50
MAX_DOT_GRAPH_DEPTH    = 0
DOT_TRANSPARENT        = YES
DOT_MULTI_TARGETS      = NO
GENERATE_LEGEND        = YES
DOT_CLEANUP            = YES

SEARCHENGINE           = NO
DOC

documentation_source = <<DOC
/** \\mainpage #{options[:project]}
 *
 * \\section overview_sec Overview
 *
 * This is the overview.
 *
 * \\section install_sec Installation
 *
 * \\subsection step1 Step 1: This is the installation step 1
 *
 * etc...
 */
DOC

readme_source = <<DOC
### Overview
This is #{options[:project]}

### Authors
See AUTHORS

### Installation
Follow these steps to build the project:
    cd #{options[:project]}
    mkdir build
    cd build
    cmake ..
    make

Remember that you can enable the build of tests using the following:
    make .. -DBUILD_TESTS=ON

The tests can be executed using:
    make test

To enable the doxygen documentation, use the following:
    cmake .. -DBUILD_DOC=ON

Then you can generate the documentation using:
    make doc

### Help and more...
Visit [my website](http://www.coderbasement.com)
DOC

install_source = <<DOC
See README
DOC

authors_source = <<DOC
Aurelien Vallee - <vallee.aurelien@gmail.com>
DOC

create_directory "#{options[:project]}",                                                 options[:noop], options[:verbose]
create_file      "#{options[:project]}/CMakeLists.txt",                                  options[:noop], options[:verbose], main_cmakelist
create_file      "#{options[:project]}/Doxyfile.in",                                     options[:noop], options[:verbose], doxyfile_in
create_file      "#{options[:project]}/README.markdown",                                 options[:noop], options[:verbose], readme_source
create_file      "#{options[:project]}/AUTHORS.markdown",                                options[:noop], options[:verbose], authors_source
create_file      "#{options[:project]}/INSTALL.markdon",                                 options[:noop], options[:verbose], install_source
create_directory "#{options[:project]}/src",                                             options[:noop], options[:verbose]
create_file      "#{options[:project]}/src/main.cpp",                                    options[:noop], options[:verbose], main_source
create_directory "#{options[:project]}/inc",                                             options[:noop], options[:verbose]
create_directory "#{options[:project]}/tests",                                           options[:noop], options[:verbose]
create_directory "#{options[:project]}/resources",                                       options[:noop], options[:verbose]
create_file      "#{options[:project]}/tests/CMakeLists.txt",                            options[:noop], options[:verbose], tests_cmakelist
create_directory "#{options[:project]}/inc/#{options[:project]}",                        options[:noop], options[:verbose]
create_file      "#{options[:project]}/inc/#{options[:project]}/#{options[:project]}.h", options[:noop], options[:verbose], documentation_source
