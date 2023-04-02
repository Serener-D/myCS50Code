from cs50 import get_int


while True:
    height = int(get_int("Height: "))
    if height > 0 and height < 9:
        break

for number in range(1, height + 1):
    print(" " * (height - number), end="")
    print("#" * number, end="")
    print(" " * 2, end="")
    print("#" * number, end="")
    print()