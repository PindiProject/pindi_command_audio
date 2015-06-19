# Pindi Command Audio

To run the code you must to compile it with:
gcc clap_detection.c -o clap_detection -lasound -lsndfile -lm

And then play claps to turn on or turn off the Pindi robot.

To run it on Raspberry Pi you must to add, in the end of file /boot/cmdline.txt, the next line:
dwc_otg.speed=1
