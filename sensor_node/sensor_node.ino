/**
 * Copyright (C) 2021 Bosch Sensortec GmbH
 *
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 */

/* The new sensor needs to be conditioned before the example can work reliably. You may run this
 * example for 24hrs to let the sensor stabilize.
 */

/**
 * basic_config_state.ino sketch :
 * This is an example for integration of BSEC2x library in BME688 development kit,
 * which has been designed to work with Adafruit ESP32 Feather Board
 * For more information visit : 
 * https://www.bosch-sensortec.com/software-tools/software/bme688-software/
 * 
 * For quick integration test, example code can be used with configuration file under folder
 * Bosch_BSEC2_Library/src/config/FieldAir_HandSanitizer (Configuration file added as simple
 * code example for integration but not optimized on classification performance)
 * Config string for H2S and NonH2S target classes is also kept for reference (Suitable for
 * lab-based characterization of the sensor)
 */

#include <EEPROM.h>
#include "lib/bsec2/bsec2.h"
#include "lib/bsec2/commMux/commMux.h"
/* For two class classification use configuration under config/FieldAir_HandSanitizer */
#define CLASSIFICATION	1
#define REGRESSION		  2

#define COMPLETED       1

/* Note : 
          For the classification output from BSEC algorithm set OUTPUT_MODE macro to CLASSIFICATION.
          For the regression output from BSEC algorithm set OUTPUT_MODE macro to REGRESSION.
*/
#define OUTPUT_MODE   CLASSIFICATION

#if (OUTPUT_MODE == CLASSIFICATION)
  const uint8_t bsec_config[] = {
                                  2,0,5,2,189,1,0,0,0,0,0,0,247,7,0,0,176,0,1,0,0,168,19,73,64,49,119,76,0,0,97,69,0,0,97,69,137,65,0,191,205,204,204,190,0,0,64,191,225,122,148,190,10,0,3,0,0,0,96,64,23,183,209,56,0,0,0,0,0,0,0,0,0,0,0,0,205,204,204,189,0,0,0,0,0,0,0,0,0,0,128,63,0,0,0,0,0,0,128,63,0,0,0,0,0,0,0,0,0,0,128,63,0,0,0,0,0,0,128,63,0,0,0,0,0,0,0,0,0,0,128,63,0,0,0,0,0,0,128,63,82,73,157,188,95,41,203,61,118,224,108,63,155,230,125,63,191,14,124,63,0,0,160,65,0,0,32,66,0,0,160,65,0,0,32,66,0,0,32,66,0,0,160,65,0,0,32,66,0,0,160,65,8,0,2,0,0,0,72,66,16,0,3,0,10,215,163,60,10,215,35,59,10,215,35,59,13,0,5,0,0,0,0,0,100,254,131,137,87,88,0,9,0,7,240,150,61,0,0,0,0,0,0,0,0,28,124,225,61,52,128,215,63,0,0,160,64,0,0,0,0,0,0,0,0,205,204,12,62,103,213,39,62,230,63,76,192,0,0,0,0,0,0,0,0,145,237,60,191,251,58,64,63,177,80,131,64,0,0,0,0,0,0,0,0,93,254,227,62,54,60,133,191,0,0,64,64,12,0,10,0,0,0,0,0,0,0,0,0,45,5,11,0,0,0,2,149,186,208,61,180,151,171,61,207,14,15,63,174,251,104,62,183,185,71,62,121,86,250,61,108,206,175,62,73,39,103,62,128,143,255,188,119,73,180,61,188,229,67,62,241,8,221,60,96,15,130,62,74,176,161,62,25,176,70,62,119,111,215,61,243,179,18,62,165,208,136,62,247,93,141,62,165,10,89,61,83,113,174,60,83,113,174,188,0,0,0,0,0,0,0,0,143,85,143,191,237,125,157,63,22,224,49,191,240,199,45,63,124,156,81,191,100,148,202,188,26,144,161,190,210,63,120,62,122,190,8,63,95,113,50,191,234,255,197,62,199,72,222,61,241,159,22,63,87,161,242,190,228,132,182,62,79,29,141,189,188,177,189,190,82,248,4,191,197,224,200,62,69,237,69,191,33,211,39,62,191,108,174,190,40,23,117,63,152,254,203,188,126,62,244,62,197,2,12,191,103,42,86,190,57,112,40,191,214,65,76,62,46,191,43,191,58,38,76,62,67,56,22,63,115,215,144,61,78,40,52,63,88,180,145,61,157,34,171,190,88,161,18,190,157,171,9,62,47,203,38,61,20,77,205,190,105,104,21,190,240,99,171,190,168,244,115,190,26,149,87,191,92,91,160,188,70,235,236,62,49,174,61,62,226,199,60,191,185,255,225,190,40,152,169,188,192,96,129,62,254,128,214,62,21,244,16,62,72,93,0,190,142,147,134,190,149,57,135,190,89,210,126,62,183,31,229,190,97,113,190,190,82,71,241,59,127,28,223,189,138,125,252,190,105,190,86,191,190,236,103,189,196,10,1,62,248,236,39,62,0,39,40,191,21,4,132,62,237,231,63,62,201,128,220,62,235,178,195,62,143,122,68,191,15,123,83,191,11,253,1,191,239,18,206,189,240,18,142,190,152,59,220,62,165,223,152,190,75,43,234,189,204,178,13,63,179,166,89,190,115,108,18,190,169,94,74,189,58,82,170,61,160,206,94,189,183,10,149,62,6,46,89,62,104,155,250,62,174,235,16,191,94,186,188,190,164,115,25,62,52,122,194,190,101,110,137,190,229,36,98,190,196,113,208,62,45,104,28,191,190,100,3,63,11,229,202,62,215,90,35,190,156,30,217,190,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,226,234,12,191,196,64,236,61,139,147,65,190,41,132,137,191,129,69,160,62,79,108,215,61,75,148,159,191,125,63,22,63,110,210,193,190,57,181,189,62,146,253,0,64,73,134,250,191,9,113,176,63,46,209,245,63,25,225,102,191,209,250,230,191,105,206,205,63,164,166,192,191,243,199,159,63,106,76,130,191,106,162,15,63,110,218,132,63,174,50,167,190,192,201,33,63,245,140,65,190,178,221,29,63,245,139,12,63,172,89,244,187,30,87,100,62,165,119,9,63,84,100,82,63,254,179,243,190,46,38,58,63,88,225,128,63,24,8,157,190,124,63,28,191,147,144,199,62,123,106,16,191,33,164,125,63,188,133,24,191,51,205,14,191,108,32,24,63,37,167,131,190,146,209,202,190,66,23,57,63,189,124,100,63,151,68,8,191,212,91,26,63,53,42,19,191,178,117,151,61,240,46,181,190,71,76,132,61,55,5,203,188,66,94,19,190,17,72,238,189,47,145,0,62,158,193,54,190,95,21,26,63,204,122,32,62,41,28,29,191,124,135,117,190,41,36,62,63,149,253,195,61,63,34,31,191,201,165,71,63,214,22,1,63,175,185,168,189,5,182,97,63,116,145,227,190,142,190,71,63,91,145,132,62,204,211,185,190,228,163,26,63,180,143,48,190,29,133,184,189,155,65,224,190,205,73,6,63,230,196,185,62,183,243,59,62,111,249,46,190,107,155,171,63,213,194,20,191,78,128,92,63,186,97,111,63,181,143,149,191,166,234,158,191,82,117,157,63,212,49,8,191,141,128,144,63,251,143,150,191,149,184,8,190,199,34,111,190,248,160,80,62,59,167,0,191,230,30,18,63,31,30,113,62,83,6,92,190,219,193,97,62,114,122,254,188,126,230,153,190,137,228,99,191,141,136,71,63,0,0,0,0,0,0,0,0,170,223,255,63,68,56,12,192,0,0,0,0,0,0,0,0,57,222,239,190,195,234,43,62,0,0,0,0,0,0,0,0,118,94,176,191,62,15,214,63,0,0,0,0,0,0,0,0,102,94,81,63,192,48,90,191,0,0,0,0,0,0,0,0,204,222,214,63,234,17,206,191,0,0,0,0,0,0,0,0,153,34,97,191,177,92,243,62,0,0,0,0,0,0,0,0,146,192,52,63,165,49,151,191,0,0,0,0,0,0,0,0,122,17,236,190,53,9,25,63,0,0,0,0,0,0,0,0,34,244,81,63,60,40,229,190,0,0,0,0,0,0,0,0,10,10,2,3,32,108,72,154,240,124,75,206,177,197,74,41,15,18,74,234,185,131,72,224,16,119,72,95,157,102,72,164,186,59,72,151,180,90,72,121,150,102,72,0,0,0,0,0,0,0,0,0,0,0,0,53,91,7,72,48,131,77,75,231,181,156,74,252,65,229,73,186,42,57,72,180,135,50,72,184,132,41,72,128,82,213,71,204,115,251,71,4,154,4,72,0,0,128,63,0,0,128,63,0,0,128,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,145,1,254,0,2,1,5,48,117,100,0,44,1,112,23,151,7,132,3,197,0,92,4,144,1,64,1,64,1,144,1,48,117,48,117,48,117,48,117,100,0,100,0,100,0,48,117,48,117,48,117,100,0,100,0,48,117,48,117,8,7,8,7,8,7,8,7,8,7,8,7,8,7,8,7,8,7,100,0,100,0,100,0,100,0,48,117,48,117,48,117,100,0,100,0,100,0,48,117,48,117,100,0,100,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,112,23,112,23,112,23,112,23,8,7,8,7,8,7,8,7,112,23,112,23,112,23,112,23,112,23,112,23,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,112,23,112,23,112,23,112,23,255,255,255,255,220,5,220,5,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,220,5,220,5,220,5,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,48,117,0,1,0,5,0,2,0,10,0,30,0,5,0,5,0,5,0,5,0,5,0,5,0,64,1,100,0,100,0,100,0,200,0,200,0,200,0,64,1,64,1,64,1,10,1,0,0,0,0,0,166,146,0,0
                                };
#elif (OUTPUT_MODE == REGRESSION)
  const uint8_t bsec_config[] = {
                                  2,0,5,2,189,1,0,0,0,0,0,0,247,7,0,0,176,0,1,0,0,192,168,71,64,49,119,76,0,0,97,69,0,0,97,69,137,65,0,191,205,204,204,190,0,0,64,191,225,122,148,190,10,0,3,0,0,0,96,64,23,183,209,56,0,0,0,0,0,0,0,0,0,0,0,0,205,204,204,189,0,0,0,0,0,0,0,0,0,0,128,63,0,0,0,0,0,0,128,63,0,0,0,0,0,0,0,0,0,0,128,63,0,0,0,0,0,0,128,63,0,0,0,0,0,0,0,0,0,0,128,63,0,0,0,0,0,0,128,63,82,73,157,188,95,41,203,61,118,224,108,63,155,230,125,63,191,14,124,63,0,0,160,65,0,0,32,66,0,0,160,65,0,0,32,66,0,0,32,66,0,0,160,65,0,0,32,66,0,0,160,65,8,0,2,0,0,0,72,66,16,0,3,0,10,215,163,60,10,215,35,59,10,215,35,59,13,0,5,0,0,0,0,0,100,35,41,29,86,88,0,9,0,7,240,150,61,0,0,0,0,0,0,0,0,28,124,225,61,52,128,215,63,0,0,160,64,0,0,0,0,0,0,0,0,205,204,12,62,103,213,39,62,230,63,76,192,0,0,0,0,0,0,0,0,145,237,60,191,251,58,64,63,177,80,131,64,0,0,0,0,0,0,0,0,93,254,227,62,54,60,133,191,0,0,64,64,12,0,10,0,0,0,0,0,0,0,0,0,45,5,11,0,1,1,0,116,77,73,192,170,234,154,193,244,150,196,191,170,223,161,64,2,228,33,65,60,58,105,64,56,148,131,192,97,49,71,64,182,61,174,63,119,74,42,192,255,50,178,65,127,251,112,65,121,88,223,65,34,253,102,65,233,72,103,65,114,10,212,194,204,127,28,192,148,246,218,193,51,196,90,194,198,28,248,66,20,208,112,64,0,0,0,0,0,0,0,0,0,0,0,0,162,122,133,193,52,229,158,193,202,95,189,193,145,0,132,194,252,58,182,194,200,68,177,64,37,245,2,194,3,184,232,65,160,13,98,65,94,146,131,193,94,199,37,65,45,180,123,193,228,67,236,64,186,106,243,64,21,68,243,193,39,210,234,64,157,69,115,63,149,240,56,193,173,68,193,192,207,65,31,65,233,128,169,193,152,148,161,65,54,122,16,193,138,189,251,63,231,164,169,65,25,128,213,63,36,27,186,191,38,174,183,64,11,46,17,65,166,189,244,192,140,182,146,65,183,28,217,192,38,254,19,64,249,75,243,192,50,69,78,64,223,14,162,193,92,76,202,192,23,54,100,65,61,149,129,192,94,201,51,192,44,50,244,65,7,228,38,65,233,23,135,64,9,7,14,194,179,163,86,64,10,193,52,192,91,77,48,66,145,248,4,194,237,31,238,63,183,55,203,191,249,24,39,65,153,200,195,65,92,125,48,192,180,28,147,65,199,115,193,193,163,45,86,193,188,241,128,192,188,158,138,193,51,130,192,64,18,233,64,65,181,39,224,193,169,53,82,194,200,182,93,192,251,48,54,65,42,166,179,64,96,38,80,65,53,221,152,193,91,98,189,65,139,102,114,192,18,164,196,192,121,33,212,193,168,84,149,66,71,186,226,64,84,250,115,194,213,213,142,66,153,145,59,65,54,114,63,66,244,173,194,193,47,84,211,64,184,216,24,65,11,64,2,193,5,0,146,65,229,109,173,192,37,252,224,65,114,102,1,193,226,3,239,65,100,141,146,65,100,30,181,193,141,97,159,65,190,132,131,191,6,174,210,65,33,161,150,193,120,105,199,65,185,40,186,66,130,196,48,66,22,16,243,193,249,220,33,194,125,145,0,66,12,158,50,194,164,146,16,65,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,110,170,157,65,198,180,124,194,35,32,156,65,27,238,146,65,92,52,149,65,74,156,30,193,194,202,127,193,166,209,153,193,254,229,183,65,58,94,123,66,121,157,96,65,122,154,64,193,203,250,104,65,96,244,59,64,214,77,53,64,151,135,70,190,194,171,17,193,35,226,98,193,214,38,41,190,183,169,78,66,235,15,151,193,26,37,220,65,65,190,115,64,68,170,150,65,157,2,155,65,119,172,120,194,54,80,96,193,249,145,129,192,215,84,225,191,176,210,179,65,42,113,146,63,116,44,32,63,34,228,133,191,1,166,22,192,217,22,26,192,136,65,174,64,139,69,88,193,25,235,136,63,90,51,105,190,239,71,81,193,157,129,0,193,194,225,159,193,90,214,180,192,69,213,51,193,152,217,55,193,162,149,162,66,86,240,16,193,17,191,183,64,29,113,221,63,50,128,80,193,23,177,250,64,118,91,16,194,111,165,28,65,110,22,82,65,150,149,86,65,86,179,24,193,217,118,155,192,90,32,28,193,255,11,149,66,75,218,27,66,144,107,155,64,197,117,242,193,6,23,143,64,58,241,89,65,168,252,95,65,231,184,237,65,249,71,156,193,165,56,149,192,26,209,12,64,172,89,94,193,51,60,59,65,104,31,49,194,92,118,18,65,113,153,195,65,235,218,200,65,107,210,61,193,80,162,135,193,234,242,22,193,239,161,244,63,181,33,149,193,252,104,115,193,121,57,131,65,157,158,137,64,228,242,177,65,207,38,183,65,210,243,14,194,11,37,171,193,64,132,146,192,244,132,6,192,67,50,10,193,227,166,154,65,121,201,71,194,246,106,33,64,12,208,188,64,64,153,193,64,156,1,140,193,66,127,13,193,81,140,37,192,170,68,175,61,233,240,195,193,83,150,33,190,0,0,0,0,0,0,0,0,0,0,0,0,195,211,203,62,0,0,0,0,0,0,0,0,0,0,0,0,233,183,207,64,0,0,0,0,0,0,0,0,0,0,0,0,64,164,15,65,0,0,0,0,0,0,0,0,0,0,0,0,198,109,8,193,0,0,0,0,0,0,0,0,0,0,0,0,31,244,236,190,0,0,0,0,0,0,0,0,0,0,0,0,186,218,202,190,0,0,0,0,0,0,0,0,0,0,0,0,3,9,220,64,0,0,0,0,0,0,0,0,0,0,0,0,35,74,145,64,0,0,0,0,0,0,0,0,0,0,0,0,96,229,208,62,0,0,0,0,0,0,0,0,0,0,0,0,10,10,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,128,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,63,0,0,0,0,0,0,0,0,0,0,0,0,145,1,254,0,2,1,5,48,117,100,0,44,1,112,23,151,7,132,3,197,0,92,4,144,1,64,1,64,1,144,1,48,117,48,117,48,117,48,117,100,0,100,0,100,0,48,117,48,117,48,117,100,0,100,0,48,117,48,117,8,7,8,7,8,7,8,7,8,7,8,7,8,7,8,7,8,7,100,0,100,0,100,0,100,0,48,117,48,117,48,117,100,0,100,0,100,0,48,117,48,117,100,0,100,0,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,44,1,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,112,23,112,23,112,23,112,23,8,7,8,7,8,7,8,7,112,23,112,23,112,23,112,23,112,23,112,23,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,112,23,112,23,112,23,112,23,255,255,255,255,220,5,220,5,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,220,5,220,5,220,5,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,48,117,0,5,10,5,0,2,0,10,0,30,0,5,0,5,0,5,0,5,0,5,0,5,0,64,1,100,0,100,0,100,0,200,0,200,0,200,0,64,1,64,1,64,1,10,1,1,0,0,0,0,26,228,0,0
                                };
#endif

/* Macros used */
#define STATE_SAVE_PERIOD   UINT32_C(360 * 60 * 1000) /* 360 minutes - 4 times a day */
/* Number of sensors to operate*/
#define NUM_OF_SENS    8
#define PANIC_LED	LED_BUILTIN
#define ERROR_DUR   1000

/* Helper functions declarations */
/**
 * @brief : This function toggles the led continuously with one second delay
 */
void errLeds(void);

/**
 * @brief : This function checks the BSEC status, prints the respective error code. Halts in case of error
 * @param[in] bsec  : Bsec2 class object
 */
void checkBsecStatus(Bsec2 bsec);

/**
 * @brief : This function updates/saves BSEC state
 * @param[in] bsec  : Bsec2 class object
 */
void updateBsecState(Bsec2 bsec);

/**
 * @brief : This function is called by the BSEC library when a new output is available
 * @param[in] input     : BME68X sensor data before processing
 * @param[in] outputs   : Processed BSEC BSEC output data
 * @param[in] bsec      : Instance of BSEC2 calling the callback
 */
void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec);

/**
 * @brief : This function retrieves the existing state
 * @param : Bsec2 class object
 */
bool loadState(Bsec2 bsec);

/**
 * @brief : This function writes the state into EEPROM
 * @param : Bsec2 class object
 */
bool saveState(Bsec2 bsec);

/* Create an object of the class Bsec2 */
Bsec2 envSensor[NUM_OF_SENS];
comm_mux commConfig[NUM_OF_SENS];
uint8_t bsecMemBlock[NUM_OF_SENS][BSEC_INSTANCE_SIZE];
uint8_t sensor = 0;
static uint8_t bsecState[BSEC_MAX_STATE_BLOB_SIZE];
/* Gas estimate names will be according to the configuration classes used */
const String gasName[] = { "Field Air", "Hand sanitizer", "Undefined 3", "Undefined 4"};

/* Entry point for the example */
void setup(void)
{
	
#if (OUTPUT_MODE == CLASSIFICATION)
	/* Desired subscription list of BSEC2 Classification outputs */
    bsecSensor sensorList[] = {
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_RAW_GAS_INDEX,
            BSEC_OUTPUT_GAS_ESTIMATE_1,
            BSEC_OUTPUT_GAS_ESTIMATE_2,
            BSEC_OUTPUT_GAS_ESTIMATE_3,
            BSEC_OUTPUT_GAS_ESTIMATE_4
    };
#elif (OUTPUT_MODE == REGRESSION)
	/* Desired subscription list of BSEC2 Regression outputs */
	bsecSensor sensorList[] = {
            BSEC_OUTPUT_RAW_TEMPERATURE,
            BSEC_OUTPUT_RAW_PRESSURE,
            BSEC_OUTPUT_RAW_HUMIDITY,
            BSEC_OUTPUT_RAW_GAS,
            BSEC_OUTPUT_RAW_GAS_INDEX,
            BSEC_OUTPUT_REGRESSION_ESTIMATE_1,
            BSEC_OUTPUT_REGRESSION_ESTIMATE_2,
            BSEC_OUTPUT_REGRESSION_ESTIMATE_3,
            BSEC_OUTPUT_REGRESSION_ESTIMATE_4
    };
#endif

    Serial.begin(115200);
    EEPROM.begin(BSEC_MAX_STATE_BLOB_SIZE + 1);
    /* Initiate SPI communication */
    comm_mux_begin(Wire, SPI);
    pinMode(PANIC_LED, OUTPUT);
	  delay(100);
	  /* Valid for boards with USB-COM. Wait until the port is open */
    while (!Serial) delay(10);

    uint8_t state_write_otp = 0;
    
    for (uint8_t i = 0; i < NUM_OF_SENS; i++)
    {
        /* Sets the Communication interface for the given sensor */
        commConfig[i] = comm_mux_set_config(Wire, SPI, i, commConfig[i]);
    
         /* Assigning a chunk of memory block to the bsecInstance */
         envSensor[i].allocateMemory(bsecMemBlock[i]);
   
        /* Initialize the library and interfaces */
        if (!envSensor[i].begin(BME68X_SPI_INTF, comm_mux_read, comm_mux_write, comm_mux_delay, &commConfig[i]))
        {
            checkBsecStatus (envSensor[i]);
        }

        /* Load the configuration string that stores information on how to classify the detected gas */
        if (!envSensor[i].setConfig(bsec_config))
        {
        	checkBsecStatus (envSensor[i]);
        }

        /* Copy state from the EEPROM to the algorithm */
        if (state_write_otp == 0) 
        {
          if (!loadState(envSensor[i]))
          {
              checkBsecStatus (envSensor[i]);
          }
          state_write_otp = COMPLETED;
        }

        /* Subscribe for the desired BSEC2 outputs */
        if (!envSensor[i].updateSubscription(sensorList, ARRAY_LEN(sensorList), BSEC_SAMPLE_RATE_SCAN))
        {
            checkBsecStatus (envSensor[i]);
        }

        /* Whenever new data is available call the newDataCallback function */
        envSensor[i].attachCallback(newDataCallback);

        updateBsecState(envSensor[i]);
    }

    Serial.println("\nBSEC library version " + \
            String(envSensor[0].version.major) + "." \
            + String(envSensor[0].version.minor) + "." \
            + String(envSensor[0].version.major_bugfix) + "." \
            + String(envSensor[0].version.minor_bugfix));
}

/* Function that is looped forever */
void loop(void)
{
    /* Call the run function often so that the library can
     * check if it is time to read new data from the sensor
     * and process it.
     */
    for (sensor = 0; sensor < NUM_OF_SENS; sensor++)
    {
        if (!envSensor[sensor].run())
        {
         checkBsecStatus(envSensor[sensor]);
        }
    }
}

void errLeds(void)
{
    while(1)
    {
        digitalWrite(PANIC_LED, HIGH);
        delay(ERROR_DUR);
        digitalWrite(PANIC_LED, LOW);
        delay(ERROR_DUR);
    }
}

void updateBsecState(Bsec2 bsec)
{
    static uint16_t stateUpdateCounter = 0;
    bool update = false;

    if (!stateUpdateCounter || (stateUpdateCounter * STATE_SAVE_PERIOD) < millis())
    {
        /* Update every STATE_SAVE_PERIOD minutes */
        update = true;
        stateUpdateCounter++;
    }

    if (update && !saveState(bsec))
        checkBsecStatus(bsec);
}

void newDataCallback(const bme68xData data, const bsecOutputs outputs, Bsec2 bsec)
{
    if (!outputs.nOutputs)
        return;

    Serial.println("BSEC outputs:\n\tsensor num = " + String(sensor));
    Serial.println("\ttimestamp = " + String((int) (outputs.output[0].time_stamp / INT64_C(1000000))));

	  int index = 0;

    for (uint8_t i = 0; i < outputs.nOutputs; i++)
    {
        const bsecData output  = outputs.output[i];
        switch (output.sensor_id)
        {
            case BSEC_OUTPUT_RAW_TEMPERATURE:
                Serial.println("\ttemperature = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_PRESSURE:
                Serial.println("\tpressure = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_HUMIDITY:
                Serial.println("\thumidity = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_GAS:
                Serial.println("\tgas resistance = " + String(output.signal));
                break;
            case BSEC_OUTPUT_RAW_GAS_INDEX:
                Serial.println("\tgas index = " + String(output.signal));
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE:
                Serial.println("\tcompensated temperature = " + String(output.signal));
                break;
            case BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY:
                Serial.println("\tcompensated humidity = " + String(output.signal));
                break;
            case BSEC_OUTPUT_GAS_ESTIMATE_1:
            case BSEC_OUTPUT_GAS_ESTIMATE_2:
            case BSEC_OUTPUT_GAS_ESTIMATE_3:
            case BSEC_OUTPUT_GAS_ESTIMATE_4:
                index = (output.sensor_id - BSEC_OUTPUT_GAS_ESTIMATE_1);
                if (index == 0) // The four classes are updated from BSEC with same accuracy, thus printing is done just once.
                {
                  Serial.println("\taccuracy = " + String((int) output.accuracy));
                }
                Serial.println(("\tclass " + String(index + 1) + " probability : ") + String(output.signal * 100) + "%");
                break;
            case BSEC_OUTPUT_REGRESSION_ESTIMATE_1:
            case BSEC_OUTPUT_REGRESSION_ESTIMATE_2:
            case BSEC_OUTPUT_REGRESSION_ESTIMATE_3:
            case BSEC_OUTPUT_REGRESSION_ESTIMATE_4:                
                index = (output.sensor_id - BSEC_OUTPUT_REGRESSION_ESTIMATE_1);
                if (index == 0) // The four targets are updated from BSEC with same accuracy, thus printing is done just once.
                {
                  Serial.println("\taccuracy = " + String(output.accuracy));
                }
                Serial.println("\ttarget " + String(index + 1) + " : " + String(output.signal * 100));
                break;
            default:
                break;
        }
    }

}

void checkBsecStatus(Bsec2 bsec)
{
    if (bsec.status < BSEC_OK)
    {
        Serial.println("BSEC error code : " + String(bsec.status));
        errLeds(); /* Halt in case of failure */
    } else if (bsec.status > BSEC_OK)
    {
        Serial.println("BSEC warning code : " + String(bsec.status));
    }

    if (bsec.sensor.status < BME68X_OK)
    {
        Serial.println("BME68X error code : " + String(bsec.sensor.status));
        errLeds(); /* Halt in case of failure */
    } else if (bsec.sensor.status > BME68X_OK)
    {
        Serial.println("BME68X warning code : " + String(bsec.sensor.status));
    }
}

bool loadState(Bsec2 bsec)
{

    if (EEPROM.read(0) == BSEC_MAX_STATE_BLOB_SIZE)
    {
        /* Existing state in EEPROM */
        Serial.println("Reading state from EEPROM");
        Serial.print("State file: ");
        for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE; i++)
        {
            bsecState[i] = EEPROM.read(i + 1);
            Serial.print(String(bsecState[i], HEX) + ", ");
        }
        Serial.println();

        if (!bsec.setState(bsecState))
            return false;
    } else
    {
        /* Erase the EEPROM with zeroes */
        Serial.println("Erasing EEPROM");

        for (uint8_t i = 0; i <= BSEC_MAX_STATE_BLOB_SIZE; i++)
            EEPROM.write(i, 0);

        EEPROM.commit();
    }
    return true;
}

bool saveState(Bsec2 bsec)
{
    if (!bsec.getState(bsecState))
        return false;

    Serial.println("Writing state to EEPROM");
    Serial.print("State file: ");

    for (uint8_t i = 0; i < BSEC_MAX_STATE_BLOB_SIZE; i++)
    {
        EEPROM.write(i + 1, bsecState[i]);
        Serial.print(String(bsecState[i], HEX) + ", ");
    }
    Serial.println();

    EEPROM.write(0, BSEC_MAX_STATE_BLOB_SIZE);
    EEPROM.commit();

    return true;
}