#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*We will take entries for an year so we limit max entries to 365*/
#define maxentries 365
#define lowmood 4
/* declaring all the structs for user entry */
struct entry
{
    char date[11];
    float water;
    float sleep;
    int steps;
    int exercise;
    int mood;
};

struct profile
{
    int age;
    char gender;
    float watergoal;
    float sleepgoal;
    int stepsgoal;
    int exercisegoal;
};
/*Creating a struct array for entries*/
struct entry entries[maxentries];
/*Counter to know how many entries are actually being used*/
int entrycount = 0;
struct profile user;
/*names of the categories we are storing*/
const char *namesofcategories[4] = {"Water (L)", "Sleep (hrs)", "Steps", "Exercise (min)"};

/*Validating the date entered*/
int validdate(const char *text)
{
    int i, year, month, day, maxday;
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    /* must be YYYY-MM-DD length */
    if (strlen(text) != 10)
        return 0;
    /* check dashes and digit characters */
    for (i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            if (text[i] != '-')
                return 0;
        }
        else if (text[i] < '0' || text[i] > '9')
            return 0;
    }
    /* extract year, month, day */
    year = atoi(text);
    month = atoi(text + 5);
    day = atoi(text + 8);
    if (year < 2000 || year > 2100 || month < 1 || month > 12)
        return 0;
    maxday = days[month - 1];
    /* adjust February for leap years */
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))
        maxday = 29;
    if (day < 1 || day > maxday)
        return 0;
    return 1;
}
/*function for finding a particular days entry*/
int finddate(const char *date)
{
    int i;

    /* scan existing entries for date match */
    for (i = 0; i < entrycount; i++)
        if (strcmp(entries[i].date, date) == 0)
            return i;
    return -1;
}
/*finding the users age bracket*/
const char *agebracket()
{
        /* check age thresholds */
        if (user.age < 18)
        return "under 18";
    if (user.age < 40)
        return "18-39";
    if (user.age < 65)
        return "40-64";
    return "65 and above";
}

/* return readable gender label */
const char *gendername()
{
    if (user.gender == 'M')
        return "male";
            return "female";
}
/*applying defaults based on a users age*/
void applydefaults()
{
    /* youth defaults */
    if (user.age < 18)
    {
        user.sleepgoal = 9;
        user.stepsgoal = 12000;
        user.exercisegoal = 60;
        user.watergoal = 1.8;
    }
    else
    {
        /* adult step and sleep targets */
        if (user.age < 40)
        {
            user.sleepgoal = 8;
            user.stepsgoal = 10000;
        }
        else if (user.age < 65)
        {
            user.sleepgoal = 8;
            user.stepsgoal = 8000;
        }
        else
        {
            user.sleepgoal = 7.5;
            user.stepsgoal = 6000;
        }
        user.exercisegoal = 30;
        /* water target by gender */
        if (user.gender == 'M')
            user.watergoal = 3.0;
        else
            user.watergoal = 2.2;
    }
}
/* editing and changing the users profile */
void saveprofile()
{
    /* write profile values to disk */
    FILE *fp = fopen("profile.txt", "w");

    if (fp == NULL)
    {
        printf("Could not save profile.\n");
        return;
    }
    fprintf(fp, "%d %c %.2f %.2f %d %d\n", user.age, user.gender, user.watergoal, user.sleepgoal, user.stepsgoal, user.exercisegoal);
    fclose(fp);
}
/* loading a users profile every time the program runs*/
int loadprofile()
{
    /* read saved profile if present */
    FILE *fp = fopen("profile.txt", "r");
    int result;

    if (fp == NULL)
        return 0;
    result = fscanf(fp, "%d %c %f %f %d %d", &user.age, &user.gender, &user.watergoal, &user.sleepgoal, &user.stepsgoal, &user.exercisegoal);
    fclose(fp);
    return result == 6;
}
/* printing a users goals*/
void showgoals()
{
    /* display each target */
    printf("   Water:    %.2f liters\n", user.watergoal);
    printf("  Sleep:    %.1f hours\n", user.sleepgoal);
    printf("  Steps:    %d\n", user.stepsgoal);
    printf("  Exercise: %d minutes\n", user.exercisegoal);
}
/*function used for setting up users profile the first time*/
void setupprofile()
{
    char ch;
    int valid, r, c, intvalue;
    float floatvalue;

    /* prompt and validate age */
    valid = 0;
    while (!valid)
    {
        printf("Enter your age ( 5-100): ");
        r = scanf("%d", &intvalue);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (r == 1 && intvalue >= 5 && intvalue <= 100)
            valid = 1;
        else
            printf("Invalid input. Enter a whole number between 5 and 100.\n");
    }
    user.age = intvalue;

    /* prompt and validate gender */
    valid = 0;
    while (!valid)
    {
        printf("Enter your gender (M/F): ");
        r = scanf(" %c", &ch);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (ch == 'M' || ch == 'm')
        {
            user.gender = 'M';
            valid = 1;
        }
        else if (ch == 'F' || ch == 'f')
        {
            user.gender = 'F';
            valid = 1;
        }
        else
            printf("Invalid input. Enter M or F.\n");
    }
/*shows the users if defaults need to be applied upon profile entry or need to be changed*/
    applydefaults();
    printf("\nRecommended daily goals for a %s person in the %s age group:\n", gendername(), agebracket());
    showgoals();
    printf("\nCustomise these goals? (y/n): ");
    r = scanf(" %c", &ch);
    if (r == EOF)
        exit(0);
    while ((c = getchar()) != '\n' && c != EOF);
    /* optional custom goal overrides */
    if (ch == 'y' || ch == 'Y')
    {
        /* custom water goal */
        valid = 0;
        while (!valid)
        {
            printf("Water goal in litres (0.5-10): ");
            r = scanf("%f", &floatvalue);
            if (r == EOF)
                exit(0);
            while ((c = getchar()) != '\n' && c != EOF);
            if (r == 1 && floatvalue >= 0.5 && floatvalue <= 10)
                valid = 1;
            else
                printf("Invalid input. Enter a number between 0.5 and 10.\n");
        }
        user.watergoal = floatvalue;

        /* custom sleep goal */
        valid = 0;
        while (!valid)
        {
            printf("Sleep goal in hours (3-14): ");
            r = scanf("%f", &floatvalue);
            if (r == EOF)
                exit(0);
            while ((c = getchar()) != '\n' && c != EOF);
            if (r == 1 && floatvalue >= 3 && floatvalue <= 14)
                valid = 1;
            else
                printf("Invalid input. Enter a number between 3 and 14.\n");
        }
        user.sleepgoal = floatvalue;

        /* custom step goal */
        valid = 0;
        while (!valid)
        {
            printf("Step goal (1000-50000): ");
            r = scanf("%d", &intvalue);
            if (r == EOF)
                exit(0);
            while ((c = getchar()) != '\n' && c != EOF);
            if (r == 1 && intvalue >= 1000 && intvalue <= 50000)
                valid = 1;
            else
                printf("Invalid input. Enter a whole number between 1000 and 50000.\n");
        }
        user.stepsgoal = intvalue;

        /* custom exercise goal */
        valid = 0;
        while (!valid)
        {
            printf("Exercise goal in minutes (5-300): ");
            r = scanf("%d", &intvalue);
            if (r == EOF)
                exit(0);
            while ((c = getchar()) != '\n' && c != EOF);
            if (r == 1 && intvalue >= 5 && intvalue <= 300)
                valid = 1;
            else
                printf("Invalid input. Enter a whole number between 5 and 300.\n");
        }
        user.exercisegoal = intvalue;
    }
    saveprofile();
    printf("\nProfile and goals saved.\n");
}
/*loading previous entires*/
void loadentries()
{
    /* load log rows up to maxentries */
    FILE *fp = fopen("healthlog.txt", "r");

    if (fp == NULL)
        return;
        while (entrycount < maxentries && fscanf(fp, "%10s %f %f %d %d %d", entries[entrycount].date, &entries[entrycount].water, &entries[entrycount].sleep, &entries[entrycount].steps, &entries[entrycount].exercise, &entries[entrycount].mood) == 6)
        entrycount++;
    fclose(fp);
}

/* copy entry metrics into array */
void getmetrics(struct entry e, float out[4])
{
    out[0] = e.water;
    out[1] = e.sleep;
    out[2] = e.steps;
    out[3] = e.exercise;
}
/*getting a users goals*/
void getgoals(float goals[4])
{
    /* copy user goals into array */
    goals[0] = user.watergoal;
    goals[1] = user.sleepgoal;
    goals[2] = user.stepsgoal;
    goals[3] = user.exercisegoal;
}
/*entering the users score*/
float scoreentry(struct entry e, float scores[4])
{
    float metrics[4], goals[4], total = 0;
    int i;

    getmetrics(e, metrics);
    getgoals(goals);
    /* cap each metric at 100 percent */
    for (i = 0; i < 4; i++)
    {
        scores[i] = metrics[i] / goals[i] * 100;
        if (scores[i] > 100)
            scores[i] = 100;
        total += scores[i];
    }
    return total / 4;
}

/* map numeric score to label */
const char *rating(float score)
{
    if (score >= 90)
        return "Excellent";
    if (score >= 70)
        return "Good";
    if (score >= 50)
        return "Fair";
    return "Needs improvement";
}

/* render ASCII bar */
void drawbar(float value, float full, int width)
{
    int filled = (int)(value / full * width);
    int i;

    /* clamp filled length to bar bounds */
    if (filled > width)
        filled = width;
    if (filled < 0)
        filled = 0;
    printf("|");
    for (i = 0; i < width; i++)
    {
        if (i < filled)
            printf("#");
        else
            printf(".");
    }
    printf("|");
}

/* print metric bars and overall score */
void printscores(float scores[4], float overall)
{
    int i;

    for (i = 0; i < 4; i++)
    {
        printf("  %-15s ", namesofcategories[i]);
        drawbar(scores[i], 100, 20);
        printf(" %5.1f%%\n", scores[i]);
    }
    printf("  %-15s ", "Overall");
    drawbar(overall, 100, 20);
    printf(" %5.1f%%  (%s)\n", overall, rating(overall));
}

/* collect, save, and score a new daily entry */
void addentry()
{
    struct entry e;
    char buffer[21];
    float scores[4], overall, floatvalue;
    int valid, r, c, intvalue;
    FILE *fp;

    if (entrycount >= maxentries)
    {
        printf("The log is full.\n");
        return;
    }

    /* prompt for unique valid date */
    while (1)
    {
        printf("Enter date (YYYY-MM-DD): ");
        r = scanf("%20s", buffer);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (!validdate(buffer))
        {
            printf("Invalid date. Use the format YYYY-MM-DD.\n");
            continue;
        }
        if (finddate(buffer) != -1)
        {
            printf("An entry for that date already exists.\n");
            continue;
        }
        break;
    }
    strcpy(e.date, buffer);

    /* water input */
    valid = 0;
    while (!valid)
    {
        printf("Water intake in litres (0-15): ");
        r = scanf("%f", &floatvalue);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (r == 1 && floatvalue >= 0 && floatvalue <= 15)
            valid = 1;
        else
            printf("Invalid input. Enter a number between 0 and 15.\n");
    }
    e.water = floatvalue;

    /* sleep input */
    valid = 0;
    while (!valid)
    {
        printf("Sleep in hours (0-24): ");
        r = scanf("%f", &floatvalue);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (r == 1 && floatvalue >= 0 && floatvalue <= 24)
            valid = 1;
        else
            printf("Invalid input. Enter a number between 0 and 24.\n");
    }
    e.sleep = floatvalue;

    /* steps input */
    valid = 0;
    while (!valid)
    {
        printf("Step count (0-100000): ");
        r = scanf("%d", &intvalue);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (r == 1 && intvalue >= 0 && intvalue <= 100000)
            valid = 1;
        else
            printf("Invalid input. Enter a whole number between 0 and 100000.\n");
    }
    e.steps = intvalue;

    /* exercise input */
    valid = 0;
    while (!valid)
    {
        printf("Exercise time in minutes (0-600): ");
        r = scanf("%d", &intvalue);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (r == 1 && intvalue >= 0 && intvalue <= 600)
            valid = 1;
        else
            printf("Invalid input. Enter a whole number between 0 and 600.\n");
    }
    e.exercise = intvalue;

    /* mood input */
    valid = 0;
    while (!valid)
    {
        printf("Mood score (1-10): ");
        r = scanf("%d", &intvalue);
        if (r == EOF)
            exit(0);
        while ((c = getchar()) != '\n' && c != EOF);
        if (r == 1 && intvalue >= 1 && intvalue <= 10)
            valid = 1;
        else
            printf("Invalid input. Enter a whole number between 1 and 10.\n");
    }
    e.mood = intvalue;

    /* append entry to log file and memory */
    fp = fopen("healthlog.txt", "a");
    if (fp == NULL)
    {
        printf("Could not open healthlog.txt for writing.\n");
        return;
    }
    fprintf(fp, "%s %.2f %.2f %d %d %d\n", e.date, e.water, e.sleep, e.steps, e.exercise, e.mood);
    fclose(fp);
    entries[entrycount++] = e;

    printf("\nEntry saved. Score for %s:\n", e.date);
    overall = scoreentry(e, scores);
    printscores(scores, overall);
}

/* print all logged entries */
void showentries()
{
    int i;

    if (entrycount == 0)
    {
        printf("No entries logged yet.\n");
        return;
    }
    printf("\n%-12s %8s %8s %8s %10s %6s\n", "Date", "Water", "Sleep", "Steps", "Exercise", "Mood");
    printf("-----------------------------------------------------------\n");
    for (i = 0; i < entrycount; i++)
        printf("%-12s %8.2f %8.2f %8d %10d %6d\n", entries[i].date, entries[i].water, entries[i].sleep, entries[i].steps, entries[i].exercise, entries[i].mood);
}

/* display latest and average scores */
void showscore()
{
    float scores[4], average[4] = {0, 0, 0, 0}, overall, totaloverall = 0;
    int i, j;

    if (entrycount == 0)
    {
        printf("No entries logged yet.\n");
        return;
    }
    /* accumulate scores across all days */
    for (i = 0; i < entrycount; i++)
    {
        overall = scoreentry(entries[i], scores);
        totaloverall += overall;
        for (j = 0; j < 4; j++)
            average[j] += scores[j];
    }
    for (j = 0; j < 4; j++)
        average[j] /= entrycount;
    totaloverall /= entrycount;

    overall = scoreentry(entries[entrycount - 1], scores);
    printf("\nTargets used: recommended levels for a %s person aged %s (or your custom goals).\n", gendername(), agebracket());
    printf("\nLatest entry (%s):\n", entries[entrycount - 1].date);
    printscores(scores, overall);
    printf("\nAverage across all %d entries:\n", entrycount);
    printscores(average, totaloverall);
}

/* chart recent entries against goals */
void visualize()
{
    float metrics[4], goals[4];
    int start, i, j;

    if (entrycount == 0)
    {
        printf("No entries logged yet.\n");
        return;
    }
    /* start from at most 7 entries back */
    start = entrycount > 7 ? entrycount - 7 : 0;
    getgoals(goals);

    printf("\nLast %d entries (a full bar equals your daily goal)\n", entrycount - start);
    for (j = 0; j < 4; j++)
    {
        printf("\n%s - goal %.2f\n", namesofcategories[j], goals[j]);
        for (i = start; i < entrycount; i++)
        {
            getmetrics(entries[i], metrics);
            printf("  %s ", entries[i].date);
            drawbar(metrics[j], goals[j], 20);
            printf(" %.2f\n", metrics[j]);
        }
    }
    /* mood chart scaled out of 10 */
    printf("\nMood (1-10)\n");
    for (i = start; i < entrycount; i++)
    {
        printf("  %s ", entries[i].date);
        drawbar(entries[i].mood, 10, 10);
        printf(" %d\n", entries[i].mood);
    }
}

/* compare averages on low-mood days vs other days */
void moodpredictor()
{
    float lowsum[4] = {0, 0, 0, 0}, goodsum[4] = {0, 0, 0, 0};
    float lowavg[4], goodavg[4], goals[4], metrics[4];
    int lowcount = 0, goodcount = 0, flagged = 0;
    int i, j, shortfall, contrast;
    float below;

    if (entrycount < 3)
    {
        printf("Log at least 3 days before using the mood predictor.\n");
        return;
    }
    /* group metrics by mood threshold */
    for (i = 0; i < entrycount; i++)
    {
        getmetrics(entries[i], metrics);
        if (entries[i].mood <= lowmood)
        {
            lowcount++;
            for (j = 0; j < 4; j++)
                lowsum[j] += metrics[j];
        }
        else
        {
            goodcount++;
            for (j = 0; j < 4; j++)
                goodsum[j] += metrics[j];
        }
    }
    if (lowcount == 0)
    {
        printf("No low-mood days (mood %d or below) found. Nothing to analyse.\n", lowmood);
        return;
    }

    getgoals(goals);
    /* compute group averages */
    for (j = 0; j < 4; j++)
    {
        lowavg[j] = lowsum[j] / lowcount;
        if (goodcount > 0)
            goodavg[j] = goodsum[j] / goodcount;
        else
            goodavg[j] = 0;
    }

    printf("\nLow-mood days found: %d of %d (mood %d or below)\n\n", lowcount, entrycount, lowmood);
    printf("%-15s %10s %14s %14s\n", "Metric", "Goal", "Low-mood avg", "Other days avg");
    printf("---------------------------------------------------------\n");
    for (j = 0; j < 4; j++)
    {
        if (goodcount > 0)
            printf("%-15s %10.2f %14.2f %14.2f\n", namesofcategories[j], goals[j], lowavg[j], goodavg[j]);
        else
            printf("%-15s %10.2f %14.2f %14s\n", namesofcategories[j], goals[j], lowavg[j], "n/a");
    }

    /* check for shortfalls against goals and other days */
    printf("\nInsights:\n");
    for (j = 0; j < 4; j++)
    {
        shortfall = lowavg[j] < goals[j] * 0.85;
        contrast = goodcount > 0 && lowavg[j] < goodavg[j] * 0.9;
        below = (goals[j] - lowavg[j]) / goals[j] * 100;
        if (shortfall && contrast)
        {
            printf("- %s: on low-mood days you averaged %.2f, which is %.0f%% below your goal and lower than the %.2f on your other days.\n", namesofcategories[j], lowavg[j], below, goodavg[j]);
            flagged++;
        }
        else if (shortfall)
        {
            printf("- %s: on low-mood days you averaged %.2f, which is %.0f%% below your goal of %.2f.\n", namesofcategories[j], lowavg[j], below, goals[j]);
            flagged++;
        }
        else if (contrast)
        {
            printf("- %s: on low-mood days you averaged %.2f, lower than the %.2f on your other days.\n", namesofcategories[j], lowavg[j], goodavg[j]);
            flagged++;
        }
    }
    if (flagged == 0)
        printf("- Your low-mood days do not show a clear drop in the tracked habits. Other factors may be involved.\n");
    else
        printf("\nPossible link: falling short on the habits above tends to coincide with your low-mood days.\n");
    printf("Note: this is a simple pattern check, not a medical diagnosis.\n");
}

/* program entry point and menu loop */
int main()
{
    int choice, valid, r, c;

    loadentries();
    if (!loadprofile())
    {
        printf("Welcome to the Daily Health Logger. Let's set up your profile first.\n\n");
        setupprofile();
    }

    do
    {
        printf("\n=== DAILY HEALTH LOGGER ===\n");
        printf("1. Add daily entry\n");
        printf("2. View all entries\n");
        printf("3. Score analyzer\n");
        printf("4. Mood predictor\n");
        printf("5. ASCII data visualization\n");
        printf("6. Update profile and goals\n");
        printf("7. Exit\n");
        valid = 0;
        /* validate menu selection */
        while (!valid)
        {
            printf("Enter your choice: ");
            r = scanf("%d", &choice);
            if (r == EOF)
                exit(0);
            while ((c = getchar()) != '\n' && c != EOF);
            if (r == 1 && choice >= 1 && choice <= 7)
                valid = 1;
            else
                printf("Invalid input. Enter a whole number between 1 and 7.\n");
        }

        /* dispatch selected action */
        if (choice == 1)
            addentry();
        else if (choice == 2)
            showentries();
        else if (choice == 3)
            showscore();
        else if (choice == 4)
            moodpredictor();
        else if (choice == 5)
            visualize();
        else if (choice == 6)
            setupprofile();
    }
    while (choice != 7);

    printf("Exiting.\n");
    return 0;
}
