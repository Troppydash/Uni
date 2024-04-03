package workshop.five.chess;

import java.util.Objects;

public class Piece {
    private int currentRow;
    private int currentColumn;
    final static int BOARD_SIZE = 8;

    // constructor
    public Piece(int currentRow, int currentColumn) {
        this.currentRow = currentRow;
        this.currentColumn = currentColumn;
    }

    // getters and setters
    public int getCurrentRow() { return this.currentRow;}
    public void setCurrentRow(int currentRow) {
        this.currentRow = currentRow;
    }
    public int getCurrentColumn() { return this.currentColumn;}
    public void setCurrentColumn(int currentColumn) {
        this.currentColumn = currentColumn;
    }

    public final boolean move(int toRow, int toColumn) {
        System.out.println("Piece class: move() method");
        if (!isValidMove(toRow, toColumn)) return false;
        this.currentRow = toRow;
        this.currentColumn = toColumn;
        this.handleMove();

        return true;
    }

    protected void handleMove() {

    }

    protected boolean isValidMove(int toRow, int toColumn) {
        System.out.println("Piece class: isValidMove() method");
        return toRow >= 0 && toRow < BOARD_SIZE &&
                toColumn >= 0 && toColumn < BOARD_SIZE;
    }

    @Override
    public String toString() { return "(" + currentRow + "," + currentColumn + ")"; }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Piece piece = (Piece) o;
        return currentRow == piece.currentRow &&
                currentColumn == piece.currentColumn;
    }

    @Override
    public int hashCode() {
        return Objects.hash(currentRow, currentColumn);
    }
}
