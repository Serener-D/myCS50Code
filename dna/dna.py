import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Missing command-line arguments")
        sys.exit(1)

    # TODO: Read database file into a variable
    dnaPatternsList = list()
    suspectsDict = dict()
    with open(sys.argv[1]) as csvfile:
        reader = csv.reader(csvfile, delimiter=' ')
        for row in reader:
            currentRowlist = row[0].split(",")
            if reader.line_num == 1:
                dnaPatternsList = currentRowlist
                dnaPatternsList.pop(0)
                continue
            suspectsDict.update({currentRowlist.pop(0): currentRowlist})

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    dnaMatches = list()
    for pattern in dnaPatternsList:
        dnaMatches.append(str(longest_match(sequence, pattern)))

    result = "No match"
    # TODO: Check database for matching profiles
    for suspect in suspectsDict:
        if suspectsDict[suspect] == dnaMatches:
            result = suspect

    print(result)
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
