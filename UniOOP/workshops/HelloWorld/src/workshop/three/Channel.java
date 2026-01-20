package workshop.three;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

class Show {
    private final String name;
    private final int duration;  // in minutes
    private final int airtime;  // in minutes, [0, 24*60]

    public Show(String name, int duration, int airtimeH, int airtimeM) {
        this.name = name;
        this.duration = duration;
        this.airtime = airtimeH * 24 + airtimeM;
    }

    public boolean doesClash(Show other) {
        // we include the end point as clashing so every time has a unique state
        if (this.airtime <= other.airtime) {
            return this.airtime + this.duration >= other.airtime;
        } else {
            return other.airtime + other.duration >= this.airtime;
        }
    }

    public boolean containsTime(int time) {
        return time >= this.airtime && time <= this.duration;
    }

    public String getName() {
        return name;
    }

    public int getDuration() {
        return duration;
    }

    public int getAirtime() {
        return airtime;
    }
}

public class Channel {
    private final String name;
    private ArrayList<Show> shows;

    public Channel(String name) {
        this.shows = new ArrayList<>(5);
        this.name = name;
    }

    public void addShow(Show newShow) {
        if (this.shows.size() >= 5) {
            return;
        }

        for (var show : this.shows) {
            if (show.doesClash(newShow)) {
                return;
            }
        }

        this.shows.add(newShow);
    }

    public void removeShow(String name) {
        this.shows.removeIf(show -> show.getName().equals(name));
    }

    public Optional<Show> getShow(int hour, int minute) {
        int time = hour * 24 + minute;
        for (var show : this.shows) {
            if (show.containsTime(time)) {
                return Optional.of(show);
            }
        }

        return Optional.empty();
    }

    public boolean hasShow(String name) {
        return this.shows.stream().anyMatch(show -> show.getName().equals(name));
    }

    public String getName() {
        return this.name;
    }
}

class Network {
    private final String name;
    private ArrayList<Channel> channels;

    public Network(String name) {
        this.name = name;
        this.channels = new ArrayList<>(3);
    }

    public void addChannel(Channel channel) {
        if (this.channels.size() >= 3) {
            return;
        }

        this.channels.add(channel);
    }

    public void removeChanel(String name) {
        this.channels.removeIf(channel -> channel.getName().equals(name));
    }

    public HashMap<String, Show> getShows(int hour, int minute) {
        HashMap<String, Show> output = new HashMap<>();
        for (var channel : this.channels) {
            Optional<Show> show = channel.getShow(hour, minute);
            if (show.isPresent()) {
                output.put(channel.getName(), show.get());
            }
        }

        return output;
    }

    public List<Channel> lookupShow(String name) {
        return this.channels.stream().filter(channel -> channel.hasShow(name)).collect(Collectors.toList());
    }
}