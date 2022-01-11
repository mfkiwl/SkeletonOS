# Skeleton

## Idea

things to do while contributing to the project: 
- watch Ronnie O'Sullivan match.
- listen to Apparat.
- eat ice cream (only summer season).

My idea is to put together the basic tools to create a skeleton project in C. Through skeleton the user can be immediately operational, he will have:
- Scheduler
- Logger
- Circular buffer
- JSON config
- lightDSP (FFT iFFT Statistics)
- CMake like
- Multi architecture support

Time permitting, I'll add more ...
MD Syntax Guide: https://www.markdownguide.org/basic-syntax/ 

## Architecture

Available architectures:
- Linux 
- RP2040

## MacOS

### Requirements

#### Homebrew
(https://www.digitalocean.com/community/tutorials/how-to-install-and-use-homebrew-on-macos)

0. Start:
```
$ xcode-select --install
$ cd Dev/
$ curl -fsSL -o install.sh https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh
$ /bin/bash install.sh
```
1. Now, you check if you have bash or zsh shell with:
```
$ echo $0
```
If you’re using ZSH, you’ll open the file ~/.zshrc in your editor:
```
$ vim ~/.zshrc
```
If you’re using the Bash shell, you’ll use the file ~/.bash_profile:
```
$ vim ~/.bash_profile
```
2. Then, Once the file opens up in the Terminal window, add the following lines to the end of the file:
```
# Add Homebrew's executable directory to the front of the PATH
export PATH=/usr/local/sbin:$PATH
```
3. Now, reboot the system, or:
```
$ source ~/.zshrc
```
4. Brew prefer the last OS version:
```
$ softwareupdate --all --install --force
```
5. Use this command for verify Homebrew installation:
```
$ brew doctor
```
If you didn't see any warning, your installation was fine.
(Probably, need Command Line upgraded via Software Upgrade System)
The right output message is:
```
Your system is ready to brew.
```

6. Now, we install a usefull tool for brew:
```
$ brew install tree
```
Take care, brew tree is not a tree line command tools on yours OS, they are two different tool! 
Occasionally, you’ll want to upgrade an existing package. Use the brew upgrade command, followed by the package name:
```
$ brew upgrade tree
```

#### Toolchain
(https://andrejacobs.org/electronics/develop-and-debug-raspberry-pi-pico-on-macos-big-sur/)

1. Ensure everything is ok and update homebrew packages.
```
$ brew doctor
$ brew update
$ brew upgrade
```
2. Install the Toolchain (following section 9.1 Building on Apple macOS from the Official guide)
```
$ brew install cmake
$ brew tap ArmMbed/homebrew-formulae
$ brew install arm-none-eabi-gcc
```
3. Now, we get the RP2040 SDK:
```
# cd into a directory where you want to store the files
$ git clone -b master https://github.com/raspberrypi/pico-sdk.git
$ cd pico-sdk
$ git submodule update --init
```
```
# Get the example code
$ cd ..
$ git clone -b master https://github.com/raspberrypi/pico-examples.git
```
Now, we can test all env dev for RP2040:

1. Get cmake ready to build the examples.
```
$ cd pico-examples
$ mkdir build
$ cd build
```
2. Set the path to where the SDK can be found
```
$ export PICO_SDK_PATH=../../pico-sdk
```
3. Get the build directory ready for cmake
```
$ cmake ..
```
4. Build the blink example.
```
$ cd blink
$ make -j4
...
[100%] Linking CXX executable blink.elf
[100%] Built target blink
```
3. You should now have a blink.uf2 file inside the build/blink directory

#### OpenOCD

1. Install dependencies
```
$ brew install libtool automake libusb wget pkg-config gcc texinfo
```
2. Add texinfo on your PATH:
```
$ echo 'export PATH="/usr/local/opt/texinfo/bin:$PATH"' >> ~/.zshrc
```
3. Download source code and build OpenOCD
```
$ git clone https://github.com/raspberrypi/openocd.git --branch picoprobe --depth=1
$ cd openocd
$ ./bootstrap
$ ./configure --enable-picoprobe --disable-werror
$ make -j4
$ make install
```
4. Run OpenOCD to verify it was built and installed
```
$ openocd
Open On-Chip Debugger 0.10.0+dev-g18b4c35-dirty (2021-07-07-09:58)
...
Error: Debug Adapter has to be specified, see "adapter driver" command
...
```

#### Picoprobe

Build picoprobe from source.
```
$ git clone https://github.com/raspberrypi/picoprobe.git
$ cd picoprobe
$ mkdir build
$ cd build
# You might need to set the PICO_SDK_PATH again if you closed the terminal at some point
# export PICO_SDK_PATH=../../pico-sdk
$ cmake ..
$ make -j4
...
[100%] Linking CXX executable picoprobe.elf
[100%] Built target picoprobe
```

You should now have a filed called picoprobe.uf2 inside the build directory.
Hold down BOOTSEL and connect the Pico to your Mac. The drive RPI-RP2 should be mounted.
Drag the picoprobe.uf2 file onto the drive.

#### Wire up the two Picos

See Appendix A: Using Picoprobe from the official getting started guide for the wiring diagram.

(Image from the official guide)
![alt text](https://andrejacobs.org/wp-content/uploads/2021/07/Pico2.png)


#### Visual Studio Code & RP2040

Execute the following command in your terminal:
```
$ brew install visual-studio-code
```
To remove it, use brew uninstall:
```
$ brew uninstall visual-studio-code
```

Now would be a good time to add PICO_SDK_PATH to your shell’s path since Visual Studio Code will need this.
```
$ vim .zshrc 

# Add Homebrew's executable directory to the front of the PATH
export PATH=/usr/local/sbin:$PATH
export PATH="/usr/local/opt/texinfo/bin:$PATH"
export PICO_SDK_PATH=~/Dev/pico-sdk
```
Click on the Extensions icon in the sidebar or ⌘⇧X.
* Install the CMake Tools extension made by Microsoft. 
* Install Cortex-Debug by marus25.
* Install C/C++ from Microsoft.

Configure toolchain, steps:
1. Open the project example directory
2. Configure CMake Tools. Click Yes.
3. Select the arm-none-eabi option.

#### Only for test 

Now, create the launch.json and settings.json files inside the .vscode directory of your project. 
Since I am using the pico-examples directory as my project, I will create the files in there.
```
$ cd <YOUR_PATH>/pico-examples
$ mkdir .vscode
# The example repo comes with example settings
$ cp ~/Dev/pico-examples/ide/vscode/launch-raspberrypi-swd.json .vscode/launch.json
$ cp ~/Dev/pico-examples/ide/vscode/settings.json .vscode/settings.json
```

Edit the file .vscode/launch.json and make the following changes.
```
"gdbPath" : "arm-none-eabi-gdb",
"configFiles": [
    "interface/picoprobe.cfg",
    "target/rp2040.cfg"
],
```

Close and reopen Visual Studio Code and open the project (in my case the pico-examples directory).
Ensure the Picoprobe + Pico is connected to your Mac.
Click the green play icon from the Run and Debug section. Select the program to run (e.g. blink)

![alt text](https://andrejacobs.org/wp-content/uploads/2021/07/Pico12.png)

The debugger will stop on main() function and you can now step through the code.

![alt text](https://andrejacobs.org/wp-content/uploads/2021/07/Pico13.png)


### Compile and Run (MacOS env for RP2040)
On your Terminal: 
```
$ git clone https://github.com/VashTheProgrammer/Skeleton/
```
go on VSC, open Skeleton folder, setup the project with GCC arm toolchain and ...
that's it!

## Linux

### Requirements
```
$ apt install texinfo build-essential manpages-dev make
```
Miss information, please!

### Compile and Run (Linux)
On your Terminal:
```
git clone https://github.com/VashTheProgrammer/Skeleton/
cd Skeleton
mkdir build
cd build
cmake -D_BUILD_TYPE="linux" ..
make
./skeleton
```

## How does it work ?

### Conventions
Function returns (int):
 - (0)   -> Success
 - (-1)  -> Error
<br/><br/>

### Serial (minicom)

For MacOS:
```
$ minicom -D /dev/tty.usbmodemXXXXX -b 115200
```
Exit to minicom: esc + z

### Main (main.c)

**lib: main.c and main.h**

It can be used to test all skeleton components.
Inside you will find 2 tasks with two different periods, one pushes and the other pops on a circular test buffer and it is possible to debug through the logger.
The comments explain how the library APIs work.
All API is masked by DEFINE Macro.
<br/><br/>

### Scheduler

**lib: scheduler.c and include/scheduler.h**

API:
```c
// name task, period is milliseconds, function call - return pid
int PROCESS_ATTACH(char *name, uint32_t period, void *function); 

// pid of the task - return successful or not
int PROCESS_DETACH(uint16_t pid);

// return successful or not - contains while(1)
int RUN();
```

Tricks: 
```c
usleep(MIN_TIMESLOT_NS); // 1 millisecond for CPU relax into "scheduler()"
```
<br/>

### Logger

**lib: logger.c and include/logger.h**

API:
```c
#define LOGGER_LEVEL_TRACE          LOW
#define LOGGER_LEVEL_DEBUG          ...
#define LOGGER_LEVEL_INFO           ...
#define LOGGER_LEVEL_WARNING        ...
#define LOGGER_LEVEL_ERROR          ...
#define LOGGER_LEVEL_FATAL          HIGH

// file name and path, logger level start, stdout enable/disable - return successful or not, [MUST] put on the main() head 
int LOGGER_INIT("test.log", LOGGER_LEVEL_DEBUG, LOGGER_STDOUT_ON);

// using like classic printf - return successful or not
int LOGGER_ERROR("color error\n");
int LOGGER_WARN("color warning\n");
int LOGGER_INFO("color infomation\n");
int LOGGER_DEBUG("color debug\n");

// close file, auto flush is always present for every logger raw - return successful or not
int LOGGER_CLOSE();
```
<br/>

### Circular Buffer
**lib: cbuf.c and include/cbuf.h**

API:
```c
// Defines a global circular buffer `buf` of a given type and size. The type
// can be native data types or user-defined data types.
// Usage:
// CIRC_GBUF_DEF(uint8_t, byte_buf, 13);
// CIRC_GBUF_DEF(struct foo, foo_buf, 10);
int CIRC_GBUF_DEF(type, buf, size);

// Resets the circular buffer offsets to zero. Does not clean the newly freed slots. 
int CIRC_GBUF_FLUSH(buf);

// Pushes element pointed to by `elem` at the head of circular buffer `buf`.
// This is read-write method, occupancy count increases by one.
int CIRC_GBUF_PUSH(buf, elem)  _push_refd(elem);

// Copies the element at tail of circular buffer `buf` into location pointed
// by `elem`. This method is read-only, does not later occupancy status. 
int CIRC_GBUF_POP(buf, elem)  _pop_refd(elem);

// Returns the number of free slots in the circular buffer `buf` - return: number of slots available.
int CIRC_GBUF_FS(buf)   __circ_gbuf_free_space(&buf);

// OPTIONS:
// Zero slots in circular buffer after a pop.
int CRICBUF_CLEAN_ON_POP
```
<br/>

### JSON Config

```c
// file name and path, config JSON File to read - return successful or not
int CONFIG_INIT("pathfile");

// put the key and read the value from JSON File
int CONFIG_READ(const char *key, char *value);

```
<br/>

## TIPS

#### Visual Studio Code
CMD+SHIFT+V -> .MD Preview on/off
Probably need to install something

#### Uninstalling Homebrew
If you no longer need Homebrew, you can use its uninstall script.

Download the uninstall script with curl:
```
curl -fsSL -o uninstall.sh https://raw.githubusercontent.com/Homebrew/install/master/uninstall.sh
``` 
As always, review the contents of the script with the less command to verify the script’s contents:
```
less uninstall.sh
``` 
Once you’ve verified the script, execute the script with the --help flag to see the various options you can use:
```
bash uninstall.sh --help
bash uninstall.sh -d
```
When you’re ready to remove everything, execute the script without any flags:
```
bash uninstall.sh
```

# Have fun!
REM said, remember, the ticket to the future is always blank.<br/>
Vash