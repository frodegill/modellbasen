#!/bin/sh

sudo apt-get install g++ make libwthttp-dev libboost-locale-dev mysql-server libmysqlclient-dev unixodbc libmyodbc unixodbc-dev
echo "Depends on Poco 1.6.1+. Download, compile and install the Complete Edition from http://pocoproject.org/"

#Create database
echo "When asked for password, use sudo if applicable, then MySQL admin password"
sudo mysql -u root -p < ./prepare_modellbasen.sql
echo "When asked for password, use MySQL modellbasen password"
sudo mysql -u modellbasen -p modellbasen < ./create_modellbasen.sql

#Set up unixodbc (system) driver and (user) datasource
sudo odbcinst -i -l -d -f /usr/share/libmyodbc/odbcinst.ini
odbcinst -i -h -s -f ./datasource.template

ln -s /usr/share/Wt/resources
make
