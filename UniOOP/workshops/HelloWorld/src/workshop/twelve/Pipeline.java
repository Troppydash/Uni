package workshop.twelve;


import java.util.Arrays;
import java.util.List;

class Show {
    double getRatings() {
        return 0.0f;
    }
}

class IMDB {
    static boolean isTVShow(String name) {
        return true;
    }

    static Show getShow(String name) {
        return null;
    }
}

public class Pipeline {

    public void javaPipeline() {
        List<String> list = Arrays.asList("Avengers: End Game", "Game of Thrones",
                "Jon Snow", "Arya", "SWEN20003", "Suits");

        int count = 0;
        for (var name : list) {
            if (!IMDB.isTVShow(name)) {
                continue;
            }

            var show = IMDB.getShow(name);
            if (show.getRatings() > 4.0) {
                count += 1;
            }
        }
    }

    public void streamPipeline() {
        /*
         * The pipeline takes a list of shows, then for each show:
         *  filters for if the show is a tv show in IMDB
         *  gets the show instance
         *  filters for the shows with a higher than 4 rating
         *  counts the number of shows left
         *
         * Essentially, it counts the number of IMDB TV shows with a higher than 4 rating
         */

        List<String> list = Arrays.asList("Avengers: End Game", "Game of Thrones",
                "Jon Snow", "Arya", "SWEN20003", "Suits");
        long count = list.stream()
                .filter(IMDB::isTVShow)
                .map(name -> IMDB.getShow(name))
                .filter(show -> show.getRatings() > 4.0)
                .count();
    }

    public static void main(String[] args) {


    }
}
