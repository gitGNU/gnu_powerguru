/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf,v 1.9 2005/01/07 09:37:18 dts12 Exp $
 */
#ifndef OUTBACK_H
#define OUTBACK_H

/*
 * function declarations 
 */
void            init_outback(void);
Netsnmp_Node_Handler handle_outbackMXAddress;
Netsnmp_Node_Handler handle_outbackMXChargeAmps;
Netsnmp_Node_Handler handle_outbackMXPVVolts;
Netsnmp_Node_Handler handle_outbackMXDailyKW;
Netsnmp_Node_Handler handle_outbackMXAuxMode;
Netsnmp_Node_Handler handle_outbackMXErrorMode;
Netsnmp_Node_Handler handle_outbackMXChargerMode;
Netsnmp_Node_Handler handle_outbackMXBatteryVolts;
Netsnmp_Node_Handler handle_outbackFXAddress;
Netsnmp_Node_Handler handle_outbackFXLoadAmps;
Netsnmp_Node_Handler handle_outbackFXChargeAmps;
Netsnmp_Node_Handler handle_outbackFXBuyAmps;
Netsnmp_Node_Handler handle_outbackFXInputVolts;
Netsnmp_Node_Handler handle_outbackFXOutputVolts;
Netsnmp_Node_Handler handle_outbackFXSellAmps;
Netsnmp_Node_Handler handle_outbackFXOpMode;
Netsnmp_Node_Handler handle_outbackFXErrorMode;
Netsnmp_Node_Handler handle_outbackFXACMode;
Netsnmp_Node_Handler handle_outbackFXBatteryVolts;
Netsnmp_Node_Handler handle_outbackFXMiscByte;
Netsnmp_Node_Handler handle_outbackFXWarningMode;
Netsnmp_Node_Handler handle_outbackFXCommand;

#endif                          /* OUTBACK_H */