#!/bin/bash
echo "Build"
mkdir -p ../build
cd ../build
cmake ..
make
cd -
echo "Coping executable"
systemctl --user stop suchai-fs.service
mkdir -p ~/bin
cp ../build/PlantSat-Payload ~/bin

echo "Coping service file"
mkdir -p ~/.local/share/systemd/user
cp suchai-fs.service ~/.local/share/systemd/user

echo "Updating service"
systemctl --user daemon-reload
#systemctl --user enable suchai-fs.service
systemctl --user restart suchai-fs.service
systemctl --user status suchai-fs.service
