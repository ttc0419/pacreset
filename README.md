# Pacreset
A recovery program to reset Arch Linux package file permissions

## Usage
```shell
# Reset all files and directories owned by installed packages
pacreset

# Only reset files and directories under the specified file/directory
pacreset /path/to/file/or/directory
```

## Reset permissions using bootable USB
If you cannot boot Arch Linux because of file permissions, you need to make a bootable USB drive.
Then, you need to copy the program to the root partition of Arch Linux on your computer.
Mount the root partition and then ```arch-chroot``` to that partition.
Finally, run the ```pacreset``` command to reset all files and directories owned by all packages.

## Build
You can download the program from the "Packages" page on GitHub.
However, if you want to build the program yourself, first you need to install some dependencies using the following command:
```shell
pacman -S cmake pkgconf ninja gcc
```
Then, you just need to run the build script:
```shell
bash build.sh
```
The program is located at build/pacreset

## Sponsor
It takes a lot of time to create and maintain a project.  If you think it helped you, could you buy me a cup of coffee? 😉  
You can use any of the following methods to donate:

| [![PayPal](/res/paypal.svg)](https://www.paypal.com/paypalme/tianchentang)<br/>Click [here](https://www.paypal.com/paypalme/tianchentang) to donate | ![Wechat Pay](/res/wechat.jpg)<br/>Wechat Pay | ![Alipay](/res/alipay.jpg) Alipay |
|---------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------|---------------------------------------|
