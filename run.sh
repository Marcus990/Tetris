#!/bin/bash
# run.sh - Script to start XQuartz and run Tetris

echo "======================================"
echo "   Tetris with XQuartz Graphics"
echo "======================================"
echo ""

# Check if XQuartz is installed
if [ ! -d "/Applications/Utilities/XQuartz.app" ]; then
    echo "ERROR: XQuartz is not installed!"
    echo "Please install from: https://www.xquartz.org/"
    exit 1
fi

# Check if XQuartz is running
if ! pgrep -x "X11" > /dev/null 2>&1; then
    echo "Starting XQuartz..."
    open -a XQuartz
    echo "Waiting for XQuartz to start (5 seconds)..."
    sleep 5
else
    echo "XQuartz is already running ✓"
fi

# Set DISPLAY variable (required for X11)
export DISPLAY=:0

# Check if program is compiled
if [ ! -f "./tetris" ]; then
    echo ""
    echo "Program not compiled yet. Compiling..."
    make clean
    make
    
    if [ $? -ne 0 ]; then
        echo ""
        echo "ERROR: Compilation failed!"
        echo "Please check the error messages above."
        exit 1
    fi
fi

echo ""
echo "Starting Tetris..."
echo ""
echo "Command line options:"
echo "  -text           : Run in text-only mode (no graphics)"
echo "  -seed N         : Set random seed to N"
echo "  -startlevel N   : Start at level N"
echo "  -scriptfile1 F  : Use file F as script for player 1"
echo "  -scriptfile2 F  : Use file F as script for player 2"
echo ""

# Run the program with any provided arguments
./tetris "$@"

