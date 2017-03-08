#!/bin/bash
sudo apt-get update
sudo apt-get -y --force-yes install python python-pip python-crypto python-dev avrdude libgmp3-dev build-essential python-numpy python-matplotlib python-wheel libjpeg-dev < /dev/null
sudo apt-get -y --force-yes remove python-pil < /dev/null
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/firmwareBundlePackAndSign.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/generic_hid_device.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_defines.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_hid_device.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_init_proc.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_security_check.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_single_prog.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_mass_prog.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_tool.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/publickey.bin
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/requirements.txt
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/updatefile.img
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/mooltipass_mass_prod_init_proc.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/png_labels.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/generate_prog_file.py
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/FreeSans.ttf
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/_mass_prog.sh
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/tools/_python_framework/_prod_init.sh
wget -N https://raw.githubusercontent.com/limpkin/mooltipass/master/bitmaps/mini/bundle.img
sudo chmod +x _mass_prog.sh
sudo chmod +x _prod_init.sh
sudo pip install -r requirements.txt
sudo pip install seccure
sudo pip install https://github.com/pklaus/brother_ql/archive/7a3c638.zip
sudo pip install --upgrade --no-deps https://github.com/pklaus/brother_ql/archive/7a3c638.zip
mkdir -p export
sudo rm _install_reqs.sh