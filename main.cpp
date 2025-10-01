#include <bits/stdc++.h>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;
using namespace std::chrono;

// ---------- Linux versions of kbhit() & getch() ----------
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

// ---------- Passages ----------
vector<string> passages = {
    "The quick brown fox jumps over the lazy dog.",
    "Practice makes a programmer perfect.",
    "Typing fast is a useful and fun skill to learn."
};

string choose_passage() {
    static mt19937 rng((unsigned)time(nullptr));
    uniform_int_distribution<int> dist(0, (int)passages.size()-1);
    return passages[dist(rng)];
}

// ---------- Mode 1 & 2: Typing Speed + Accuracy ----------
void typing_test() {
    string passage = choose_passage();
    cout << "\nType the following passage exactly:\n\n" << passage << "\n\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Press ENTER to start...";
    cin.get();

    cout << "\nStart typing:\n";
    auto start = high_resolution_clock::now();
    string typed;
    getline(cin, typed);
    auto end = high_resolution_clock::now();

    double elapsed = duration<double>(end - start).count();
    int target_len = passage.size();
    int typed_len = typed.size();

    int correct = 0;
    for (int i = 0; i < min(target_len, typed_len); i++) {
        if (typed[i] == passage[i]) correct++;
    }
    int errors = typed_len - correct;
    if (errors < 0) errors = 0;

    double minutes = elapsed / 60.0;
    double wpm = minutes > 0 ? (correct/5.0) / minutes : 0;
    double accuracy = typed_len > 0 ? (100.0 * correct / typed_len) : 0;

    cout << "\n---- Results ----\n";
    cout << "Time: " << elapsed << " seconds\n";
    cout << "Typed: " << typed_len << " chars\n";
    cout << "Correct: " << correct << " | Errors: " << errors << "\n";
    cout << "Accuracy: " << fixed << setprecision(2) << accuracy << "%\n";
    cout << "Speed: " << fixed << setprecision(2) << wpm << " WPM\n";
    cout << "-----------------\n";
}

// ---------- Mode 3: Typing Game ----------
void typing_game() {
    cout << "\nTyping Game: Type the letters before they fall!\n";
    cout << "Press ESC to quit.\n";

    vector<char> letters = {'a','s','d','f','j','k','l'};
    int score = 0, misses = 0;

    while (true) {
        char target = letters[rand() % letters.size()];
        cout << "\nLetter: " << target << "  -> Type it: ";

        auto start = high_resolution_clock::now();
        bool hit = false;
        while (duration<double>(high_resolution_clock::now() - start).count() < 3.0) {
            if (kbhit()) {
                char c = getch();
                if (c == 27) { // ESC key
                    cout << "\nGame Over!\n";
                    cout << "Score: " << score << " | Misses: " << misses << "\n";
                    return;
                }
                if (c == target) {
                    cout << "✔";
                    score++;
                } else {
                    cout << "✘";
                    misses++;
                }
                hit = true;
                break;
            }
            this_thread::sleep_for(50ms);
        }
        if (!hit) {
            cout << "Missed!";
            misses++;
        }
    }
}

// ---------- Main Menu ----------
int main() {
    srand(time(0));
    while (true) {
        cout << "\n=== Typing Program ===\n";
        cout << "1. Typing Speed Test (WPM)\n";
        cout << "2. Accuracy Test\n";
        cout << "3. Typing Game (falling letters)\n";
        cout << "4. Exit\n";
        cout << "Choose option: ";
        int ch; cin >> ch;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ch == 1 || ch == 2) typing_test();
        else if (ch == 3) typing_game();
        else if (ch == 4) break;
        else cout << "Invalid option!\n";
    }
    return 0;
}
