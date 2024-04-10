package workshop.five.chess;

public class Pawn extends Piece {
    private boolean hasMoved;

    public Pawn(int currentRow, int currentColumn) {
        super(currentRow, currentColumn);
        this.hasMoved = false;
    }

    @Override
    protected void handleMove() {
        this.hasMoved = true;
    }

    @Override
    protected boolean isValidMove(int toRow, int toColumn) {
        if (!super.isValidMove(toRow, toColumn)) {
            return false;
        }

        if (this.hasMoved) {
            return toColumn - 1 == this.getCurrentColumn() && toRow == this.getCurrentRow();
        } else {
            return (toColumn - 1 == this.getCurrentColumn() || toColumn - 2 == this.getCurrentColumn()) && toRow == this.getCurrentRow();
        }
    }
}
