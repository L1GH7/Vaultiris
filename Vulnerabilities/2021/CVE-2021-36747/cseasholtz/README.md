# CVE-2021-36747

### Blackboard Learn through 9.1 allows XSS by an authenticated user via the Feedback to Learner form.

</br>

#### Discovered: May 11th, 2018
*The Blackboard Security Team has been notified of this issue. They acknowledged the vulnerability on May 14, 2018.*
</br>

#### Description:
The Blackboard tool commonly used in K-12 and Higher Education allows Instructors and Graders to provide feedback on assignments submitted. This feedback is plaintext that is converted to HTML. This could allow malicious actors to insert Javascript, which would be executed in the Learner's web browser when viewing the feedback.
</br>

#### Impact:
An arbitrary script may be executed on the user's web browser (CWE-79).
</br>

#### Affected Versions:
Version 9.1 and earlier versions that include the ability to edit HTML when providing Feedback to Learner in the grading process.
</br>

#### Steps to Reproduce:
1. To exploit the vulnerability, the attacker must enter a <script> open and </script> close tag in the Feedback to Learner area of the assignment grading tool
2. On submission, the script is stored in the comment section of the assignment. When the student user views the comment on their Grades page, the script is executed
</br>

#### Screenshots
![alt text](https://github.com/cseasholtz/CVE-2021-36747/blob/main/Feedback-to-Learner.png "Feedback to Learner")
![alt text](https://github.com/cseasholtz/CVE-2021-36747/blob/main/Results.png "Results")
</br>
