cd ~/Coding/personal/cs449/project4
make
make clean
cp dice_dev.c ~/Documents
cd ~/Documents
make clean
make
sudo rmmod dice_dev
sudo insmod dice_dev.ko
cd ~/Coding/personal/cs449/project4
