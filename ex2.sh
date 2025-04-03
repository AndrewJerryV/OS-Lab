#!/bin/bash
while true
do
    echo "1) Addition"
    echo "2) Subtraction"
    echo "3) Multiplication"
    echo "4) Division"
    echo "5) Modulus"
    echo "6) Quit"
    echo -n "Enter choice: "
    read choice
    case $choice in
        1)
            echo -n "Enter two numbers: "
            read num1 num2
            echo "Sum: $((num1 + num2))"
            ;;
        2)
            echo -n "Enter two numbers: "
            read num1 num2
            echo "Difference: $((num1 - num2))"
            ;;
        3)
            echo -n "Enter two numbers: "
            read num1 num2
            echo "Product: $((num1 * num2))"
            ;;
        4)
            echo -n "Enter two numbers: "
            read num1 num2
            if [ $num2 -ne 0 ]; then
                echo "Quotient: $((num1 / num2))"
            else
                echo "Error: Division by zero is not allowed."
            fi
            ;;
        5)
            echo -n "Enter two numbers: "
            read num1 num2
            echo "Remainder: $((num1 % num2))"
            ;;
        6)
            break
            ;;
        *)
            echo "Invalid choice!"
            ;;
    esac
    echo ""
done
