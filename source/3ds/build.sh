make clean
make

rsync -avzP *.3dsx ~/.local/share/citra-emu/sdmc/3ds/nzportable/
rsync -avzP *.bin ~/.local/share/citra-emu/sdmc/3ds/nzportable/
rsync -avzP *.smdh ~/.local/share/citra-emu/sdmc/3ds/nzportable/
