### BASIC CALCULATOR USING KEYPAD : 

This project involves a basic calculator using the PIC18F452 microcontroller, an LCD display, and a keypad. 
The calculator supports basic arithmetic operations (+, -, *, /) and only perform one operation at a time. 
The maximum input number allowed is 255.



https://github.com/LatifEmbedded/Keypad-Lcd-Calculator/assets/155848361/107a886e-3018-45b5-9d57-aa237c8a4979
| OPERATION | STATE |
| --------- | ----- |
| a   +   b | allowed |
| --------- | ----- |
| a   -  b  | allowed |
| --------- | ----- |
| a   * b   | allowed |
| --------- | ----- |
| a    /  b | allowed |
| --------- | ----- |
| + a  -  b | not allowed |
| --------- | ----- |
| - a  -  b | not allowed |
| --------- | ----- |
| - a  +  b | not allowed |
