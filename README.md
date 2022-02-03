# ESP32_Password_keyboard
Bluetooth password keyboard using the ESP32

Versions:

1.0 February 1st, 2022 - First public version


Description:

This project uses a ESP32, 4x4 matrix keyboard, an SD card reader, and a RGB LED.
The goual is to create a macro / password keyboard with 8 banks of 15 key mappings.

The star (*) key is used for shifting trought the key assignemnt banks.

Each bank has a corresponding file on the SD card.
The file should have 15 lines with macro sequences, and at least one blank line at the end.
The first line corresponds to the key "1", then 2 trough D, and the last is sequence for the key "#".

------------------------------------------------------------------------------------------------------
Examples for macro sequences:

1) just a plain text:
somekeycombination

2) text with tab key at the end
username\t

3) text with enter key at the end:
password\n

or

password\r\n

4) text that has slash character ( exam\ple )
exam\\ple

------------------------------------------------------------------------------------------------------

Here is the list of available special characters:

\n - adds ASCII char 10 - line feed
\r - adds ASCII char 13 - carriage return
\t - adds ASCII char 9 - horizontal tab
\e - adds ASCII char 27 - escape
\b - adds ASCII char 8 - backspace
\d - adds ASCII char 127 - delete

------------------------------------------------------------------------------------------------------
