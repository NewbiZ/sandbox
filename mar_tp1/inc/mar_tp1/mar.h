/**
 * \file
 * \brief Main header. Mainly contains documentation.
 */

/** \mainpage MAR TP1
 *
 * \section overview_sec Overview
 *
 * This is the first practical work of MAR.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Create the build directory
 * \code
 *     cd mar_tp1
 *     mkdir build
 *     cd build
 * \endcode
 *
 * \subsection step2 Step 2: Generate an appropriate project file
 * \code
 *     cmake ..
 * \endcode
 * 
 * To enable the doxygen documentation, use the following:
 * \code
 *     cmake .. -DBUILD_DOC=ON
 * \endcode
 * 
 * \subsection step3 Step 3: Build the project
 * If you chose to generate an IDE, then just use it. Otherwise for makefiles,
 * just use:
 * \code
 *     make
 * \endcode
 *
 * Then you can generate the documentation using:
 * \code
 *     make doc
 * \endcode
 */

/**
 * \namespace mar
 * \brief Contains all symbols specific to MAR.
 */
namespace mar {}