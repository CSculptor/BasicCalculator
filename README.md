### BASIC CALCULATOR USING KEYPAD : 

- This project involves a basic calculator using the PIC18F452 microcontroller, an LCD display, and keypad. 
- The calculator supports basic arithmetic operations (+, -, *, /). 




https://github.com/LatifEmbedded/Keypad-Lcd-Calculator/assets/155848361/57b4c48a-995b-4557-a5d6-a168d50b1416



| operation| state |
| -------- | ----- |
| a   +  b | allowed |
| a   -  b | allowed |
| a   * b  | allowed |
| a   /  b | allowed |
| -a  /  b | allowed |
| -a  *  b | allowed |
| -a  -  b | allowed |
| -a  +  b | allowed |


### NOTE : 
- The maximum input number allowed is 255, other then an error is displayed.
- One operation at a time for example (a+b+c), (a-b+c), (a-b-c) is not allowed.
