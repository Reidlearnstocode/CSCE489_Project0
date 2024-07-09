# :wave: C / C ++ Project 0 Code 

# This is a simple interest Calculator

Given a borrowed amount, repayment term (in years), and interest rate, the calculator will display a table with estimated monthly payments given the terms of the loan.

To use this calculator take the following steps:

1. Create a main function that calls HessmanGovernmentMortgageCalculator

2. Input your desired loan amount (assigned as double borrowedAmount)
3. Select desired loan repayment period(s) you'd like displayed (Entered/assigned in[] yearArray)
4. Input your estimated interest rates (Entered/assigned in double[] rateArray as a double floating point - for example a 5% interest rate is entered as "0.05")

*Note* For your convenience a main function exists, and you can simply add, or modify the current values for borrowedAmount, yearArray, and rateArray and run the function.

# Under the hood: 🚙☁️☁️☁️

This calculator utilizes a standard simple interest calculation:

![image](https://github.com/Reidlearnstocode/CSCE489_Project0/assets/169181658/0b14f360-8ad1-4988-acc6-e6eb9e0c07d9)

Given the final amount, "A" (borrowed amount plus total interest), it then calculates a monthly payment by dividing the final amount "A" by the number of months in the repayment period.

See comments in the program for additional information.

If you have any questions, comments, or concerns, please feel free to reach out at anytime to reidlearnstocode @ github.com
