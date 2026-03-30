/*
 * ============================================================
 *  quiz_game.cpp  —  DEFINITIONS  (game logic layer)
 *
 *  Contains implementations of:
 *    • registerPlayer()     — user registration with validation
 *    • chooseCategory()     — category selection menu
 *    • chooseDifficulty()   — difficulty selection menu
 *    • mainMenu()           — main navigation menu
 *    • runQuiz()            — core question-answer loop
 *    • showResults()        — post-quiz score + answer review
 *    • leaderCmp()          — leaderboard comparator
 *    • showLeaderboard()    — ranked leaderboard display
 * ============================================================
 */

#include "Task_2.h"

// ─────────────────────────────────────────────────────────────
//  USER REGISTRATION
// ─────────────────────────────────────────────────────────────

/*
 * Prompt the user for a name and age.
 * Performs full input validation on both fields.
 * Returns a fully initialised Player struct.
 */
Player registerPlayer() {
    Player p;
    clearScreen();
    printHeader("  WELCOME TO QUIZ MASTER  ");
    cout << "\n";

    // --- Name input ---
    cout << "  Enter your name : ";
    getline(cin, p.name);

    while (p.name.empty()) {
        cout << "  Name cannot be empty. Enter your name: ";
        getline(cin, p.name);
    }

    // --- Age input ---
    cout << "  Enter your age  : ";
    while (!(cin >> p.age) || p.age <= 0 || p.age > 120) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid age. Please enter a valid age (1-120): ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // --- Initialise counters ---
    p.score          = 0;
    p.totalQuestions = 0;
    p.category       = "";

    cout << "\n  Welcome, " << p.name << "! Let's test your knowledge!\n";
    pressEnter();
    return p;
}

// ─────────────────────────────────────────────────────────────
//  CATEGORY SELECTION
// ─────────────────────────────────────────────────────────────

/*
 * Display all available categories from the question bank.
 * Includes a "Random" option as the last choice.
 * Returns the name of the selected category string.
 */
string chooseCategory(const map<string, vector<Question>>& bank) {
    // Collect and sort category names alphabetically
    vector<string> categories;
    for (auto& kv : bank)
        categories.push_back(kv.first);
    sort(categories.begin(), categories.end());

    clearScreen();
    printHeader("  SELECT A CATEGORY  ");
    cout << "\n";

    for (int i = 0; i < (int)categories.size(); i++)
        cout << "  [" << (i + 1) << "]  " << categories[i] << "\n";
    cout << "  [" << (int)categories.size() + 1 << "]  Random\n\n";

    int choice = 0;
    cout << "  Your choice: ";
    while (!(cin >> choice) ||
           choice < 1 ||
           choice > (int)categories.size() + 1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid choice. Try again: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == (int)categories.size() + 1)
        return categories[rand() % categories.size()];  // Random pick
    return categories[choice - 1];
}

// ─────────────────────────────────────────────────────────────
//  DIFFICULTY SELECTION
// ─────────────────────────────────────────────────────────────

/*
 * Let the player choose a difficulty level.
 * Returns:  1 = Easy (+10 pts/q)
 *           2 = Medium (+15 pts/q)
 *           3 = Hard (+20 pts/q)
 */
int chooseDifficulty() {
    clearScreen();
    printHeader("  SELECT DIFFICULTY  ");
    cout << "\n"
         << "  [1]  Easy    --  6 questions, +10 pts each\n"
         << "  [2]  Medium  --  6 questions, +15 pts each\n"
         << "  [3]  Hard    --  6 questions, +20 pts each\n\n"
         << "  Your choice: ";

    int d = 0;
    while (!(cin >> d) || d < 1 || d > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid. Choose 1, 2, or 3: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return d;
}

// ─────────────────────────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────────────────────────

/*
 * Display the main navigation menu.
 * Returns the player's choice as an integer (1-4).
 */
int mainMenu() {
    clearScreen();
    printHeader("  QUIZ MASTER -- MAIN MENU  ");
    cout << "\n"
         << "  [1]  Start New Quiz\n"
         << "  [2]  View Leaderboard\n"
         << "  [3]  Change Player\n"
         << "  [4]  Quit\n\n"
         << "  Choice: ";

    int c = 0;
    while (!(cin >> c) || c < 1 || c > 4) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid. Enter 1-4: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return c;
}

// ─────────────────────────────────────────────────────────────
//  CORE QUIZ LOOP
// ─────────────────────────────────────────────────────────────

/*
 * Run one complete quiz round.
 *
 * Parameters:
 *   questions  — pre-shuffled list of Question objects to ask
 *   category   — display name of the chosen category
 *   player     — current player (used for header display only)
 *   difficulty — 1/2/3, determines points per correct answer
 *
 * Returns:
 *   QuizResult containing score, total count, and full answer history
 */
QuizResult runQuiz(const vector<Question>& questions,
                   const string&           category,
                   const Player&           player,
                   int                     difficulty) {

    // Points awarded per correct answer
    int pointsPerQ = (difficulty == 1) ? 10
                   : (difficulty == 2) ? 15
                   :                     20;

    QuizResult result;
    result.score = 0;
    result.total = (int)questions.size();

    for (int i = 0; i < (int)questions.size(); i++) {
        const Question& q = questions[i];
        clearScreen();

        // ── Status bar ────────────────────────────────────────
        printLine('-');
        cout << "  Category : " << category
             << "  |  Player: "  << player.name
             << "  |  Score: "   << result.score << "\n";
        printLine('-');

        // ── Question text ─────────────────────────────────────
        cout << "\n  Question " << (i + 1) << " of " << questions.size()
             << "\n\n  " << q.text << "\n\n";

        // ── Answer options ────────────────────────────────────
        for (int j = 0; j < 4; j++)
            cout << "    " << char('A' + j) << ")  " << q.options[j] << "\n";

        // ── Accept input ──────────────────────────────────────
        cout << "\n  Your answer (A/B/C/D): ";
        char ans = ' ';
        while (true) {
            cin >> ans;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            ans = (char)toupper((unsigned char)ans);
            if (ans >= 'A' && ans <= 'D') break;
            cout << "  Invalid input. Enter A, B, C, or D: ";
        }

        int userIdx = ans - 'A';
        result.history.push_back({q, userIdx});  // Record for review

        // ── Feedback ──────────────────────────────────────────
        if (userIdx == q.correctIndex) {
            result.score += pointsPerQ;
            cout << "\n  [CORRECT]  +" << pointsPerQ << " points\n";
        } else {
            cout << "\n  [WRONG]  The correct answer was "
                 << char('A' + q.correctIndex) << ") "
                 << q.options[q.correctIndex] << "\n";
        }
        cout << "  INFO: " << q.explanation << "\n";
        pressEnter();
    }

    return result;
}

// ─────────────────────────────────────────────────────────────
//  RESULTS SCREEN
// ─────────────────────────────────────────────────────────────

/*
 * Display the end-of-round results screen.
 * Shows: player info, score, accuracy, a grade, and a full
 * question-by-question answer review with correct/wrong markers.
 */
void showResults(const Player&     player,
                 const QuizResult& result,
                 const string&     category,
                 int               difficulty) {

    int    pointsPerQ = (difficulty == 1) ? 10
                      : (difficulty == 2) ? 15
                      :                     20;
    int    maxScore   = result.total * pointsPerQ;
    double pct        = (maxScore > 0)
                        ? (100.0 * result.score / maxScore)
                        : 0.0;

    clearScreen();
    printHeader("  QUIZ RESULTS  ");
    cout << "\n";

    // ── Summary ───────────────────────────────────────────────
    cout << "  Player   : " << player.name << "  (Age: " << player.age << ")\n"
         << "  Category : " << category    << "\n"
         << "  Score    : " << result.score << " / " << maxScore << "\n";
    cout << fixed << setprecision(1);
    cout << "  Accuracy : " << pct << "%\n\n";

    // ── Grade ─────────────────────────────────────────────────
    string grade;
    if      (pct == 100.0) grade = "PERFECT!  Outstanding performance!";
    else if (pct >= 80.0)  grade = "EXCELLENT!  Great work!";
    else if (pct >= 60.0)  grade = "GOOD JOB!  Solid effort!";
    else if (pct >= 40.0)  grade = "KEEP PRACTICING!  You'll get there!";
    else                   grade = "BETTER LUCK NEXT TIME!  Don't give up!";
    cout << "  >> " << grade << "\n\n";

    // ── Answer review ─────────────────────────────────────────
    printLine('-');
    cout << "  ANSWER REVIEW\n";
    printLine('-');

    for (int i = 0; i < (int)result.history.size(); i++) {
        const Question& q   = result.history[i].first;
        int             usr = result.history[i].second;
        bool            ok  = (usr == q.correctIndex);

        cout << "\n  Q" << (i + 1) << ". " << q.text << "\n";

        for (int j = 0; j < 4; j++) {
            string marker = "     ";          // neutral
            if      (j == q.correctIndex && j == usr) marker = " [/] ";  // correct + chosen
            else if (j == q.correctIndex)             marker = " [*] ";  // correct, not chosen
            else if (j == usr)                        marker = " [X] ";  // wrong choice

            cout << "  " << marker << char('A' + j) << ") " << q.options[j] << "\n";
        }

        // Show explanation only when the player was wrong
        if (!ok) {
            cout << "  --> Correct answer: "
                 << char('A' + q.correctIndex) << ")  "
                 << q.explanation << "\n";
        }
    }

    printLine('-');
    cout << "\n  Legend:  [/] = Your correct answer   "
            "[*] = Correct (missed)   [X] = Your wrong answer\n";
}

// ─────────────────────────────────────────────────────────────
//  LEADERBOARD
// ─────────────────────────────────────────────────────────────

/*
 * Comparator for sorting LeaderEntry objects by score (descending).
 * Used with std::sort in showLeaderboard().
 */
bool leaderCmp(const LeaderEntry& a, const LeaderEntry& b) {
    return a.score > b.score;
}

/*
 * Display the session leaderboard, sorted by score (highest first).
 * Prints rank, name, score, category, and accuracy for each entry.
 */
void showLeaderboard(vector<LeaderEntry>& board) {
    clearScreen();
    printHeader("  LEADERBOARD  ");
    cout << "\n";

    if (board.empty()) {
        cout << "  No scores recorded yet. Play a round first!\n";
        pressEnter();
        return;
    }

    sort(board.begin(), board.end(), leaderCmp);

    // Table header
    cout << "  " << left
         << setw(6)  << "Rank"
         << setw(22) << "Name"
         << setw(10) << "Score"
         << setw(16) << "Category"
         << "Accuracy\n";
    printLine('-');

    // Table rows
    for (int i = 0; i < (int)board.size(); i++) {
        cout << "  " << left
             << setw(6)  << (i + 1)
             << setw(22) << board[i].name
             << setw(10) << board[i].score
             << setw(16) << board[i].category
             << fixed << setprecision(1) << board[i].accuracy << "%\n";
    }

    pressEnter();
}

