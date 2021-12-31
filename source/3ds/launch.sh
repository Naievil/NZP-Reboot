rm -rf ~/.local/share/citra-emu/sdmc/3ds/nzportable
rsync -avzP ../../3ds/nzportable/ ~/.local/share/citra-emu/sdmc/3ds/nzportable/
sleep 1

/usr/local/bin/citra ~/.local/share/citra-emu/sdmc/3ds/nzportable/nzportable.3dsx
