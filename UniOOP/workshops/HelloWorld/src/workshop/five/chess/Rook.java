package workshop.five.chess;

public class Rook extends Piece {

    public final String name;

    public Rook(int currentRow, int currentColumn) {
        super(currentRow, currentColumn);
        this.name = "Rook";
        // Any other code
    }

    public Rook(int currentRow, int currentColumn, String name) {
        super(currentRow, currentColumn);
        this.name = name;
        // Any other code
    }

    @Override
    public String toString() {
        return "hello";
    }
// public int accessCurrentRow() { return super.currentColumn;}

    @Override
    public boolean isValidMove(int toRow, int toColumn) {
        boolean isValid = true;
        System.out.println("Rook class: isValidMove() method");
        if (!super.isValidMove(toRow, toColumn))
            return false;
        //Logic for checking valid move and set isValid
        return isValid;
    }


/*    @Override
    public String toString() {
        return "I am a rook at" + "(" + super.getCurrentRow() + "," + super.getCurrentColumn() + ")";
    }*/


}
