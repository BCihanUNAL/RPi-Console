#!/bin/bash

FTP_PKG="vsftpd"

if [[ "" = $(dpkg-query -W --showformat='${Status}\n' $FTP_PKG|grep "install ok installed") ]];
then
	
	echo "ftp sunucusu kuruluyor"
	sudo apt-get install $FTP_PKG -y
	
fi

USER="pi"

if [[ ! -e /home/${USER}/ftp ]]
then
	mkdir /home/${USER}/ftp
fi

if [[ ! -e /home/${USER}/ftp/files ]]
then
	mkdir -p /home/${USER}/ftp/files
fi

chmod a-w /home/pi/ftp

if [[ "#####" != $(tail -1 /etc/vsftpd.conf) ]];
then
	
	sudo echo "anonymous_enable=NO" | sudo tee -a /etc/vsftpd.conf
	sudo echo "local_enable=YES" | sudo tee -a /etc/vsftpd.conf
	sudo echo "write_enable=YES" | sudo tee -a /etc/vsftpd.conf
	sudo echo "local_umask=022" | sudo tee -a /etc/vsftpd.conf
	sudo echo "chroot_local_user=YES" | sudo tee -a /etc/vsftpd.conf
	
	sudo echo "user_sub_token=${USER}" | sudo tee -a /etc/vsftpd.conf
	sudo echo "local_root=/home/${USER}/ftp" | sudo tee -a /etc/vsftpd.conf
	sudo echo "#####" | sudo tee -a /etc/vsftpd.conf

fi

sudo service vsftpd restart
