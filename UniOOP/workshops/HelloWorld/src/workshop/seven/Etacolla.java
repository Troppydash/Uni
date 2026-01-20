package workshop.seven;

import java.util.ArrayList;
import java.util.List;

class Subject {

}

class Activity {

}

class Preference {
}

class Student2 {
    List<Preference> getPreferences() {
        return new ArrayList<>();
    }
}

interface UniversityService {
    List<String> getEnrolledSubjectCodes(Student2 student);

    Subject getSubject(String name);

    void registerStudentInActivity(Student2 student, Activity activity);
}


public class Etacolla {
    private final UniversityService mUniService;

    public Etacolla(UniversityService service) {
        this.mUniService = service;
    }

    ArrayList<Activity> allocatePreferences(List<Preference> preferences, List<Subject> subjects) {
        return new ArrayList<>();
    }

    public void generateTimetable(Student2 student) {
        List<String> subjectNames = mUniService.getEnrolledSubjectCodes(student);
        List<Subject> subjects = new ArrayList<>();
        for (String subjectName : subjectNames) {
            Subject subject = mUniService.getSubject(subjectName);
            subjects.add(subject);
        }
        // allocate activities to student ...
        List<Activity> allocated = allocatePreferences(student.getPreferences(), subjects);
        for (Activity activity : allocated) {
            mUniService.registerStudentInActivity(student, activity);
        }
    }
}