# ComputeIDLE
A simple screensaver that also computes the buddhabrot for the Lambda Function, f(z) = r * z * ( 1 - z), with z, r in C, and r being fixed. It computes an histogram of trajectories that eventually escapes after a fixed number iterations.


## Client Side
## Compiling
You will need X11 Libs
```
make
```

### Running
```
./computeIdle
```

I recommend using XScreensaver to manage the screensaver part. You can add computeIdle to the list of screensaver by modifying `{HOME}/.xscreensaver`. Add the full path to the executable in the programs section.


Finally, export your work once every day at noon by adding this line to your crontab

```
crontab -e

0 12 * * * * rsync {PATH_TO_EXEC}/checkpoint.csv {SERVER_ADDRESS}:${}
```

## Server Side
The `compute_server` contains a small program that will handle adding each checkpoints to each other and saving the image.
### Installation
Preferably in a venv, run
```
	pip install -r requirements.txt
```

Next up, create two folders
```
	mkdir arrivals backups
```


Finally, make the script run once every day at noon by adding this line to your crontab

```
crontab -e

0 12 * * * * python {PATH_TO_EXEC}/main.py
```
