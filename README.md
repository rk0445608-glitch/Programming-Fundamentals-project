# Programming-Fundamentals-project
Programming Fundamentals Project made by Rayan Khan and Abdullah bin Kashif
# Daily Health Logger

A console-based program written in C that logs daily water intake, sleep, steps, exercise time and mood. It scores your habits against daily goals, draws ASCII bar charts, and checks whether your low-mood days line up with habits that fell short. Everything is stored in plain text files on your own computer, so no internet connection is needed.

Programming Fundamentals, Section B

Group members:
- 26K-0562 Abdullah bin Kashif
- 26K-0555 Farhan Ali Soomro
- 26K-0578 Rayan Khan

## What you need

- A C compiler. GCC is recommended. Code::Blocks and Dev C++ also work.
- Windows or Linux.
- Only the standard libraries `stdio.h`, `stdlib.h` and `string.h` are used, so nothing extra has to be installed.

## Compiling

Open a terminal in the folder that holds the source file and run:

```
gcc dailyhealthlogger.c -o dailyhealthlogger
```

Replace `dailyhealthlogger.c` with the name of your source file. If you use Code::Blocks or Dev C++, open the file in the IDE and press Build and Run instead.

## Running

Linux or macOS:

```
./dailyhealthlogger
```

Windows (Command Prompt or PowerShell):

```
dailyhealthlogger.exe
```

Run it from a terminal rather than by double-clicking, otherwise the window may close as soon as the program ends.

## First run

On the first run there is no saved profile, so the program asks you to set one up:

1. Enter your age (5 to 100).
2. Enter your gender (M or F).
3. The program shows recommended daily goals for your age group and gender.
4. Type `y` to enter your own goals, or `n` to keep the recommended ones.

Your profile and goals are saved, and you will not be asked again on later runs. You can change them any time with menu option 6.

### Recommended goals

| Age group | Sleep | Steps | Exercise | Water |
| --- | --- | --- | --- | --- |
| Under 18 | 9 hours | 12000 | 60 min | 1.8 L |
| 18 to 39 | 8 hours | 10000 | 30 min | 3.0 L (male), 2.2 L (female) |
| 40 to 64 | 8 hours | 8000 | 30 min | 3.0 L (male), 2.2 L (female) |
| 65 and above | 7.5 hours | 6000 | 30 min | 3.0 L (male), 2.2 L (female) |

These are simplified general guidelines, not medical standards.

If you choose custom goals, the allowed ranges are: water 0.5 to 10 litres, sleep 3 to 14 hours, steps 1000 to 50000, exercise 5 to 300 minutes.

## The menu

| Option | What it does |
| --- | --- |
| 1. Add daily entry | Logs one day of data and shows that day's score |
| 2. View all entries | Prints every logged day in a table |
| 3. Score analyzer | Shows scores for your latest entry and the average of all entries |
| 4. Mood predictor | Compares your low-mood days with your other days |
| 5. ASCII data visualization | Draws bar charts for the last 7 entries |
| 6. Update profile and goals | Lets you change your age, gender and goals |
| 7. Exit | Closes the program |

### 1. Add daily entry

You are asked for the following, one at a time. If a value is invalid, the program explains why and asks again.

| Field | Accepted values |
| --- | --- |
| Date | `YYYY-MM-DD`, a real date, for example `2026-11-10` |
| Water | 0 to 15 litres |
| Sleep | 0 to 24 hours |
| Steps | 0 to 100000 |
| Exercise | 0 to 600 minutes |
| Mood | 1 to 10 (1 is lowest, 10 is highest) |

Only one entry is allowed per date. The log holds up to 365 entries, which is one year of daily records.

### 3. Score analyzer

Each metric is scored as a percentage of its goal. Going over the goal does not give extra points, so each score tops out at 100%. The overall score is the average of the four metrics.

| Overall score | Rating |
| --- | --- |
| 90% and above | Excellent |
| 70% to 89% | Good |
| 50% to 69% | Fair |
| Below 50% | Needs improvement |

### 4. Mood predictor

The predictor needs at least 3 logged days. It works like this:

1. Days with a mood of 4 or below count as low-mood days. All other days are treated as normal days.
2. It averages water, sleep, steps and exercise separately for low-mood days and for normal days.
3. It reports a habit if the low-mood average is more than 15% below your goal, or more than 10% below your average on normal days.
4. If no habit stands out, it says there is no clear link.

This is a simple pattern check on your own data. It is not a medical diagnosis and does not prove that a habit caused a mood change.

### 5. ASCII data visualization

Shows one bar chart each for water, sleep, steps and exercise, plus one for mood, covering your last 7 entries. In the habit charts a full bar means you reached your daily goal. The mood chart is scaled out of 10.

```
Sleep (hrs) - goal 8.00
  2026-11-10 |####################| 8.00
  2026-11-11 |############........| 5.00
```

## Files the program creates

Both files are created automatically in the folder you run the program from.

| File | Contents |
| --- | --- |
| `healthlog.txt` | One line per day: date, water, sleep, steps, exercise, mood |
| `profile.txt` | One line: age, gender, water goal, sleep goal, steps goal, exercise goal |

Example `healthlog.txt` line:

```
2026-11-10 3.00 8.00 10000 40 8
```

Example `profile.txt` line:

```
20 M 3.00 8.00 10000 30
```

You can open these files in any text editor. If you edit them by hand, keep the same layout: one entry per line, values separated by single spaces, and no spaces inside the date.

## Continuing where you left off

You can close the terminal at any time. Each entry is saved as soon as you finish answering the last question, and the program reloads both files every time it starts. If you close the program halfway through entering a day, only that unfinished entry is lost.

To keep your history safe, always run the program from the same folder, and back up `healthlog.txt` and `profile.txt` now and then.

## Starting fresh

Close the program, then delete `healthlog.txt` to clear all entries, and delete `profile.txt` to set up a new profile on the next run.

## Troubleshooting

| Problem | Likely cause and fix |
| --- | --- |
| `gcc` is not recognised | GCC is not installed or not on your PATH. On Linux install `build-essential`. On Windows install MinGW or use Code::Blocks or Dev C++. |
| The program asks for your profile every time | `profile.txt` is missing or you are running from a different folder. Run it from the folder that contains the file. |
| Your old entries are gone | You are probably running from a different folder, so a new `healthlog.txt` was created. Go back to the original folder. |
| "The log is full" | The 365 entry limit has been reached. Back up `healthlog.txt` and start a new one. |
| "An entry for that date already exists" | That date is already logged. Use a different date. |
| The window closes immediately on Windows | Open Command Prompt first and run the program from there. |

## Limitations

- The goals are general guidelines based on age group and gender, not medical advice.
- Scores compare you against those recommended goals, not against data from other people.
- Entries cannot be edited or deleted from inside the program. Edit `healthlog.txt` by hand if you need to fix one.
- One user profile and up to 365 entries are supported.
- The program is console-only and has no graphical interface.
