package workshop.twelve;

import java.util.Map;

public enum Direction {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST;

    public int toDegrees() {
        Map<Direction, Integer> lookup = Map.of(
                NORTH, 0,
                SOUTH, 180,
                EAST, 90,
                WEST, 270,
                NORTH_EAST, 45,
                NORTH_WEST, 270 + 45,
                SOUTH_EAST, 90 + 45,
                SOUTH_WEST, 180 + 45
        );
        return lookup.get(this);
    }

    public static void main(String[] args) {
        System.out.println(NORTH.toDegrees());
        System.out.println(SOUTH.toDegrees());
    }
}
