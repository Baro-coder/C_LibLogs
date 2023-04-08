#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

check_result() 
{
    if [[ $? -eq 0 ]]; then
        return 1
    else
        return 0
    fi
}

cd ./logs
if check_result; then
    echo "Something went wrong!"
    exit 1
fi

echo "Baro-coder : C : liblogs : installing..."

# Compile sources
echo "  * Compiling sources..."

sudo gcc -c ${SCRIPT_DIR}/logs/logs.c
if check_result; then
    echo "Something went wrong!"
    exit 1
fi

# Making library
echo "  * Making library..."

sudo ar crs ./liblogs.a ./logs.o
if check_result; then
    echo "Something went wrong!"
    exit 1
fi

# Header
echo "  * Copying header..."

sudo cp ${SCRIPT_DIR}/logs/logs.h /usr/include/
if check_result; then
    echo "Something went wrong!"
    exit 1
fi

# Link: lib
echo "  * Copying library..."

sudo cp ./liblogs.a /usr/lib/
if check_result; then
    echo "Something went wrong!"
    exit 1
fi

echo "Baro-coder : C : liblogs : Successfully installed."
