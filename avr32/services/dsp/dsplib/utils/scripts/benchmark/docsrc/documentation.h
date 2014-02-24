/**
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*!
 * \defgroup group_dsplib_benchmark Benchmark Helper Script
 * \ingroup group_dsplib_tools
 * \brief This script is used to workbench a program, without any intervention. Once it is well configured,
 * it will automatically compile the program to evaluate, download it into the target, retrieves the data
 * and computes them to extract the error information. It will store into an output file the cycle count
 * this process took, the duration in seconds, the error average and the maximal error of the results.
 *
 * To make this script works you will need a serial cable that you have to connect to the port COM1 of you PC.
 * The other end of this cable has to be connected on the USART of the target.\n
 * To configure this script, edit the file “benchmark” and modify the values of the variables
 * defines in the section “BENCH CONFIG”.
 *
 * You will need \ref group_dsplib_dataget and group_dsplib_dataextract plus various standard UNIX tools.
 *
 * \section configuration Configuration
 *
 * \subsection config_const Constants
 * This is the definition of the constants used in the header file generated by this script in
 * order to configure the C program to workbench.
 * - \b param1_def, \b param2_def, \b param3_def, \b param4_def: These variables contain the names of the constants.
 * for example: 'VECT2_SIZE'
 * - \b param1, \b param2, \b param3, \b param4: These variables are defined by a list containing the values of the actual
 * parameters (param1_def, param2_def, param3_def and param4_def).
 * The values of the list are separated by the character ‘#’. If the value has to
 * be a blank, use the special string ‘SPACE’ to specify this case.
 * \warning All the parameters have to be used. For example: '32#64#128#256'
 *
 * \subsection config_program Program
 * This is the definition of the parameters that permit computations according to the actual
 * configuration of the C program.
 * - \b FREQ: This variable defines the main clock frequency of the target. It is used in
 * the time calculation that took the program in its computation. This variable
 * can be dependant of the parameters. For example: i.e.: If the clock frequency of the target depends on the parameter number
 * 4, you can define it as follow: FREQ='$cur_param4*1000000'.
 * - \b QA, \b QB: This is the Q formatted parameters that define the  actual format to use.
 * The resulting format will be a “Q QA . QB” format. For example: QA='8' and QB='24' that define a 32-bit Q8.24 format number.
 * - \b OUTPUT_DATA_TYPE: The output data type of the resulting vector that returns the target. Its value
 * must be ‘real’ or ‘imag’. For example: 'real'
 *
 * \subsection config_path Path
 * Important path used in the script.
 * - \b CUR_PATH: Specify the directory where is located the Makefile you are using to
 * compile the program (from the script path). For example: '/cygdrive/d/trunk/SERVICES/DSPLIB/EXAMPLES/BENCH/AT32UC3A/GCC'
 * - \b CONFIG_FILE_PATH: Specify the path of the output header file used to parameter the C program
 * to workbench (from the $CUR_PATH directory). For example: '../../bench_config.h'
 * - \b SCRIPT_ERROR_FILE_PATH: Specify the path of the Scilab script used to generate the reference output
 * signal of the C program in order to calculate the error (from the $CUR_PATH directory).
 * For example: "`pwd`/RESSOURCES/ref_conv.sce"
 *
 * \subsection config_progloc Program's location
 * Specify the programs’ path from the $CUR_PATH directory.
 * - \b DATAGET: Specify the path of the DataGet application.
 * For example: "`pwd`/../../SOURCES/WINDOWS/DATA_GET/DataGet.exe"
 * - \b DATAEXTRACT: Specify the path of the DataExtract application.
 * For example:"`pwd`/../../SOURCES/WINDOWS/DATA_EXTRACT/DataExtract.exe"
 * - \b SCILAB_PATH: Specify the path of the Scilab directory.
 * For example: '/cygdrive/c/Program Files/scilab-4.1'
 *
 * \section usage Usage
 * \code ./benchmark \endcode
 *
 * \section pattern C Program File Pattern
 * In order to make the script compatible with the C program, you should use this C
 * program pattern to design your benchmark. This pattern works perfectly with the
 * EVK1100 board.
 * \code #include <avr32/io.h>
	 #include "compiler.h"
	 #include "board.h"

	 #include "dsp.h"
	 #include "pm.h"
	 #include "count.h"

	 #include "bench_config.h"

	 //! The main function
	 int main(int argc, char *argv[])
	 {
	   unsigned int cycle_count;

	   // Switch to external Oscillator 0.
	   pm_switch_to_osc0(&AVR32_PM, FOSC0, OSC0_STARTUP);

	   // Initialize the DSP debug module
	   dsp_debug_initialization(FOSC0);

	   // Get the actual cycle count
	   cycle_count = Get_sys_count();

	   // <<< Type here the code you need to workbench >>>

	   // Calculate the number of cycles
	   cycle_count = Get_sys_count() - cycle_count;

	   // Print the number of cycles
	   dsp16_debug_printf("Cycle count: %d\n", cycle_count);

	   // Print the output
	   // <<< use the functions dsp16_debug_print_vect, dsp32_debug_print_vect, dsp16_debug_print_complex_vect or
	   dsp32_debug_print_complex_vect >>>

	   while(1);
	 } \endcode
 *
 * In this program you will have access to the constants you defined in the script file as
 * parameters. Those constants are accessible by including the file “bench_config.h”.
 * Finally the program has to be compiled defining DSP_BENCH=1 and DSP_DEBUG=1.
 *
 * \section scilab_comp The Scilab data comparison script
 * This is the file that contains the reference signal which should be the result of the output
 * of the program to workbench. It serves to compare this reference signal with the actual
 * output and permits to retrieve the error average and the maximal error of this output
 * signal.\n
 * In this file you will just have to generate the signal you should receive with you C
 * program. To do that you will have access to the constants you defined in the script file as
 * parameters. The output signal must be store in the  “y” variable and can be a real or a
 * imaginary vector.\n
 * Here is an example for the FFT:\n
 * \code y_size = 2^NLOG;
	 t = 0:(y_size-1);
	 vect2_input = sin(2*%pi*4000*t/40000);

	 y = fft(vect2_input)/y_size; \endcode
 *
 * \section output The output
 * The resulting file of this benchmark is a text file containing all the workbench results.
 * Each result is printed on 4 lines: the first line corresponds to the cycle count of the
 * process, the next one is the duration of this process in second, the third is the error
 * average of the result and the last, its maximal error. Then you can easily manage such a
 * file using a spreadsheet.
 *
 * \section troubleshooting Troubleshooting
 * \subsection t1 The HyperTerminal is opened and so the application DataGet cannot open the COM port.
 * Close the HyperTerminal and rerun the script.
 * \subsection t2 An error is generated by the Scilab data comparison script.
 * Check the error on the file “dsp_bench_temp” else try to launch this script
 * manually with the required files.
 * \subsection t3 An error occurred while compiling the program.
 * This might be due to an error on your C files. Compile it separately to
 * check the error or look at the file ./dsp_bench_log.txt.
 *
 * \section information Information
 * All the files related to this module are located under /avr32/services/dsp/dsplib/utils/scripts/benchmark\n
 */
