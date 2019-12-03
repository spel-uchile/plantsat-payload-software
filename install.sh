#!/usr/bin/env bash
echo "Downloading SUCHAI-FS"
if [ ! -d "./SUCHAI-Flight-Software" ]; then
  git clone https://github.com/spel-uchile/SUCHAI-Flight-Software.git
fi

cd SUCHAI-Flight-Software
echo "Updating SUCHAI-FS"
git pull
build_cmd="LINUX RPI --name PlantSat-Chamber --id 6 --hk 0 --node 6 --st_mode 2 --drivers"
echo "Configure (and build) SUCHAI-FS using:"
echo $build_cmd
python3 compile.py $build_cmd
cd -