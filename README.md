# Simplistic code for nRF52 used to find and debug TWI devices
Example code meant to be used to debug TWI applications. It is just a simple application that scans through all possible 7-bits TWI address. If a device is found on a particular address the example prints out the address on a UART terminal. This is the exact address you should pass to the TWI drivers in the SDK. 

If the example cannot find any devices then double check, and then triple check, that:
* You use the correct pins on the device and the nRF52.
* That the TWI device is powered on. 
* The nRF52 has internal pull-ups, but you might need to add external ones as well.

# Tested on:
* nRF52 DK (PCA10040)
* SDK 13.0.0-1.alpha.

# Disclaimer
<b>
 * The example is not extensively tested
 * NO WARRANTY of ANY KIND is provided.
 </b>
