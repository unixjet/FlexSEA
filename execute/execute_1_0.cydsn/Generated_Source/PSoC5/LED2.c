/*******************************************************************************
* File Name: LED2.c  
* Version 2.5
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "LED2.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 LED2__PORT == 15 && ((LED2__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: LED2_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void LED2_Write(uint8 value) 
{
    uint8 staticBits = (LED2_DR & (uint8)(~LED2_MASK));
    LED2_DR = staticBits | ((uint8)(value << LED2_SHIFT) & LED2_MASK);
}


/*******************************************************************************
* Function Name: LED2_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  LED2_DM_STRONG     Strong Drive 
*  LED2_DM_OD_HI      Open Drain, Drives High 
*  LED2_DM_OD_LO      Open Drain, Drives Low 
*  LED2_DM_RES_UP     Resistive Pull Up 
*  LED2_DM_RES_DWN    Resistive Pull Down 
*  LED2_DM_RES_UPDWN  Resistive Pull Up/Down 
*  LED2_DM_DIG_HIZ    High Impedance Digital 
*  LED2_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void LED2_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(LED2_0, mode);
}


/*******************************************************************************
* Function Name: LED2_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro LED2_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 LED2_Read(void) 
{
    return (LED2_PS & LED2_MASK) >> LED2_SHIFT;
}


/*******************************************************************************
* Function Name: LED2_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 LED2_ReadDataReg(void) 
{
    return (LED2_DR & LED2_MASK) >> LED2_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(LED2_INTSTAT) 

    /*******************************************************************************
    * Function Name: LED2_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 LED2_ClearInterrupt(void) 
    {
        return (LED2_INTSTAT & LED2_MASK) >> LED2_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
