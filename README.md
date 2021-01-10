# Operating-systems-for-embedded-systems
This repository contains the Yocto layer for the assignment

1) How to install the Yocto layer
In this github repository it is located the layer named "monitorlayer". To install that, first we need to have poky installed that contains the OpenEmbedded Build System.
Then we download the repository with the command <git clone https://github.com/Iacopo98/Operating-systems-for-embedded-systems> where we prefer. 
Then we move the repository monitorlayer in poky with the command : <mv ./Operating-systems-for-embedded-systems/monitorlayer ./poky> .
After we change directory with : <cd poky> , if we are using an arm target then we digit the following command: <source oe-init-build-env build_qemuarm> .
To add the layer to the image configuration: <bitbake-layers add-layer ../monitorlayer/> .
Inside the layer there are two recipes in recipes-example, "apprecipe" for the application and "cddrecipe" for the driver. 
In the comments of cddrecipe there is a possible implementation of the sensor, for example in this case the sensor is based on GPIO and it use a UART comunication protocol. 
In the file local.conf in /poky/build_qemuarm we need to add the following rows: <IMAGE_INSTALL_append = " app">, <IMAGE_INSTALL_append = " cddrecipe"> .
Finally we can re-build the image with the new layer with: <bitbake core-image-minimal> . The application can be tested by lanching the system with <runqemu qemuarm>, and then run the app file in user/bin/app . After few second on terminal will appear the bpm: "76 bpm". It continue in loop.
  
  
 2) About Timing
 My timing gestion is the simple as possible, in the app code I just inserted an usleep(20000). So with Yocto i cannot check if it work, but i made some consideration about that. The time from one reading of the data to another will probabily be more than 20 ms because the C instructions insert some more delay, so the frequency of reading will be more slow than 50 Hz.
For currect this I could add a variable that measure the delay of the code from the current measurement and than subtract this value in the following usleep. For example if the measured delay is x, the next usleep value will be: usleep(20000 - x); . But this is worst that have a constant delay! In fact, for example if we have a maximum delay of ymax that is 2 times the minimun delay ymin, the following situation will occur. A reading takes 20 ms + ymax times. The code subtract ymax to the following reading, but the new delay could be ymin. So sum of the delay is only ymin, but the first reading takes 20 ms+ymax, the second 20 ms-ymin. (because ymax= 2ymin). The difference between the 2 reading times is 3ymin, so the first one have a frequency slower than 50 Hz, the second have a faster frequency than 50 Hz. So I think that have always a more slower frequency is better that have an oscillating one.    
  
  
  
