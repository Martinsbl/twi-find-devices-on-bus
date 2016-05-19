# Simplistic code for nRF5 used to find and debug TWI devices
Example code meant to be used to debug TWI applications. It is just a simple application that scans through all possible 7-bits TWI address. If a device is found on a particular address the example prints out the address on a UART terminal. This is the exact address you should pass to the TWI drivers in the SDK. In SDK 10 and later you should not need to manually add a read or write bit to the address. The drivers will handle that for you. 

If the example cannot find any devices then double check, and then triple check, that:
* You use the correct pins on the device and the nRF5.
* That the TWI device is powered on. 
* The nRF5 has internal pull-ups, but you might need to add external ones as well.

# Tested on:
* nRF51 DK (PCA10028)
* nRF52 DK (PCA10040)
* SDK 0.9.2 tested only on PCA10040. NOT tested on PCA10036.

# Disclaimer
<b>
 * The example is not extensively tested
 * NO WARRANTY of ANY KIND is provided.
 </b>
