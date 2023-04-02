text = input("Text: ")

wordCount = 1
sentencesCount = 0
lettersCount = 0
for character in text:
    if character == " ":
        wordCount += 1
    elif character == "." or character == "!" or character == "?":
        sentencesCount += 1
    elif ord(character.lower()) > 92 and ord(character.lower()) < 132:
        lettersCount += 1

avgLetters = (lettersCount / wordCount) * 100
avgSentences = (sentencesCount / wordCount) * 100
grade = round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8)

if (grade > 16):
    print("Grade 16+")
elif (grade < 1):
    print("Before Grade 1")
else:
    print("Grade", grade)