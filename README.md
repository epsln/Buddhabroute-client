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

You should modify the `config.ini` with the URL and port of running server. You can also modify the number of iterations to do before 
exporting your work. The sleep_time parameters controls the amount of sleep between iterations, so you can try to not burn out your poor cpu.

## Running
Everything is done via the export.py script. 

```
python export.py
```

Which will launch the program and export to the provided url.

I recommend using XScreensaver to manage the screensaver part. You can add computeIdle to the list of screensaver by modifying `{HOME}/.xscreensaver`. Add the full path to the executable in the programs section.
