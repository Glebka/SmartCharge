# Smart Charger Device

## How to build on Windows

1. Install Arduino IDE
2. Run the following command in CMD:

        build.bat <booard_name>

    or in PowerShell:

        .\build.bat <board_name>

Where ``<board_name>`` is a file name from ``/boards`` folder without extension.

For more information about adding new boards please read the [Arduino CMake Toolchain documentation](https://github.com/a9183756-gh/Arduino-CMake-Toolchain)


## How to send commands from PC to device?

1. Install Python. I tested it with Python 2.7, but Python 3.x should be fine as well
2. Install PySerial library
   
    pip install pyserial

3. Connect your device to the PC
4. Run the following in CMD:
   
        cd tools\commander
        python -i commander.py <COM_PORT> <BAUD_RATE>

Example:

    python -i commander.py COM7 9600

You will get into the interactive python console, where `dev` object is available.

That object represents the SmartCharge device.
At first read all the data from COM port buffer if any:

    dev.read_all()

Then, you can play with device:

    >>> dev.get_state()
    Resp: '\x07\x00\x00\x00'
    State: '\x00\x00\x00\x00'
    >>> dev.get_version()
    Resp: '\x07\x10\x00\x00'
    >>> dev.charge_enable()
    Resp: '\x07\x00\x00\x00'
    >>> dev.charge_disable()
    Resp: '\x07\x00\x00\x00'
    >>> exit()

