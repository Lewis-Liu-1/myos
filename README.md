# myos

Day 1
Install Ubuntu 18

install gcc

download https://github.com/ganeshredcobra/Mini2440/blob/master/FriendlyARM/arm-linux-gcc-4.3.2.tgz

according http://www.friendlyarm.net/forum/attachment/9150


sudo –s2
.# mkdir /usr/local/arm3
.# mkdir /user/local/arm/4.3.24
.# cd /tmp5
.# tar xvzf arm-linux-gcc-4.3.2.tgz -C/(with a capital ‘C’

gedit ~/.bashrc

export PATH=$PATH:/usr/local/arm/4.3.2/bin

Github setup

ssh-keygen -t rsa -b 4096 -C "yyiu002@hotmail.com"
ssh-add ~/.ssh/id_rsa
sudo apt-get install xclip
xclip -sel clip < ~/.ssh/id_rsa.pub

From github settings, copy public key to keys area. Add

git push -u origin master





