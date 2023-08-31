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

The export program is a simple python script that will merge all your checkpoints and send to the server.

Create a virtual environment and install the requirements

```
python -m venv buddhabroute
source buddhabroute/bin/activate
pip install -r requirements.txt
```

You should modify the `config.ini` with the URL and port of running server.

Finally, add a cronjob to automatically export your work

```
crontab -l > mycron
echo "0 12 * * $(pwd)/buddhabroute/bin/activate /usr/bin/python $(pwd)/export.py" >> mycron
#install new cron file
crontab mycron
rm mycron
```
All done !

## Running
```
./buddhabroute
```
Every 100K iterations, the program will save to a checkpoint file the histogram it is computing. The filename is randomised every time the program is launched.

I recommend using XScreensaver to manage the screensaver part. You can add computeIdle to the list of screensaver by modifying `{HOME}/.xscreensaver`. Add the full path to the executable in the programs section.
