package preworkshops.two;

public class Participant {
    private String name;
    private boolean videoOn = false;
    private boolean muted = true;

    public Participant(String name) {
        this.name = name;
    }

    public String getName() {
        return this.name;
    }

    public void joinMeeting(Meeting workshop) {
        try {
            workshop.addParticipant(this);
            // participants would greet the meeting's host after joining:)
            this.videoOn = true;
            this.muted = false;
            System.out.println(this.name + " says hi to " + workshop.getHostName());
        } catch (RuntimeException e) {
            System.out.println(e);
        }

    }
}