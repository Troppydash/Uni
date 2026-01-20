package workshop.eleven;


class TooManyEntriesException extends CsvException {
    public TooManyEntriesException(int row, int expect, int got, String filename) {
        super("row " + row + " has " + got + " entries instead of " + expect, filename);
    }
}

class NotEnoughEntriesException extends CsvException {
    public NotEnoughEntriesException(int row, int expect, int got, String filename) {
        super("row " + row + " has " + got + " entries instead of " + expect, filename);
    }
}

class UnmatchedQuoteException extends CsvException {
    public UnmatchedQuoteException(int row, int col, String filename) {
        super("quote not matched for entry at row " + row + " column " + col, filename);
    }
}

public class CsvException extends Exception {
    public CsvException() {
        super("cannot parse csv file");
    }

    public CsvException(String filename) {
        super("cannot parse csv file " + filename);
    }

    public CsvException(String message, String filename) {
        super(message + " for csv file " + filename);
    }
}
