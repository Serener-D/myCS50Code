#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
static int cmppair(const void *pair1, const void *piar2);
bool isCycleCreated(int winner, int loser, pair lockedPairs[], int lockedPairsCount);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    bool candidateFound = false;
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            candidateFound = true;
            break;
        }
    }
    return candidateFound;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        int currentCandidateIndex = ranks[i];
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[currentCandidateIndex][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pair pair = {i, j};
                pairs[pair_count] = pair;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    qsort(pairs, pair_count, sizeof(pair), cmppair);
    return;
}

static int cmppair(const void *pair1, const void *piar2)
{
    pair *pairA = (pair *) pair1;
    pair *pairB = (pair *) piar2;
    if (preferences[pairA->winner][pairA -> loser] < preferences[pairB -> winner][pairB -> loser])
    {
        return 1;
    }
    else if (preferences[pairA -> winner][pairA -> loser] > preferences[pairB -> winner][pairB -> loser])
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    pair lockedPairs[pair_count];
    int lockedPairsCount = 0;
    bool isCandidateWinnerArray[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        isCandidateWinnerArray[i] = false;
    }
    for (int i = 0; i < pair_count; i++)
    {
        if (isCandidateWinnerArray[pairs[i].loser] == false
            || !isCycleCreated(pairs[i].winner, pairs[i].loser, lockedPairs, lockedPairsCount))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            isCandidateWinnerArray[pairs[i].winner] = true;
            lockedPairs[lockedPairsCount] = pairs[i];
            lockedPairsCount++;
        }
    }
    return;
}

bool isCycleCreated(int winner, int loser, pair lockedPairs[], int lockedPairsCount)
{
    for (int i = 0; i < lockedPairsCount; i++)
    {
        if (lockedPairs[i].loser == winner)
        {
            return true;
        }
        if (lockedPairs[i].winner == loser)
        {
            return isCycleCreated(lockedPairs[i].winner, lockedPairs[i].loser, lockedPairs, lockedPairsCount);
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool isSource = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                isSource = false;;
            }
        }
        if (isSource)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}