cd ~/Coding/personal/cs449/project4
make
make clean
cp dice_module.c ~/Documents
cd ~/Documents
make clean
make
sudo rmmod dice_module
sudo insmod dice_module.ko
cd ~/Coding/personal/cs449/project4
