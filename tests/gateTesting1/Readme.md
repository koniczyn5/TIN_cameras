Test zaniku napięcia na bramce.
W celu przeprowadzenia testu należy, po skomplowaniu:
1)W jednym oknie terminala uruchomić kamerę np. ./camera2 4011 4012 4013
2)W drugim oknie terminala uruchomić bramkę w trybie 1, np. ./gate2 localhost 4001 1
3)Po zakończeniu działania bramki uruchomić ją ponownie w trybie 2, np. ./gate2 localhost 4001 2


Bramka w trybie 1 wykonuje procedurę instalacyjną i konfiguracyjną.
Bramka w trybie 2 nasłuchuje nadesłania jednego pliku.
Kamera nasłuchuje 2 ządań, po czym w pętli wysyła plik.
