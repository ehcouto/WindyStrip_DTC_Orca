/**
 *  @file       BoardDebugging_prv.h
 *
 *  @brief      Configuration of the board debugging module
 *
 *  @details
 *
 *  @section
 *
 */


#ifndef BOARDDEBUGGING_PRV_H_
#define BOARDDEBUGGING_PRV_H_

#include "C_Extensions.h"
#include "Mci_prm.h"
#include "MciParametersLoader_prv.h"

#define BD_ADC_NOISE_ASSESSMENT  DISABLED      // enabling\disabling the ADC noise assessment algorithm
#if (BD_ADC_NOISE_ASSESSMENT == ENABLED)

    #define BD_CURRENT(current)     ((current)/BASE_CURRENT * 32768.0f)
    #define BD_VOLTAGE(voltage)     ((voltage)/BASE_VOLTAGE_BUS * 32768.0f)

    #define BD_CURRENT_NOISE_THR    0
    #define BD_DC_BUS_NOISE_THR     0
    #define BD_NOISE_WINDOW         600       // @25ms rate
#endif


#if (COMMAND_MODE == MCI_BD)
    #define BD_MCI_CMD              ENABLED       // enabling\disabling mci commands from this module
#else
    #define BD_MCI_CMD              DISABLED      // enabling\disabling mci commands from this module
#endif


#if (BD_MCI_CMD == ENABLED)
    #define BD_PWM_ACCESS           DISABLED     // enabling\disabling pwm manual generation from this module

#if MCL_MOTOR == ORCA_H24
	#define BD_TR                15.0f
	#define BD_MAX_SPIN_SPEED    1200
#elif MCL_MOTOR == ORCA_H34
	#define BD_TR                13.09f
	#define BD_MAX_SPIN_SPEED    1400
#elif MCL_MOTOR == ATLAS_H32
	#define BD_TR                11.64f
	#define BD_MAX_SPIN_SPEED    1600
#endif

    #define BD_MOTOR_INIT           MOTOR0

    #define BD_SPEED_INIT           (sint32)(50.0f * BD_TR) //T.R.*50
    #define BD_ACCEL_INIT           (sint32)(35.0f * BD_TR) //T.R.*35
    #define BD_STOP_INIT            10000

    #define BD_MANUAL_METHOD_INIT   MCI_MANUAL_INJECTION_AC_CURRENT
    #define BD_LEVEL_INIT           1.0f
    #define BD_LEVEL_RATE_INIT      1.0f
    #define BD_PARAM_INIT           1.0f
    #define BD_PARAM_RATE_INIT      1.0f

    // section required in case of CUSTOM COMMANDS IMPLEMENTATION (not through MCI API)  // uncomment if needed
    //#define BD_CUSTOM_CMD
    #ifdef BD_CUSTOM_CMD
        // define here macros for custom commands
        #define BD_RUN(motor_index, target_speed, target_accel)
        #define BD_MANUAL_INJ(motor_index, method, level_x32, level_rate_x32, param_x32, param_rate_x32)
        #define BD_STOP(motor_index, target_stop)
    #endif
#endif




#define BD_RESET                 ENABLED     // enabling\disabling board reset from this module

#define BD_ADC_SYNCH_ADJUST      DISABLED     // enabling\disabling adjustment of ADC synchronization


#endif /* BOARDDEBUGGING_PRV_H_ */
