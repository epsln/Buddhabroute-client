# Buddhabroute-client
A simple screensaver that also computes the buddhabrot for the Lambda Function, f(z) = r * z * ( 1 - z), with z, r in C, and r being fixed. It computes an histogram of trajectories that eventually escapes after a fixed number iterations.


## Compiling
You will need X11 Libs and opencl-headers
debian :
```
sudo apt install libx11-dev
```
archlinux :
```
pacman -S libx11
```

Then you can compile.
```
make
```

## Running
```
./buddhabroute
```
Every 100K iterations, the program will save to a checkpoint file the histogram it is computing. The filename is randomised every time the program is launched.

I recommend using XScreensaver to manage the screensaver part. You can add computeIdle to the list of screensaver by modifying `{HOME}/.xscreensaver`. Add the full path to the executable in the programs section.


Finally, export your work once every day at noon by adding this line to your crontab

First open your cronjobs
```
crontab -e
```

Then add this line:
```
0 12 * * * * curl -F @/tmp/buddhabroute_checkpoints/* -F ${UUID}{SERVER_ADDRESS} && rm /tmp/buddhabroute_checkpoints/*
```
