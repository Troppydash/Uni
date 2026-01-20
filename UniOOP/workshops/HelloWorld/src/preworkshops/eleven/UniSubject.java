package preworkshops.eleven;


import java.util.*;

abstract class User {
    private String name;

    public User(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void notifyUser() {

    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        User user = (User) o;
        return Objects.equals(name, user.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(name);
    }
}

class Student extends User {
    private String studentId;

    public Student(String name, String studentId) {
        super(name);
        this.studentId = studentId;
    }

    public String getStudentId() {
        return studentId;
    }

    @Override
    public void notifyUser() {

    }
}

class Staff extends User {
    private String staffId;

    public Staff(String name, String staffId) {
        super(name);

        this.staffId = staffId;
    }

    public String getStaffId() {
        return staffId;
    }

    @Override
    public void notifyUser() {

    }
}


public class UniSubject {

    private Set<User> users;

    public UniSubject() {
        this.users = new HashSet<>();
    }

    public void makeAnnouncement() {
        String text = "zmv tfdv kf drbv re reefletvdvek jyrufn kyv yvuxvyfxj r szkty rjj dfkyvi wltbvi yv gzjjvu fe dp wltbzex nzwv kyrkj izxyk yv kffb yzj yvuxvyfx wltbze hlzccp uztb flk reu yv gzjjvu fe dp wltbzex nzwv reu yv jrzu yzj uztb nrj kyzj szx reu z jrzu kyrkj uzjxljkzex jf zd drbzex r trccflk gfjk fe dp knzkkvi ufk tfd jyrufn kyv yvuxvyfx pfl xfk r jdrcc uztb zkj kyv jzqv fw kyzj nrcelk votvgk nrp jdrccvi reu xlvjj nyrk yvivj nyrk dp ufex cffbj czbv gwwwwwwwwxak kyrkj izxyk srsp rcc gfzek ef hlzccj ef gzccfnj cffb rk kyrk zk cffbj czbv knf srccj reu r sfex yv wltbvu dp nzwv jf xlvjj nyrk zd xfeer wltb kyv vriky kyrkj izxyk kyzj zj nyrk pfl xvk dp jlgvi crqvi gzjj votvgk zd efk xfeer gzjj fe kyv vriky zd xfeer xf yzxyvi zd gzjjzex fe kyv dffffffe yfn uf pfl czbv kyrk fsrdr z gzjjvu fe kyv dffe pfl zuzfk pfl yrmv knvekpkyivv yflij svwfiv kyv gzjj u i f g c v k j yzk kyv wltbzex vriky efn xvk flk fw dp wltbzex jzxyk svwfiv z gzjj fe pfl kff";
        System.out.println(shift(text, -69));

        for (var user : users) {
            user.notifyUser();
        }
    }

    public void registerUser(User user) {
        this.users.add(user);
    }

    public void unregisterUser(User user) {
        this.users.remove(user);
    }

    public static String shift(String text, int shift) {
        String out = "";
        for (var c : text.toCharArray()) {
            if (Character.isAlphabetic(c)) {
                out += Character.toString(Math.floorMod(Character.toLowerCase(c) - 'a' + shift, 26) + 'a');
            } else if (c == ' ') {
                out += ' ';
            }
        }
        return out;
    }

    public static void main(String[] args) {
        UniSubject subject = new UniSubject();
        subject.makeAnnouncement();
    }
}
