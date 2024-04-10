package workshop.five.chess;

public class InheritanceTester {
    public static void main(String[] args) {
        Rook rook1 = new Rook(2, 4);

        // getter/setter from superclass
        System.out.println("rook1 current row:" + rook1.getCurrentRow());
        System.out.println("rook1 current column:" + rook1.getCurrentColumn());
        System.out.println(rook1.isValidMove(2,3));
        rook1.move(4,10);
        System.out.println("rook1 location: " + rook1);
        System.out.println();

        


        Piece rook2 = new Rook(3,5);
        rook2.move (4,4);
        System.out.println("rook2 location: " + rook2);
        // upcasting results in .equals method
        System.out.println(rook1.equals(rook2));
        System.out.println();

        // downcasting
        //Rook rook3 = new Piece(4,6);

        Piece rook3 = new Piece(4,6);
        rook3.move(8,12);
        System.out.println("rook3 location: " + rook3);

    }
}
