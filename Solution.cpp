
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <unordered_set>
using namespace std;

class Solution {

    struct Student {
        int ID{};
        int score{};
        Student(int ID, int score) : ID{ID}, score{score}{}
    };

    struct ComparatorStudent {
        bool operator()(const Student& studentOne, const Student& studentTwo) const {
            return (studentOne.score == studentTwo.score) ? studentTwo.ID > studentOne.ID : studentOne.score > studentTwo.score;
        };
    };

    using MinHeapStudents = priority_queue<Student, vector<Student>, ComparatorStudent>;
    unique_ptr<unordered_set<string>> positiveFeedbackWords;
    unique_ptr<unordered_set<string>> negativeFeedbackWords;

public:
    vector<int> topStudents(const vector<string>& positiveFeedback, const vector<string>& negativeFeedback,
            const vector<string>& report, const vector<int>& studentID, int numberOfTopStudentsToRank) {

        positiveFeedbackWords = make_unique<unordered_set<string>>(positiveFeedback.begin(), positiveFeedback.end());
        negativeFeedbackWords = make_unique<unordered_set<string>>(negativeFeedback.begin(), negativeFeedback.end());
        MinHeapStudents minHeapStudents;

        int index = 0;
        while (index < studentID.size() && minHeapStudents.size() < numberOfTopStudentsToRank) {
            int score = calculateScore(report[index]);
            minHeapStudents.emplace(Student(studentID[index], score));
            ++index;
        }

        while (index < studentID.size()) {
            int score = calculateScore(report[index]);
            if (currentStudentRanksHigher(minHeapStudents, studentID[index], score)) {
                minHeapStudents.pop();
                minHeapStudents.emplace(Student(studentID[index], score));
            }
            ++index;
        }

        return rankedStudentsAsSortedVector(minHeapStudents);
    }

private:
    vector<int> rankedStudentsAsSortedVector(MinHeapStudents& minHeapStudents) const {
        vector<int> topStudentsID;
        while (!minHeapStudents.empty()) {
            topStudentsID.push_back(minHeapStudents.top().ID);
            minHeapStudents.pop();
        }
        reverse(topStudentsID.begin(), topStudentsID.end());
        return topStudentsID;
    }

    int calculateScore(const string& report) const {
        stringstream streamReport(report);
        string feedbackWord;
        int score = 0;

        while (getline(streamReport, feedbackWord, ' ')) {
            if (positiveFeedbackWords->find(feedbackWord) != positiveFeedbackWords->end()) {
                score += 3;
            } else if (negativeFeedbackWords->find(feedbackWord) != negativeFeedbackWords->end()) {
                --score;
            }
        }
        return score;
    }

    bool currentStudentRanksHigher(const MinHeapStudents& minHeapStudents, int studentID, int score) const {
        return score > minHeapStudents.top().score || (score == minHeapStudents.top().score && studentID < minHeapStudents.top().ID);
    }
};
