def countLuhn(number):
    digitsSum = 0
    for i in range(len(number) - 2, -1, -2):
        for digit in str(int(number[i]) * 2):
            digitsSum += int(digit)

    for i in range(len(number) - 1, -1, -2):
        digitsSum += int(number[i])
    return digitsSum


number = input("Number: ")
luhnNumber = countLuhn(number)

if luhnNumber % 10 != 0:
    print("INVALID")
elif number[0] == "3" and number[1] == "7" and len(number) == 15:
    print("AMEX")
elif number[0] == "5" and (number[1] == "1" or number[1] == "5") and len(number) == 16:
    print("MASTERCARD")
elif number[0] == "4" and (len(number) == 13 or len(number) == 16):
    print("VISA")
else:
    print("INVALID")