
/**
 * @param {string[]} positiveFeedback
 * @param {string[]} negativeFeedback
 * @param {string[]} report
 * @param {number[]} studentID
 * @param {number} numberOfTopStudentsToRank
 * @return {number[]}
 */
var topStudents = function (positiveFeedback, negativeFeedback, report, studentID, numberOfTopStudentsToRank) {
    this.positiveFeedbackWords = new Set(positiveFeedback);
    this.negativeFeedbackWords = new Set(negativeFeedback);

    //const {MinPriorityQueue} = require('@datastructures-js/priority-queue');
    const minHeapStudents = new MinPriorityQueue({compare:
                            (studentOne, studentTwo) => (studentOne.score === studentTwo.score)
                            ? studentTwo.ID - studentOne.ID
                            : studentOne.score - studentTwo.score});

    let index = 0;
    while (index < studentID.length && minHeapStudents.size() < numberOfTopStudentsToRank) {
        let score = calculateScore(report[index]);
        minHeapStudents.enqueue(new Student(studentID[index], score));
        ++index;
    }

    while (index < studentID.length) {
        let score = calculateScore(report[index]);
        if (currentStudentRanksHigher(minHeapStudents, studentID[index], score)) {
            minHeapStudents.dequeue();
            minHeapStudents.enqueue(new Student(studentID[index], score));
        }
        ++index;
    }

    return rankedStudentsAsSortedArray(minHeapStudents);
};

/**
 * @param {number} ID
 * @param {number} score
 */
function Student(ID, score) {
    this.ID = ID;
    this.score = score;
}

/**
 * @param {MinPriorityQueue<Student>} minHeapStudents
 * @return {number[]}
 */
function rankedStudentsAsSortedArray(minHeapStudents) {
    const topStudentsID = [];
    while (!minHeapStudents.isEmpty()) {
        topStudentsID.push(minHeapStudents.dequeue().ID);
    }
    topStudentsID.reverse();
    return topStudentsID;
}

/**
 * @param {string[]} report
 * @return {number}
 */
function  calculateScore(report) {
    let score = 0;
    const array = report.split(" ");
    for (let i = 0; i < array.length; ++i) {
        if (this.positiveFeedbackWords.has(array[i])) {
            score += 3;
        } else if (this.negativeFeedbackWords.has(array[i])) {
            --score;
        }
    }
    return score;
}

/**
 * @param {MinPriorityQueue<Student>} minHeapStudents
 * @param {number} studentID
 * @param {number} score
 * @return {boolean}
 */
function currentStudentRanksHigher(minHeapStudents, studentID, score) {
    return score > minHeapStudents.front().score || (score === minHeapStudents.front().score && studentID < minHeapStudents.front().ID);
}
