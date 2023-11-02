/**
  Color sensor click board functionality header file

  @File Name
    color.h

  @Description
    This header file provides functionality for color sensor click board by Mikroelektronika.
 
  @References
    1. http://whatis.techtarget.com/definition/hue-saturation-and-brightness
    2. TCS3471 Color Light-to-Digital Converter data sheet
    3. https://libstock.mikroe.com/projects/view/780/color-click-example
    
*/
#ifndef _COLOR_H
#define _COLOR_H

/**
  Section: Included Files
*/

#include <stdint.h>

/**
 Section: Macro definitions
*/

// Find maximal floating point value
#define MAX_FLOAT(a, b) (((a) > (b)) ? (a) : (b))

// Find minimal floating point value
#define MIN_FLOAT(a, b) (((a) < (b)) ? (a) : (b))

#define I2C_SLAVE_ADDR (0x38U << 1)

#define _XTAL_FREQ 16000000

/**
  Section: Variable declaration
 
 @Description:      
    The color sensor click board has a color sensor TCS3471 that has a 4*4 photodiode array. The array is composed of
    a red-filtered, blue-filtered, green-filtered and clear photodiodes- four of each type. Four integrating ADCs 
    simultaneously convert the amplified photodiode currents to a digital value providing upto a 16-bit resolution.
    Upon completion of conversion cycle, the conversion result is transferred to the data registers. These registers 
    are then read to be find the red, blue, green and clear content of the color. Clear content gives the intensity,
    while the red, green and blue content provide their appropriate channel count. These can be stored and the ratio
    between red, blue or green over clear can be found. These ratio is then used in a formula to find the hue of color
    which determines what color it is. 
    
    Clear:              Type:       unsigned short
                        Function:   Stores the lower byte of clear data register
    ClearData:          Type:       unsigned int
                        Function:   Stores clear data after reading the 16-bit clear data register
    Red:                Type:       unsigned short
                        Function:   Stores the lower byte of red data register
    RedData:            Type:       unsigned int
                        Function:   Stores clear data after reading the 16-bit red data register
    Green:              Type:       unsigned short
                        Function:   Stores the lower byte of green data register
    GreenData:          Type:       unsigned int
                        Function:   Stores clear data after reading the 16-bit green data register
    Blue:               Type:       unsigned short
                        Function:   Stores the lower byte of blue data register
    BlueData:           Type:       unsigned int
                        Function:   Stores clear data after reading the 16-bit blue data register
    RedRatio:           Type:       float
                        Function:   Calculates the ratio between the RedData and the ClearData to find how
                                    much red content is there in the color being sensed
    GreenRatio:         Type:       float
                        Function:   Calculates the ratio between the GreenData and the ClearData to find how
                                    much green content is there in the color being sensed
    BlueRatio:          Type:       float
                        Function:   Calculates the ratio between the BlueData and the ClearData to find how
                                    much blue content is there in the color being sensed  
    RatioMax:           Type:       float volatile
                        Function:   Calculates the maximum of the three ratios that were calculated for red, green and blue
    RatioMin:           Type:       float volatile
                        Function:   Calculates the minimum of the three ratios that were calculated for red, green and blue
    Saturation:         Type:       float volatile
                        Function:   Saturation is an expression for the relative bandwidth of the visible output from a 
                                    light source. It is calculated using the formula:   Saturation = 1 -(RatioMin/RatioMax)
    Hue:                Type:       float
                        Function:   Hue is the wavelength within the visible-light spectrum at which the energy output from
                                    a source is greatest. This is calculated as:
                                            if (RatioMax == RedRatio)
                                                Hue = (GreenRatio - BlueRatio) / (RatioMax - RatioMin)
                                            else if (RatioMax == GreenRatio)
                                                Hue = 2 + (BlueRatio - RedRatio) / (RatioMax - RatioMin)
                                            else
                                                Hue = 4 + (RedRatio - GreenRatio) / (RatioMax - RatioMin)
                                            Hue = Hue / 6
    CharacterArray:     Type:       char
                        Function:   A character array that is used to display a string on the OLED B click board
    DataByte:           Type:       uint8_t
                        Function:   Data byte array that is used for reading or writing data from or to the I2C bus
    status:             Type:       I2C1_MESSAGE_STATUS
                        Function:   A pointer to the status variable that the I2C driver updates during the 
                                    execution of the message
*/
uint8_t Red, Green, Blue;
uint16_t RedData, GreenData, BlueData;
float RedRatio, GreenRatio, BlueRatio;
float volatile RatioMax, RatioMin, Delta, Max;
int16_t Hue; 
uint8_t Saturation, Value;

uint8_t DataByte[2];
//I2C1_MESSAGE_STATUS status;

/**
  Section: Function definitions
 
*/ 

/**
  @Function
    Color_Initialize
  @Summary
    Initializes the Color sensor Click Board

  @Description
    This routine issues commands to initialize the color sensor click board. The module does a I2C write
    to the enable the click board.

  @Param
    None

  @Returns
    None

 */
void Color_Initialize(void);

/**
  @Function
    Color_Read
  @Summary
    Reads the data registers to gather information of red, green, blue and clear values and calculates the hue of color

  @Description
    This routine reads the red data register to store the 16-bit data into a variable called RedData. 
    Similarly, the GreenData, BlueData and the ClearData is read and stored. The ratios of the each of the 
    red, green and blue with respect to the clear intensity is calculated. Based on the formula, the hue is
    calculated for a color that is sensed by the color sensor click board.

  @Param
    None

  @Returns
    None

 */
void Color_Read(void);

/**
  @Function
    Color_Display
  @Summary
    Based on the hue value, the color is detected and displayed on the OLED B click board

  @Description
    This routine chooses a color to be displayed based on the hue value. The OLED B click board displays the color and then
    their red, green and blue values in hexadecimal format. The following colors are detected based on the hue value: 
    1. ORANGE: If the hue value lies between 0.992 and 0.999
    2. RED: If the hue value lies between 0.9750 and 0.9919
    3. PINK: If the hue value lies between 0.920 and 0.9749
    4. PURPLE: If the hue value lies between 0.6201 and 0.919
    5. BLUE: If the hue value lies between 0.521 and 0.6200
    6. CYAN: If the hue value lies between 0.470 and 0.520
    7. GREEN: If the hue value lies between 0.210 and 0.469
    8. YELLOW: If the hue value lies between 0.0650 and 0.1800
 
  @Param
    None

  @Returns
    None

 */
void Color_Display(void);

#endif // _COLOR_H
