package preworkshops.two;

import java.util.ArrayList;

public class Meeting {
    private ArrayList<Participant> participants;
    private Participant host;

    public static int totalParticipants = 0;
    public static final int TOTAL_PARTICIPANT_LIMIT = 1_000_000;

    public Meeting(Participant host) {
        this.host = host;
        this.participants = new ArrayList<>();
    }

    public Meeting(Participant host, ArrayList<Participant> participants) {
        this.host = host;
        this.participants = participants;
    }

    public void addParticipant(Participant participant) {
        if (totalParticipants >= TOTAL_PARTICIPANT_LIMIT) {
            throw new RuntimeException("participant limit reached");
        }

        this.participants.add(participant);
        Meeting.totalParticipants += 1;
    }

    public String getHostName() {
        return host.getName();
    }
}

// Extension, you can do it without a static attribute by making a wrapper class around
// meeting that holds all ongoing meetings and checking before addParticipant in that class
class Application {
    private ArrayList<Meeting> meetings;
    private int participants;

    public Application() {
        this.meetings = new ArrayList<>();
        this.participants = 0;
    }

    public void addParticipantToMeeting(Participant participant, int meetingId) {
        if (this.participants >= 1_000_000) {
            throw new RuntimeException("");
        }

        this.meetings.get(meetingId).addParticipant(participant);
        this.participants += 1;
    }

}