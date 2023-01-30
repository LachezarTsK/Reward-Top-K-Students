
import java.util.Set;
import java.util.List;
import java.util.HashSet;
import java.util.ArrayList;
import java.util.Collections;
import java.util.PriorityQueue;

public class Solution {

    private record Student(int ID, int score) {}
    private final Set<String> positiveFeedbackWords = new HashSet<>();
    private final Set<String> negativeFeedbackWords = new HashSet<>();
   
    public List<Integer> topStudents(String[] positiveFeedback, String[] negativeFeedback,
            String[] report, int[] studentID, int numberOfTopStudentsToRank) {

        Collections.addAll(positiveFeedbackWords, positiveFeedback);
        Collections.addAll(negativeFeedbackWords, negativeFeedback);
        PriorityQueue<Student> minHeapStudents = new PriorityQueue<>(
                                                 (studentOne, studentTwo) -> (studentOne.score == studentTwo.score)
                                                 ? studentTwo.ID - studentOne.ID
                                                 : studentOne.score - studentTwo.score);

        int index = 0;
        while (index < studentID.length && minHeapStudents.size() < numberOfTopStudentsToRank) {
            int score = calculateScore(report[index]);
            minHeapStudents.add(new Student(studentID[index], score));
            ++index;
        }

        while (index < studentID.length) {
            int score = calculateScore(report[index]);
            if (currentStudentRanksHigher(minHeapStudents, studentID[index], score)) {
                minHeapStudents.poll();
                minHeapStudents.add(new Student(studentID[index], score));
            }
            ++index;
        }

        return rankedStudentsAsSortedList(minHeapStudents);
    }

    private List<Integer> rankedStudentsAsSortedList(PriorityQueue<Student> minHeapStudents) {
        List<Integer> topStudentsID = new ArrayList<>();
        while (!minHeapStudents.isEmpty()) {
            topStudentsID.add(minHeapStudents.poll().ID);
        }
        Collections.reverse(topStudentsID);
        return topStudentsID;
    }

    private int calculateScore(String report) {
        int score = 0;
        String[] array = report.split(" ");
        for (int i = 0; i < array.length; ++i) {
            if (positiveFeedbackWords.contains(array[i])) {
                score += 3;
            } else if (negativeFeedbackWords.contains(array[i])) {
                --score;
            }
        }
        return score;
    }

    private boolean currentStudentRanksHigher(PriorityQueue<Student> minHeapStudents, int studentID, int score) {
        return score > minHeapStudents.peek().score || (score == minHeapStudents.peek().score && studentID < minHeapStudents.peek().ID);
    }
}
