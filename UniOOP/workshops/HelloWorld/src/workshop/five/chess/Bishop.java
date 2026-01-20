package workshop.five.chess;

public class Bishop extends Piece{
    public Bishop(int currentRow, int currentColumn) {
        super(currentRow, currentColumn);
    }

    @Override
    protected boolean isValidMove(int toRow, int toColumn) {
        if (!super.isValidMove(toRow, toColumn)) {
            return false;
        }

        int deltaRow = this.getCurrentRow() - toRow;
        int deltaCol = this.getCurrentColumn() - toColumn;
        return Math.abs(deltaRow) == Math.abs(deltaCol);
    }
}
