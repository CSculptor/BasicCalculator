### BASIC CALCULATOR USING KEYPAD : 

- This project involves a basic calculator using the PIC18F452 microcontroller, an LCD display, and keypad. 
- The calculator supports basic arithmetic operations (+, -, *, /). 



https://github.com/LatifEmbedded/Keypad-Lcd-Calculator/assets/155848361/107a886e-3018-45b5-9d57-aa237c8a4979


| operation | state |
| --------- | ----- |
| a   +   b | allowed |
| a   -  b  | allowed |
| a   * b   | allowed |
| a   /  b | allowed |
| -a  /  b | not allowed |
| +a  -  b | allowed |
| -a  *  b | allowed |
| -a  -  b | allowed |
| -a  +  b | allowed |


### NOTE : 
- The maximum input number allowed is 255, other then an error is displayed.
- One operation at a time for example (a+b+c), (a-b+c), (a-b-c) is not allowed.
