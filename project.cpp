#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <sstream>
#include <algorithm>
#include <cctype>     // for toupper
#include <windows.h>  // for system("cls") on Windows

using namespace std;


void clearScreen() {
    system("cls");  // Windows ϵͳ
}

// �������ݽṹ
struct Question {
    string questionText;
    set<char> correctAnswers;
    string explanation;
};

// ��������
set<char> parseAnswers(const string& answerLine) {
    set<char> correctAnswers;
    size_t pos = answerLine.find("Answer:");
    if (pos != string::npos) {
        string answerPart = answerLine.substr(pos + 7);
        stringstream ss(answerPart);
        string token;
        while (getline(ss, token, ',')) {
            for (size_t i = 0; i < token.size(); ++i) {
                char c = token[i];
                if (isalpha(c)) {
                    correctAnswers.insert(toupper(c));
                }
            }
        }
    }
    return correctAnswers;
}

// ����������
string parseExplanation(const string& line) {
    size_t pos = line.find("Explanation:");
    if (pos != string::npos) {
        return line.substr(pos + 12);
    }
    return "";
}

// ��ȡ�û���
set<char> getUserAnswers() {
    string input;
    cout << "Your answer (A/B/C/D or multiple like A,C): ";
    cin >> input;
    cin.ignore(); // ������뻺����

    set<char> userAnswers;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (isalpha(c)) {
            userAnswers.insert(toupper(c));
        }
    }
    return userAnswers;
}

// ��ȡ�û�����
string getUserFeedback() {
    string feedback;
    cout << "\nPlease share your feedback about this quiz:\n";
    getline(cin, feedback);
    return feedback;
}

// ��ȡ��������
string getReview(int score) {
    if (score <= 3) return "Needs Improvement";
    else if (score <= 7) return "Good Job!";
    else return "Excellent! You��re a Smart City Transport Champion!";
}

// �����û�����
void saveUserData(const string& name, int score, const string& review, const string& feedback) {
    ofstream file("user.txt", ios::app); // ׷��ģʽ
    if (file.is_open()) {
        file << "Name: " << name << "\n";
        file << "Score: " << score << "/15\n";
        file << "Review: " << review << "\n";
        file << "Feedback: " << feedback << "\n";
        file << "------------------------\n";
        file.close();
        cout << "Your data has been saved successfully.\n";
    } else {
        cerr << "Unable to save user data.\n";
    }
}

// ����ϵͳ��
class EcoTransportQuizSystem {
public:
    // ���캯�� - ��ʾ���ض���
    EcoTransportQuizSystem() {
        system("COLOR 0e");
        system("cls");

        SetConsoleCP(437);
        SetConsoleOutputCP(437);

        int bar1 = 219;

        cout << "\n\n\n\t\t\t\tWelcome to Eco-Friendly Transportation Quiz :)";
        cout << "\n\n\n\t\t\t\t";

        cout << "\r\t\t\t\t";

        for (int i = 0; i <= 100; i += 4) {
            cout << " " << i << "%";
            cout << flush;
            Sleep(15);
            cout << "\r\t\t\t\t";
            for (int j = 0; j < i / 4; j++) {
                cout << (char)bar1;
            }
        }
        cout << "\n\t\t\t\t" << (char)1 << "!";
        cout << endl;
        system("pause");
        system("cls");
    }

    // ��������
    ~EcoTransportQuizSystem() {
        cout << "Thank you for using our Eco-Friendly Transportation Quiz. Goodbye!\n";
    }
};

int main() {
    // ��ʼ������ϵͳ
    EcoTransportQuizSystem quizSystem;

    // �������ļ�
    ifstream file("question.txt");
    if (!file) {
        cout << "Error: Could not open question.txt file." << endl;
        return 1;
    }

    string userName;
    int correctCount = 0;
    int totalQuestions = 0;

    // ��ȡ�û�����
    clearScreen();
    cout << "===== Welcome to the Green City, Smart Living Quiz! =====" << endl;
    cout << "Topic: Eco-Friendly Transportation in Smart Cities" << endl;
    cout << "Please enter your name: ";
    getline(cin, userName);

    // ��ʾ����˵��
    clearScreen();
    cout << "\nHi " << userName << "! You will be asked 15 multiple-choice questions." << endl;
    cout << "Each correct answer gives you 1 point. Select all correct options for multiple-answer questions.\n\nLet's begin!\n\n";

    // ��ȡ�����ļ�
    string line;
    while (getline(file, line)) {
        Question currentQuestion;
        currentQuestion.questionText = "";

        // ��ȡ�����ı�
        while (line.find("Answer:") == string::npos && !file.eof()) {
            currentQuestion.questionText += line + "\n";
            getline(file, line);
        }

        // ������ȷ��
        currentQuestion.correctAnswers = parseAnswers(line);

        // ��ȡ����
        if (getline(file, line)) {
            currentQuestion.explanation = parseExplanation(line);
        }

        // ��ʾ����
        totalQuestions++;
        cout << "===== Question " << totalQuestions << " =====" << endl;
        cout << currentQuestion.questionText << endl;

        // ��ȡ�û���
        set<char> userAnswers = getUserAnswers();

        // ����
        if (userAnswers == currentQuestion.correctAnswers) {
            cout << "Correct!\n\n";
            correctCount++;
        } else {
            cout << "Wrong! The correct answer was: ";
            set<char>::iterator it;
            for (it = currentQuestion.correctAnswers.begin(); it != currentQuestion.correctAnswers.end(); ++it) {
                cout << *it << " ";
            }
            cout << "\n\n";

            // ��ʾ����
            if (!currentQuestion.explanation.empty()) {
                cout << "Explanation:\n" << currentQuestion.explanation << "\n\n";
            }
        }
    }

    file.close();

    // �������ֺ�����
    string review = getReview(correctCount);

    // ��ȡ�û�����
    string feedback = getUserFeedback();

    // �����û�����
    saveUserData(userName, correctCount, review, feedback);

    // ��ʾ���
    clearScreen();
    cout << "===== Quiz Complete! =====" << endl;
    cout << userName << ", you scored " << correctCount << " out of " << totalQuestions << "." << endl;
    cout << "Review: " << review << endl;

    system("pause"); // ���ִ��ڴ�
    return 0;
}
