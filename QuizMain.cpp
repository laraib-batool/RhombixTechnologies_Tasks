

#include "Task_2.h"

int main() {
    // ── Seed RNG (used for question shuffling & random category) ──
    srand((unsigned int)time(nullptr));

    // ── Build the question bank from quiz_utils.cpp ───────────────
    map<string, vector<Question>> bank = buildQuestionBank();

    // ── Session leaderboard (accumulates across multiple rounds) ──
    vector<LeaderEntry> leaderboard;

    // ── Register the first player ─────────────────────────────────
    Player player = registerPlayer();

    // ── Main game loop ────────────────────────────────────────────
    bool running = true;

    while (running) {
        int choice = mainMenu();

        switch (choice) {

            // ── Case 1: Start a new quiz round ────────────────────
            case 1: {
                // 1. Let player pick a category
                string category = chooseCategory(bank);

                // 2. Let player pick a difficulty
                int difficulty = chooseDifficulty();

                // 3. Shuffle a copy of the chosen category's questions
                //    (Fisher-Yates shuffle so every run is different)
                vector<Question> qs = bank[category];
                for (int i = (int)qs.size() - 1; i > 0; i--) {
                    int j = rand() % (i + 1);
                    swap(qs[i], qs[j]);
                }

                // 4. Run the quiz and collect the result
                QuizResult result = runQuiz(qs, category, player, difficulty);

                // 5. Show the results + answer review screen
                showResults(player, result, category, difficulty);

                // 6. Update the player's cumulative session totals
                player.score          += result.score;
                player.totalQuestions += result.total;
                player.category        = category;

                // 7. Add this round's entry to the leaderboard
                int pointsPerQ = (difficulty == 1) ? 10
                               : (difficulty == 2) ? 15
                               :                     20;
                int    maxScore = result.total * pointsPerQ;
                double accuracy = (maxScore > 0)
                                  ? (100.0 * result.score / maxScore)
                                  : 0.0;

                LeaderEntry entry;
                entry.name     = player.name;
                entry.score    = result.score;
                entry.category = category;
                entry.accuracy = accuracy;
                leaderboard.push_back(entry);

                pressEnter();
                break;
            }

            // ── Case 2: View leaderboard ──────────────────────────
            case 2:
                showLeaderboard(leaderboard);
                break;

            // ── Case 3: Change the active player ──────────────────
            case 3:
                player = registerPlayer();
                break;

            // ── Case 4: Quit the application ──────────────────────
            case 4:
                clearScreen();
                cout << "\n  Thanks for playing, " << player.name << "!\n"
                     << "  Session score : " << player.score    << " points\n"
                     << "  Questions answered : " << player.totalQuestions << "\n\n";
                running = false;
                break;
        }
    }

    return 0;
}

