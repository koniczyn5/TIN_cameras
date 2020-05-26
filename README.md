# TIN_cameras
Treść zadania:

System komunikacji pomiędzy bramą radiową a zbiorem kamer wykonujących zdjęcia.
Kamery wysyłają do bramy zdjęcia zgodnie z ustawionymi parametrami (rozdzielczość, ogniskowa, okres i zmienność opóźnienia «jitter»). Zaproponować procedury instalacyjne, testowe i konfiguracyjne. W warstwie transportowej używamy wyłącznie UDP. Niezawodność obsługiwana jest w warstwie aplikacji. Należy zaprojektować moduł do Wireshark umożliwiający wyświetlanie i analizę zdefiniowanych komunikatów.

Instrukcja uruchamiania:
Kamery: ./camera nr-portu-ipv4 nr-portu-ipv6 nr-portu-nadajacego
Bramki: ./gate adres-serwera nr-portu-kamery
