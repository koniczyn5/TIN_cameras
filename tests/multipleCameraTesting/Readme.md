Test jednoczesnego przesyłania zdjęć z wielu kamer do bramki.
W celu przeprowadzenia testu należy, po skompilowaniu:
1)W jednym oknie terminala uruchomić bramkę, z folderu "gate" poleceniem np. ./gate3 localhost 4001
2)W drugim oknie terminala uruchomić skrypt ./runCameras.sh
3)Po zakończeniu działania skryptu, można zakończyć działanie bramki i sprawdzić, czy w folderze "gate" znajduje się 5 kopii zdjęcia.
